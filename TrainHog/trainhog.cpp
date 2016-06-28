#include <opencv2/opencv.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <time.h>

using namespace cv;
using namespace cv::ml;
using namespace std;

string get_time();
void get_svm_detector(const Ptr<SVM>& svm, vector< float > & hog_detector );
void convert_to_ml(const std::vector< cv::Mat > & train_samples, cv::Mat& trainData );
void load_images( const string & prefix, const string & filename, vector< Mat > & img_lst );
void sample_neg( const vector< Mat > & full_neg_lst, vector< Mat > & neg_lst, const Size & size );
void compute_hog( const vector< Mat > & img_lst, vector< Mat > & gradient_lst, const Size & size );
void train_svm( const vector< Mat > & gradient_lst, const vector< int > & labels );

int win_size_w;
int win_size_h;
int block_size;
int block_stride;
int cell_size;
int hog_n_bins;

string get_time() {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%Y_%m_%d_%I_%M_%S",timeinfo);
    return string(buffer);
}

void get_svm_detector(const Ptr<SVM>& svm, vector< float > & hog_detector )
{
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
void convert_to_ml(const std::vector< cv::Mat > & train_samples, cv::Mat& trainData )
{
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

void load_images( const string & prefix, const string & filename, vector< Mat > & img_lst )
{
    string line;
    ifstream file;

    file.open( (prefix+filename).c_str() );
    if( !file.is_open() )
    {
        cerr << "Unable to open the list of images from " << filename << " filename." << endl;
        exit( -1 );
    }

    bool end_of_parsing = false;
    while( !end_of_parsing )
    {
        getline( file, line );
        if( line.empty() ) // no more file to read
        {
            end_of_parsing = true;
            break;
        }
        Mat img = imread( (prefix+line).c_str() ); // load the image
        if( img.empty() ) // invalid image, just skip it.
            continue;
#ifdef _DEBUG
        imshow( "image", img );
        waitKey( 10 );
#endif
        img_lst.push_back( img.clone() );
    }
}

void sample_neg( const vector< Mat > & full_neg_lst, vector< Mat > & neg_lst, const Size & size )
{
    Rect box;
    box.width = size.width;
    box.height = size.height;

    const int size_x = box.width;
    const int size_y = box.height;

    srand( (unsigned int)time( NULL ) );

    vector< Mat >::const_iterator img = full_neg_lst.begin();
    vector< Mat >::const_iterator end = full_neg_lst.end();
    for( ; img != end ; ++img )
    {
        for (int i = 0; i < img->cols && (img->cols - i) > size_x; i += size_x) {
            for (int j = 0; j < img->rows && (img->rows - j) > size_y; j += size_y) {
                box.x = i;
                box.y = j;
                Mat roi = (*img)(box);
                neg_lst.push_back( roi.clone() );
            }
        }
    }
}

void compute_hog( const vector< Mat > & img_lst, vector< Mat > & gradient_lst, const Size & size )
{
    HOGDescriptor hog;
    hog.winSize = size;
    hog.blockSize = Size(block_size, block_size);
    hog.blockStride = Size(block_stride, block_stride);
    hog.cellSize = Size(cell_size, cell_size);
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

void train_svm( const vector< Mat > & gradient_lst, const vector< int > & labels, string model_file )
{

    Mat train_data;
    convert_to_ml( gradient_lst, train_data );

    clog << "Start training...";
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
    clog << "...[done]" << endl;

    svm->save( model_file );
}

int main( int argc, char** argv )
{
    cv::CommandLineParser parser(argc, argv, "{help h|| show help message}"
            "{pd||pos_dir}{p||pos.lst}{nd||neg_dir}{n||neg.lst}{block||block_size}{stride||block_stride}{cell||cell_size}{wh||win_height}{ww||win_width}");
    if (parser.has("help"))
    {
        parser.printMessage();
        exit(0);
    }
    vector< Mat > pos_lst;
    vector< Mat > full_neg_lst;
    vector< Mat > neg_lst;
    vector< Mat > gradient_lst;
    vector< int > labels;
    string pos_dir = parser.get<string>("pd");
    string pos = parser.get<string>("p");
    string neg_dir = parser.get<string>("nd");
    string neg = parser.get<string>("n");

    block_size = parser.get<int>("block");
    block_stride = parser.get<int>("stride");
    cell_size = parser.get<int>("cell");
    win_size_w = parser.get<int>("ww");
    win_size_h = parser.get<int>("wh");

    cout << "Block size: " << block_size << endl <<
            "Block stride: " << block_stride << endl <<
            "Cell size: " << cell_size << endl <<
            "Window width: " << win_size_w << endl <<
            "Window height: " << win_size_h << endl;

    Size size = Size(win_size_w,win_size_h);
    if( pos_dir.empty() || pos.empty() || neg_dir.empty() || neg.empty() ) {

        cout << "Wrong number of parameters." << endl
            << "Usage: " << argv[0] << " --pd=pos_dir -p=pos.lst --nd=neg_dir -n=neg.lst" << endl
            << "example: " << argv[0] << " --pd=/INRIA_dataset/ -p=Train/pos.lst --nd=/INRIA_dataset/ -n=Train/neg.lst" << endl;
        exit( -1 );
    }


    load_images( pos_dir, pos, pos_lst );
    labels.assign( pos_lst.size(), +1 );
    const unsigned int old = (unsigned int)labels.size();
    load_images( neg_dir, neg, full_neg_lst );
    sample_neg( full_neg_lst, neg_lst, size );
    labels.insert( labels.end(), neg_lst.size(), -1 );
    CV_Assert( old < labels.size() );

    compute_hog( pos_lst, gradient_lst, size );
    compute_hog( neg_lst, gradient_lst, size );

    stringstream model_name;
    model_name << "_" << pos
               << "_block" << block_size
               << "_stride" << block_stride
               << "_cell" << cell_size
               << "_ww" << win_size_w
               << "_wh" << win_size_h
               << ".xml";

    string outputModel = "model" + model_name.str();
    train_svm( gradient_lst, labels, outputModel);

    stringstream model_specs;
    model_specs << "specs" << model_name.str();

    FileStorage fs(model_specs.str(), FileStorage::WRITE);
    fs << "model" << outputModel;
    fs << "blocksize" << block_size;
    fs << "blockstride" << block_stride;
    fs << "cellsize" << cell_size;
    fs << "windowwidth" << win_size_w;
    fs << "windowheight" << win_size_h;
    fs.release();

    return 0;
}
