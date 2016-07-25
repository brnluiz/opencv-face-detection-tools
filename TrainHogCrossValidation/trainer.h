#ifndef TRAINER_H
#define TRAINER_H

#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;
using namespace cv::ml;

class Trainer {
public:
    vector<float> getSvmDetector(const Ptr<SVM>& svm);

    Mat computeHog(const Mat& img, const HOGDescriptor& hog, bool do_flip = false);

    vector<Mat> computeHogList(const vector<Mat>& input_lst, HOGDescriptor hog);

    void computeHogList(const vector<Mat>& input_lst, vector<Mat>& output_lst, vector<int>& output_labels, HOGDescriptor hog, int type);

    Ptr<SVM> trainSvm(const vector<Mat>& gradient_lst, const vector<int>& labels, bool automatic = false);

private:
    /*
    * Convert training/testing set to be used by OpenCV Machine Learning algorithms.
    * TrainData is a matrix of size (#samples x max(#cols,#rows) per samples), in 32FC1.
    * Transposition of samples are made if needed.
    */
    void convertToMl(const std::vector<cv::Mat> & train_samples, cv::Mat& trainData);
};

#endif // TRAINER_H
