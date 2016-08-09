#include "trainer_svm.h"

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

//    if( param_id == SVM::C )
//    {
//        grid.minVal = 0.1;
//        grid.maxVal = 500;
//        grid.logStep = 5; // total iterations = 5
//    }
//    else if( param_id == SVM::GAMMA )
//    {
//        grid.minVal = 1e-5;
//        grid.maxVal = 0.6;
//        grid.logStep = 15; // total iterations = 4
//    }
//    else if( param_id == SVM::P )
//    {
//        grid.minVal = 0.01;
//        grid.maxVal = 100;
//        grid.logStep = 7; // total iterations = 4
//    }
//    else if( param_id == SVM::NU )
//    {
//        grid.minVal = 0.01;
//        grid.maxVal = 0.2;
//        grid.logStep = 3; // total iterations = 3
//    }
//    else if( param_id == SVM::COEF )
//    {
//        grid.minVal = 0.1;
//        grid.maxVal = 300;
//        grid.logStep = 14; // total iterations = 3
//    }
//    else if( param_id == SVM::DEGREE )
//    {
//        grid.minVal = 0.01;
//        grid.maxVal = 4;
//        grid.logStep = 7; // total iterations = 3
//    }

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
        float acc = 0;
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
                TRAINERSVM_LOG << " * Fold #" << fold+1 << ": testing..." << endl;
                Stats stat(hog_, test_pos, test_neg);
                stat.test();

                // Save the accuracy
                acc += stat.getAccuracy();

                TRAINERSVM_LOG << " + Fold #" << fold+1 << " accuracy: " << stat.getAccuracy() << endl;
            }
        } catch (cv::Exception &e) {
            continue;
        }

        // Average accuracy (based on the number of folds)
        acc = acc / folds_;
        TRAINERSVM_LOG << "- Final accuracy: " << acc << endl << endl;

        // If the average accuracy is better than the last parameter set, then save it
        if (acc > bestsvm_.acc) {
            bestsvm_.svm = svm;
            bestsvm_.acc = acc;

            TRAINERSVM_LOG << "New best SVM!" << endl;
            TRAINERSVM_LOG << bestsvm_;
        }
    }
}

Ptr<SVM> TrainerSvm::getBest() {
    return bestsvm_.svm;
}
