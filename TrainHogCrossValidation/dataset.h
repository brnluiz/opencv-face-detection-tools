#ifndef DATASET_H
#define DATASET_H

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

class DataSet {
public:
    DataSet(const string & prefix, const string & filename);

    void loadImages(const string & prefix, const string & filename);

    vector<Mat> & get();

protected:
    vector<Mat> img_lst_;
};

#endif // DATASET_H
