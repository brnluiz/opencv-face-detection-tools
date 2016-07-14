#include <opencv2/opencv.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "dataset.h"
#include "negdataset.h"
#include "kfold.h"

#include "objectdetectors/hogsvmdetector.h"

using namespace std;
using namespace cv;
using namespace cv::ml;

struct ConfusionMatrix {
    uint positives;
    uint negatives;
    uint false_positives;
    uint false_negatives;
};

float get_accuracy(ConfusionMatrix stat) {
    return (float)(stat.positives + stat.negatives) /
            (float)(stat.positives + stat.negatives + stat.false_positives + stat.false_negatives);
}

void get_svm_detector(const Ptr<SVM>& svm, vector< float > & hog_detector ) {
    // get the support vectors
    Mat sv = svm->getSupportVectors();
    const int sv_total = sv.rows;
    // get the decision function
    Mat alpha, svidx;
    double rho = svm->getDecisionFunction(0, alpha, svidx);

    CV_Assert( alpha.total() == 1 && svidx.total() == 1 && sv_total == 1 );
    CV_Assert( (alpha.type() == CV_64F && alpha.at<double>(0) == 1.) ||
               (alpha.type() == CV_32F && alpha.at<float>(0) == 1.f) );
    CV_Assert( sv.type() == CV_32F );
    hog_detector.clear();

    hog_detector.resize(sv.cols + 1);
    memcpy(&hog_detector[0], sv.ptr(), sv.cols*sizeof(hog_detector[0]));
    hog_detector[sv.cols] = (float)-rho;
}

/*
* Convert training/testing set to be used by OpenCV Machine Learning algorithms.
* TrainData is a matrix of size (#samples x max(#cols,#rows) per samples), in 32FC1.
* Transposition of samples are made if needed.
*/
void convert_to_ml(const std::vector< cv::Mat > & train_samples, cv::Mat& trainData ) {
    //--Convert data
    const int rows = (int)train_samples.size();
    const int cols = (int)std::max( train_samples[0].cols, train_samples[0].rows );
    cv::Mat tmp(1, cols, CV_32FC1); //< used for transposition if needed
    trainData = cv::Mat(rows, cols, CV_32FC1 );
    vector< Mat >::const_iterator itr = train_samples.begin();
    vector< Mat >::const_iterator end = train_samples.end();
    for( int i = 0 ; itr != end ; ++itr, ++i )
    {
        CV_Assert( itr->cols == 1 ||
            itr->rows == 1 );
        if( itr->cols == 1 )
        {
            transpose( *(itr), tmp );
            tmp.copyTo( trainData.row( i ) );
        }
        else if( itr->rows == 1 )
        {
            itr->copyTo( trainData.row( i ) );
        }
    }
}

void compute_hog( const vector< Mat > & img_lst, vector< Mat > & gradient_lst, const Size & size, HOGDescriptor hog ) {
    Mat gray;
    vector< Point > location;
    vector< float > descriptors;

    vector< Mat >::const_iterator img = img_lst.begin();
    vector< Mat >::const_iterator end = img_lst.end();
    Mat flipped;
    for( ; img != end ; ++img )
    {
        // Turn the colored photos to gray ones
        cvtColor( *img, gray, COLOR_BGR2GRAY );

        // Equalize the histogram
        equalizeHist( gray, gray );

        // Resize it considering the passed size as variable
        resize(gray, gray, size);

        // Flip the image
        flip(gray, flipped, 0);

        hog.compute( gray,    descriptors, hog.blockStride, Size( 0, 0 ), location );
        hog.compute( flipped, descriptors, hog.blockStride, Size( 0, 0 ), location );
        gradient_lst.push_back( Mat( descriptors ).clone() );
    }
}

