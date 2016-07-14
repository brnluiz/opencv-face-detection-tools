#include <iostream>
#include <fstream>

#include <vector>
#include <string>

#include "dataset.h"

using namespace std;

DataSet::DataSet(const string &prefix, const string &filename) {
    loadImages(prefix, filename);
}

void DataSet::loadImages(const string &prefix, const string &filename) {
    string line;
    ifstream file;

    file.open( (prefix+filename).c_str() );
    if( !file.is_open() ) {
        cerr << "Unable to open the list of images from " << filename << " filename." << endl;
        exit( -1 );
    }

    bool end_of_parsing = false;
    while( !end_of_parsing ) {
        getline( file, line );
        if( line.empty() ) {
            // no more file to read
            end_of_parsing = true;
            break;
        }

        // load the image
        Mat img = imread( (prefix+line).c_str() );

        // Invalid image, just skip it.
        if( img.empty() ) continue;

        // Save on the list
        img_lst_.push_back( img.clone() );
    }
}

vector<Mat> &DataSet::get() {
    return img_lst_;
}
