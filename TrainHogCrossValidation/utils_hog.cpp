#include "utils_hog.h"

namespace HogUtils {

    Mat compute(const Mat& img, const HOGDescriptor& hog, bool do_flip) {
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

        return Mat(descriptors);
    }

    vector<Mat> computeList(const vector<Mat> &input_lst, HOGDescriptor hog) {
        // Output variable
        vector<Mat> gradient_lst;

        for(vector<Mat>::const_iterator img = input_lst.begin(); img < input_lst.end(); img++) {
            vector<float> descriptors;

            // Compute HoG (normal image)
            descriptors = HogUtils::compute(*img, hog).clone();
            gradient_lst.push_back(Mat(descriptors).clone());
            descriptors.clear();

            // Compute HoG (flipped image)
            descriptors = HogUtils::compute(*img, hog, true).clone();
            gradient_lst.push_back(Mat(descriptors).clone());
        }

        return gradient_lst;
    }

    void computeList(const vector<Mat> &input_lst, vector<Mat> &output_lst, vector<int> &output_labels, HOGDescriptor hog, int type) {
        for(vector<Mat>::const_iterator img = input_lst.begin(); img < input_lst.end(); img++) {
            vector<float> descriptors;

            // Compute HoG (normal image)
            descriptors = HogUtils::compute(*img, hog).clone();
            output_lst.push_back(Mat(descriptors).clone());
            output_labels.push_back(type);

            // Compute HoG (flipped image)
            descriptors = HogUtils::compute(*img, hog, true).clone();
            output_lst.push_back(Mat(descriptors).clone());
            output_labels.push_back(type);
        }
    }

}
