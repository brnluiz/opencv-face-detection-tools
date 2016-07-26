#ifndef TESTERGROUND_H
#define TESTERGROUND_H

#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <opencv2/opencv.hpp>

#include "utils_svm.h"
#include "stats.h"
#include "log.h"

using namespace std;
using namespace cv;

struct BoundingBox {
    int id;
    int x1;
    int y1;
    int x2;
    int y2;
};
typedef vector<BoundingBox> GroundTruthList;

struct TestInfo {
    Mat img;
    GroundTruthList faces;
    string folder;
    string name;

    string file;
};
typedef vector<TestInfo> TestSet;

class TesterGround {
public:
    TesterGround(const string & groundimages_path, const string & groundlist_file, const string &output_path,
                 const HOGDescriptor &hog, const Ptr<ml::SVM> &svm);

    void run();

    void setOutput(const string & path);

    Stats getStats();

    void saveReport(const string & output_path) {
        Stats stat = getStats();

        // Open a file for the report
        ofstream report;
        report.open(output_path);
        if(!report.is_open()) {
            exit(-1);
        }

        // Save the report
        report << stat;
        report.close();
    }

private:
    void saveOutput(const Mat& img, const string& item_name);

    void saveFalsePositives(const Mat & img, const vector<Rect>& false_detections);

    void load(const string & files_path, const string & list_file);

    vector<string> split(string str, char delimiter);

    Stats stats_;

    HOGDescriptor hog_;
    TestSet ground_lst_;
    string output_path_;

    int falsepositive_count_;
    int groundboxes_count_;
};

#endif // TESTERGROUND_H
