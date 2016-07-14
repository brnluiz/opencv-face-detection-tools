#ifndef NEGDATASET_H
#define NEGDATASET_H

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

#include "dataset.h"

using namespace std;
using namespace cv;

class NegDataSet: public DataSet {
public:
    NegDataSet(const string & prefix, const string & filename, const Size & size);

    vector<Mat> & getAll();

private:
    void sample();

    Size size_;
    vector<Mat> full_img_lst_;
};


#endif // NEGDATASET_H
