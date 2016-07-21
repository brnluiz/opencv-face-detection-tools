#include "trainer.h"

vector<float> Trainer::getSvmDetector(const Ptr<SVM> &svm) {
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

vector<Mat> Trainer::computeHog(const vector<Mat> &img_lst, HOGDescriptor hog) {
    // Output variable
    vector<Mat> gradient_lst;

    vector< Mat >::const_iterator img = img_lst.begin();
    vector< Mat >::const_iterator end = img_lst.end();
    for( ; img != end ; ++img ) {
        vector<float> descriptors;

        // Compute HoG (normal image)
        descriptors = computeHog(*img, hog);
        gradient_lst.push_back(Mat(descriptors).clone());
        descriptors.clear();

        // Compute HoG (flipped image)
        descriptors = computeHog(*img, hog);
        gradient_lst.push_back(Mat(descriptors).clone());
    }

    return gradient_lst;
}

vector<float> Trainer::computeHog(Mat img, HOGDescriptor hog, bool do_flip) {
    Mat gray;
    vector<float> descriptors;

    // Turn the colored photos to gray ones
    cvtColor( img, gray, COLOR_BGR2GRAY );

    // Equalize the histogram
    equalizeHist( gray, gray );

    // Resize it considering the passed size as variable
    resize(gray, gray, hog.winSize);

    // Flip the image
    if (do_flip) flip(gray, gray, 0);

    // Compute HoG
    hog.compute(gray, descriptors, hog.blockStride);

    return descriptors;
}

void Trainer::trainSvm(const vector<Mat> &gradient_lst, const vector<int> &labels, string model_file) {
    Ptr<SVM> svm;
//    svm = trainSvm(gradient_lst, labels);

    svm->save(model_file);
}

void Trainer::trainSvm(const vector<Mat> &gradient_lst, const vector<int> &labels, Ptr<SVM>& svm, bool automatic) {

    Mat train_data;
    convertToMl( gradient_lst, train_data );
    Ptr<TrainData> train = TrainData::create(train_data, SampleTypes::ROW_SAMPLE, Mat(labels));

//    svm->setKernel(SVM::RBF);
//    svm->setType(SVM::C_SVC); // C_SVC; // EPSILON_SVR; // may be also NU_SVR; // do regression task
//    svm->setTermCriteria(TermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, 1e-6 ));

//    /* Default values to train SVM */
//    svm->setCoef0(0.0); //(POLY / SIGMOID)
//    svm->setDegree(3); // (POLY)
//    svm->setNu(0.5); // (NU_SVC / ONE_CLASS / NU_SVR)
//    svm->setP(0.1); // (EPS_SVR)

//    svm->setC(0.01); // (C_SVC / EPS_SVR / NU_SVR)
//    svm->setGamma(0.01); // (POLY / RBF / SIGMOID / CHI2)

    if (!automatic) {
        svm->setCoef0(0.0);
        svm->setDegree(3);
        svm->setTermCriteria(TermCriteria( CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, 1e-6 ));
        svm->setGamma(0);
        svm->setKernel(SVM::LINEAR);
        svm->setNu(0.5);
        svm->setP(0.1); // for EPSILON_SVR, epsilon in loss function?
        svm->setC(0.01); // From paper, soft classifier
        svm->setType(SVM::EPS_SVR); // C_SVC; // EPSILON_SVR; // may be also NU_SVR; // do regression task
        svm->train(train);
//        return svm;
    }

    svm->trainAuto(train, 10,
                   SVM::getDefaultGrid(SVM::C),
                   SVM::getDefaultGrid(SVM::GAMMA),
                   SVM::getDefaultGrid(SVM::P),
                   SVM::getDefaultGrid(SVM::NU),
                   SVM::getDefaultGrid(SVM::COEF),
                   SVM::getDefaultGrid(SVM::DEGREE),
                   true);
}

void Trainer::convertToMl(const std::vector<Mat> &train_samples, Mat &trainData) {
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
