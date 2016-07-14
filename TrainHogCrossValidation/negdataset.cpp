#include "negdataset.h"

#include <vector>
#include <string>

using namespace std;

NegDataSet::NegDataSet(const string &prefix, const string &filename, const Size &size):
    DataSet(prefix, filename), size_(size) {
    full_img_lst_ = img_lst_;
    img_lst_.clear();
    sample();
}

vector<Mat> &NegDataSet::getAll() {
    return full_img_lst_;
}

void NegDataSet::sample() {
    Rect box;
    box.width = size_.width;
    box.height = size_.height;

    const int size_x = box.width;
    const int size_y = box.height;

    srand( (unsigned int)time( NULL ) );

    vector<Mat>::const_iterator img = full_img_lst_.begin();
    vector<Mat>::const_iterator end = full_img_lst_.end();
    for( ; img != end ; ++img )
    {
        for (int i = 0; i < img->cols && (img->cols - i) > size_x; i += size_x) {
            for (int j = 0; j < img->rows && (img->rows - j) > size_y; j += size_y) {
                box.x = i;
                box.y = j;
                Mat roi = (*img)(box);
                img_lst_.push_back( roi.clone() );
            }
        }
    }
}
