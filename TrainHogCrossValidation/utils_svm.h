#ifndef SVMUTILS_H
#define SVMUTILS_H

#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;
using namespace cv::ml;

namespace SvmUtils {
    void convertData(const std::vector<cv::Mat> & train_samples, cv::Mat& trainData);
    Ptr<SVM> train(const vector<Mat>& gradient_lst, const vector<int>& labels, bool automatic = false);
    vector<float> getDetector(const Ptr<SVM>& svm);
}


#endif // SVMUTILS_H
