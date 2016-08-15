#include "trainer_svm.h"

#include <fstream>

#include "averaged_stats.h"
#include "performance_test.h"
#include "log.h"
#include "utils_hog.h"
#include "utils_svm.h"

TrainerSvm::TrainerSvm(vector<Mat> &pos, vector<Mat> &neg, const int& folds, HOGDescriptor &hog):
    TrainerAbstract(pos, neg, folds), hog_(hog) {
}

TrainerSvm::TrainerSvm(vector<Mat> &pos, vector<Mat> &neg, const int& folds, const HogParam &param): TrainerAbstract(pos, neg, folds) {
    hog_ = makeDescriptor(param);
}

#define FOR_IN_GRID(var, grid) \
            for( var = grid.minVal; var == grid.minVal || var < grid.maxVal; var = (grid.minVal == grid.maxVal) ? grid.maxVal + 1 : var * grid.logStep )

void TrainerSvm::run() {
//    // Allocate the SVM parameters (labels and HOG)
//    TRAINERSVM_LOG << "Preparing SVM parameters" << endl;
//    vector<int> labels;
//    vector<Mat> gradient_lst;
//    HogUtils::computeList(pos_, gradient_lst, labels, hog_, +1);
//    HogUtils::computeList(neg_, gradient_lst, labels, hog_, -1);

//    // Train a SVM using the actual HOGs
//    TRAINERSVM_LOG << "Training SVM and choosing the best parameters" << endl;
//    bestsvm_ = SvmUtils::train(gradient_lst, labels, true);

    // Prepare pos and negatives samples (if it is an invalid set, just continue to the next one)
    TRAINERSVM_LOG << "- Preparing samples: computing HOG and labeling the samples" << endl;
    vector<SampleInfo> pos_set, neg_set;
    try {
        pos_set = prepareSamples(pos_, hog_, +1);
        neg_set = prepareSamples(neg_, hog_, -1);
    } catch (cv::Exception &e) {
        ERROR_LOG << "Invalid set of parameters" << endl;
    }

    double C;
    double p;
    double gamma = 0;
    double nu = 0;
    double coef0 = 0;
    double degree = 0;

    ParamGrid C_grid = ParamGrid(.001, 1000, 5);
    ParamGrid p_grid = ParamGrid(.001, 500, 7);
    ParamGrid gamma_grid = ParamGrid(1e-5, .6, 15);
    ParamGrid nu_grid = ParamGrid(.01, .2, 3);
    ParamGrid coef_grid = ParamGrid(.1, 300, 14);
    ParamGrid degree_grid = ParamGrid(.01, 4, 7);

    FOR_IN_GRID(C, C_grid)
    FOR_IN_GRID(p, p_grid)
//    FOR_IN_GRID(gamma, gamma_grid)
//    FOR_IN_GRID(nu, nu_grid)
//    FOR_IN_GRID(coef0, coef_grid)
//    FOR_IN_GRID(degree, degree_grid)
    {
        Kfold<vector<SampleInfo>::const_iterator> kf_pos(folds_, pos_set.begin(), pos_set.end());
        Kfold<vector<SampleInfo>::const_iterator> kf_neg(folds_, neg_set.begin(), neg_set.end());

        Ptr<SVM> svm = SVM::create();

        /* Default values to train SVM */
        svm->setType(SVM::EPS_SVR); // C_SVC; // EPSILON_SVR; // may be also NU_SVR; // do regression task
        svm->setKernel(SVM::LINEAR);
        svm->setTermCriteria(TermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, 1e-6 ));

        /* Required to set for EPS_SVR and LINEAR kernel */
        svm->setC(C);
        svm->setP(p);

        /* Not used for EPS_SVR */
        svm->setNu(nu);

        /* Not used for Linear Kernel */
        svm->setDegree(degree);
        svm->setGamma(gamma);
        svm->setCoef0(coef0);

        // Cross-validation
        AveragedStats avg_stats;
        try {
            TRAINERSVM_LOG << "- Cross validating..." << endl;
            for (int fold = 0; fold != folds_; fold++) {
                TRAINERSVM_LOG << " * Fold #" << fold+1 << ": training..."<< endl;

                // Get the k fold for both sets (training and testing)
                vector<SampleInfo> train_pos, test_pos;
                vector<SampleInfo> train_neg, test_neg;
                kf_pos.getFold(fold + 1, back_inserter(train_pos), back_inserter(test_pos));
                kf_neg.getFold(fold + 1, back_inserter(train_neg), back_inserter(test_neg));

                // Allocate the SVM parameters (labels and HOG)
                vector<int> labels;
                vector<Mat> gradient_lst;
                prepareSvmParameters(gradient_lst, labels, train_pos, train_neg);

                // Train a SVM using the actual HOGs
                Mat train_data;
                SvmUtils::convertData( gradient_lst, train_data );
                Ptr<TrainData> train = TrainData::create(train_data, SampleTypes::ROW_SAMPLE, Mat(labels));
                svm->train(train);

                // Configure the detector to use the trained SVM
                vector<float> detector = SvmUtils::getDetector(svm);
                hog_.setSVMDetector(detector);

                // Test the actual setting using the test set
                TRAINERHOG_LOG << " * Fold #" << fold+1 << ": testing..." << endl;
                PerformanceTest tester(hog_, test_pos, test_neg);
                tester.test();

                // Save the accuracy
                avg_stats.insert(tester.getStats());
            }
        } catch (cv::Exception &e) {
            continue;
        }

        TRAINERHOG_LOG << "Average results: " << endl;
        cout << avg_stats;

        // If the average accuracy is better than the last parameter set, then save it
        if (avg_stats.getFScore() > best_.stat.getFScore()) {
            best_.svm = svm;
            best_.stat = avg_stats;

            TRAINERSVM_LOG << "New best SVM!" << endl;
            cout << best_;
        }
    }
}

void TrainerSvm::saveReport(const string &output_path) {
    // Open a file for the report
    ofstream report;
    report.open(output_path);
    if(!report.is_open()) {
        exit(-1);
    }

    // Save the report
    report << best_;
    report.close();
}

BestSvm TrainerSvm::getBest() {
    return best_;
}
