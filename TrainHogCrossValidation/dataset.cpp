#include <iostream>
#include <fstream>

#include <vector>
#include <string>

#include "dataset.h"
#include "log.h"

using namespace std;

DataSet::DataSet(const string &prefix, const string &filename) {
    loadImages(prefix, filename);
}

DataSet::DataSet(const string &prefix, const string &filename, const Size& size) {
    loadImages(prefix, filename, size);
}

void DataSet::loadImages(const string &prefix, const string &filename) {
    string line;
    ifstream file;

    file.open( (prefix+filename).c_str() );
    if( !file.is_open() ) {
        DATA_LOG << "Unable to open the list of images from " << filename << " filename." << endl;
        exit( -1 );
    }

    img_lst_.clear();

    bool end_of_parsing = false;
    while( !end_of_parsing ) {
        getline( file, line );
        if( line.empty() ) {
            // no more file to read
            end_of_parsing = true;
            break;
        }

        // Load the image
        Mat img = imread( (prefix+line).c_str() );

        // Invalid image, just skip it.
        if( img.empty() ) continue;

        // Save on the list
        img_lst_.push_back( img.clone() );
    }
}

void DataSet::loadImages(const string &prefix, const string &filename, const Size &sample_size) {
    Rect box;
    box.width  = sample_size.width;
    box.height = sample_size.height;
    const int size_x = sample_size.width;
    const int size_y = sample_size.height;

    srand( (unsigned int)time( NULL ) );

    // Load the images
    loadImages(prefix, filename);

    // Copy the actual list and clear the actual list
    const vector<Mat> original_lst = img_lst_;
    img_lst_.clear();

    // Process the original list and get small cuts from it
    for(vector<Mat>::const_iterator img = original_lst.begin(); img < original_lst.end(); img++ ) {
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

vector<Mat> DataSet::get() {
    return img_lst_;
}