void train_svm( const vector< Mat > & gradient_lst, const vector< int > & labels, string model_file ) {

    Mat train_data;
    convert_to_ml( gradient_lst, train_data );

    Ptr<SVM> svm = SVM::create();
    /* Default values to train SVM */
    svm->setCoef0(0.0);
    svm->setDegree(3);
    svm->setTermCriteria(TermCriteria( CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, 1e-6 ));
    svm->setGamma(0);
    svm->setKernel(SVM::LINEAR);
    svm->setNu(0.5);
    svm->setP(0.1); // for EPSILON_SVR, epsilon in loss function?
    svm->setC(0.01); // From paper, soft classifier
    svm->setType(SVM::EPS_SVR); // C_SVC; // EPSILON_SVR; // may be also NU_SVR; // do regression task
    svm->train(train_data, ROW_SAMPLE, Mat(labels));

    svm->save( model_file );
}

enum {
    BLOCK_SIZE,
    CELL_SIZE,
    BLOCK_STRIDE,
    WIN_SIZE
};

void combine_vector (
    vector<vector<int> > & output_perms,
    vector<vector<int> > const & input,
    vector<int> & cur_perm,
    unsigned cur_row = 0
) {
    if (cur_row >= input.size()) {
        // This is where you have found a new permutation.
        // Do whatever you want with it.
        output_perms.push_back (cur_perm);
        return;
    }

    for (unsigned i = 0; i < input[cur_row].size(); ++i) {
        cur_perm.push_back (input[cur_row][i]);
        combine_vector (output_perms, input, cur_perm, cur_row + 1);
        cur_perm.pop_back ();
    }
}

