#include "utils_svm.h"

namespace SvmUtils {

    /*
    * Convert training/testing set to be used by OpenCV Machine Learning algorithms.
    * TrainData is a matrix of size (#samples x max(#cols,#rows) per samples), in 32FC1.
    * Transposition of samples are made if needed.
    */
    void convertData(const std::vector<cv::Mat> & train_samples, cv::Mat& trainData) {
        //--Convert data
        const int rows = (int)train_samples.size();
        const int cols = (int)std::max( train_samples[0].cols, train_samples[0].rows );
        cv::Mat tmp(1, cols, CV_32FC1); //< used for transposition if needed
        trainData = cv::Mat(rows, cols, CV_32FC1 );
        vector<Mat>::const_iterator itr = train_samples.begin();
        vector<Mat>::const_iterator end = train_samples.end();
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

    Ptr<SVM> train(const vector<Mat>& gradient_lst, const vector<int>& labels, bool automatic) {

        Mat train_data;
        SvmUtils::convertData( gradient_lst, train_data );
        Ptr<TrainData> train = TrainData::create(train_data, SampleTypes::ROW_SAMPLE, Mat(labels));

        Ptr<SVM> svm = SVM::create();

        svm->setType(SVM::EPS_SVR); // C_SVC; // EPSILON_SVR; // may be also NU_SVR; // do regression task
        svm->setKernel(SVM::LINEAR);
        svm->setTermCriteria(TermCriteria(CV_TERMCRIT_ITER, 1000, 1e-6 ));

        /* Default values to train SVM */
        svm->setC(10); // (C_SVC / EPS_SVR / NU_SVR)
        svm->setP(0.05);  // (EPS_SVR)

        /* Not used for EPS_SVR */
        svm->setNu(0); // (NU_SVC / ONE_CLASS / NU_SVR)

        /* Not used for Linear Kernel */
        svm->setDegree(0);   // (POLY)
        svm->setCoef0(0.0);  // (POLY / SIGMOID)
        svm->setGamma(0.0); // (POLY / RBF / SIGMOID / CHI2)

        if (!automatic) {
            svm->train(train);
        } else {
            svm->trainAuto(train, 10,
                           SVM::getDefaultGrid(SVM::C),
                           SVM::getDefaultGrid(SVM::GAMMA),
                           SVM::getDefaultGrid(SVM::P),
                           SVM::getDefaultGrid(SVM::NU),
                           SVM::getDefaultGrid(SVM::COEF),
                           SVM::getDefaultGrid(SVM::DEGREE),
                           true);
        }

        return svm;
    }

    vector<float> getDetector(const Ptr<SVM>& svm) {
        // SVM Detector output
        vector<float> hog_detector;

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

        return hog_detector;
    }

}
