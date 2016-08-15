#ifndef HOGUTILS_H
#define HOGUTILS_H

#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

namespace HogUtils {
    Mat compute(const Mat& img, const HOGDescriptor& hog, bool do_flip = false, string output = "");
    vector<Mat> computeList(const vector<Mat> &input_lst, HOGDescriptor hog);
    void computeList(const vector<Mat> &input_lst, vector<Mat> &output_lst, vector<int> &output_labels, HOGDescriptor hog, int type);
}


#endif // HOGUTILS_H