template<class T>
void print_vector(vector<T> & v) {
    cout << "[ ";
    for(uint i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << "]" << endl;
}

struct ModelInfo {
    string name;
    struct {
        int block_size;
        int cell_size;
        int block_stride;
        int win_size;
    } specs;
    float acc;
    ConfusionMatrix confusion;
};

struct by_acc {
    bool operator ()(ModelInfo const &a, ModelInfo const &b) {
        return a.acc < b.acc;
    }
};

int main(int argc, char *argv[]) {
    Size size = Size(32,32);
    vector<ModelInfo> model_infos;

    // HOG Parameters (which will be combined and tested)
    vector<vector<int>> params = {
//        {4, 8, 16},    // Block size
//        {4, 8, 16},    // Cell size
//        {2, 4, 8, 16}, // Block stride
        {8},    // Block size
        {8, 16},    // Cell size
        {8, 16}, // Block stride
        {32},          // Win size
    };

    // Get all possible parameters combinations
    // TODO: maybe some combinations will need to be excluded
    cout << "# Generating combinations..." << endl;
    vector<vector<int> > params_combos;
    vector<int> temp;
    combine_vector(params_combos, params, temp);
    cout << "~ Generated combinations: " << params_combos.size() << endl;

    // Load all images
    cout << "# Loading images sets..." << endl;
    DataSet    pos_set("/home/brunoluiz/qt/FaceDetectionTools/builds/release/linux/x86/TrainHog/", "caltech.lst");
    NegDataSet neg_set("/home/brunoluiz/qt/FaceDetectionTools/builds/release/linux/x86/TrainHog/", "neg.lst", size);
    cout << "~ Finished loading!" << endl;

    // Setup the cross-folding
    const int folds = 10;
    Kfold<vector<Mat>::const_iterator> kf_pos(folds, pos_set.get().begin(), pos_set.get().end());
    Kfold<vector<Mat>::const_iterator> kf_neg(folds, neg_set.get().begin(), neg_set.get().end());

    cout << "# Starting the training + testing process..." << endl;
    for(uint i = 0; i < params_combos.size(); i++) {
        ModelInfo info;

        // Get the HOG parameters
        int block_size = params_combos[i][0];
        int cell_size = params_combos[i][1];
        int block_stride = params_combos[i][2];
        int win_size = params_combos[i][3];

        // HOG Parameters Configs
        HOGDescriptor hog;
        hog.blockSize = Size(block_size, block_size);
        hog.cellSize = Size(cell_size, cell_size);
        hog.blockStride = Size(block_stride, block_stride);
        hog.winSize = Size(win_size, win_size);

        cout << "> Training and Testing combination set " << i << ": ";
        print_vector(params_combos[i]);

        // Cross-folding process
        vector<Mat> train_pos, test_pos;
        vector<Mat> train_neg, test_neg;
        vector<ConfusionMatrix> confusion_stats;
        try {
            for (uint fold = 0; fold != folds; fold++) {
                vector< int > labels;
                vector< Mat > gradient_lst;

                vector<Mat>::iterator it;
                kf_pos.getFold(fold + 1, back_inserter(train_pos), back_inserter(test_pos));
                kf_neg.getFold(fold + 1, back_inserter(train_neg), back_inserter(test_neg));

                // Allocate the SVM parameters
                labels.assign( train_pos.size(), +1 );
                const unsigned int old = (unsigned int)labels.size();
                labels.insert( labels.end(), train_neg.size(), -1 );
                CV_Assert( old < labels.size() );

                // Compute HOG for the training set
                compute_hog(train_pos, gradient_lst, size, hog);
                compute_hog(train_neg, gradient_lst, size, hog);

                // Configure the name of the output model
                stringstream model_name;
                model_name << "_fold" << fold
                           << "_block" << block_size
                           << "_cell" << cell_size
                           << "_stride" << block_stride
                           << "_ww" << win_size
                           << "_wh" << win_size
                           << ".xml";

                // Train the SVM
                cout << " - Training Fold " << fold + 1 << "..." << endl;
                string outputModel = "model" + model_name.str();
                train_svm( gradient_lst, labels, outputModel);

                // Configure the specs
                stringstream model_specs;
                model_specs << "specs" << model_name.str();

                FileStorage fw(model_specs.str(), FileStorage::WRITE);
                fw << "model" << outputModel;
                fw << "blocksize" << block_size;
                fw << "blockstride" << block_stride;
                fw << "cellsize" << cell_size;
                fw << "windowwidth" << win_size;
                fw << "windowheight" << win_size;
                fw.release();

                // Create a HOG Detector with the actual model
                FileStorage fr(model_specs.str(), FileStorage::READ);
                HogSvmDetector detector(fr);

                // Use the test set and measure the accuracy
                ConfusionMatrix stat;
                cout << " - Testing Fold " << fold + 1 << " against positive samples" << endl;
                for (uint i = 0; i < test_pos.size(); i++) {
                    Objects detections = detector.detect(test_pos[i]);
                    if (detections.size() != 0) {
                        stat.positives++;
                    } else {
                        stat.false_negatives++;
                    }
                }

                cout << " - Testing Fold " << fold + 1 << " against negative samples" << endl;
                for (uint i = 0; i < test_neg.size(); i++) {
                    Objects detections = detector.detect(test_neg[i]);
                    if (detections.size() != 0) {
                        stat.false_positives++;
                    } else {
                        stat.negatives++;
                    }
                }

                float acc = get_accuracy(stat);
                cout << " - Testing Fold " << fold + 1 << " accuracy: " << acc << endl;
                confusion_stats.push_back(stat);

                train_pos.clear();
                train_neg.clear();
                test_pos.clear();
                test_neg.clear();
            }
        } catch (cv::Exception &e) {
            cerr << "[opencv][error] " << e.what() << endl;
            continue;
        }
        // Save the model name
        stringstream model_name;
        model_name << "_block" << block_size
                   << "_cell" << cell_size
                   << "_stride" << block_stride
                   << "_ww" << win_size
                   << "_wh" << win_size;
        info.name = model_name.str();

        // Save the total accuracy of the cross-folding process
        float total_acc = 0;
        for (uint i = 0; i < confusion_stats.size(); i++) {
            total_acc += get_accuracy(confusion_stats[i]);
        }
        cout << "> Combination set " << i << " accuracy: " << total_acc << endl;
        total_acc = total_acc/folds;
        info.acc = total_acc;

        // Save the model specs
        info.specs.block_size = block_size;
        info.specs.block_stride = block_stride;
        info.specs.cell_size = cell_size;
        info.specs.win_size = win_size;

        model_infos.push_back(info);
    }

    cout << "# Overall test accuracies" << endl;
    sort(model_infos.begin(), model_infos.end(), by_acc());
    for(uint i = 0; i < model_infos.size(); i++) {
        cout << model_infos[i].name << " | acc: " << model_infos[i].acc << endl;
    }

    return 0;
}
