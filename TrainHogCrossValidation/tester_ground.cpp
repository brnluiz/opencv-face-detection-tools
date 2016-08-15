#include "tester_ground.h"

#include <fstream>
#include <iomanip>

TesterGround::TesterGround(const string &groundimages_path, const string &groundlist_file, const string & output_path,
                           const HOGDescriptor &hog, const Ptr<SVM> &svm):
    hog_(hog), output_path_(output_path), falsepositive_count_(0), groundboxes_count_(0) {
    // Load the detector to the HOG object
    vector<float> detector = SvmUtils::getDetector(svm);
    hog_.setSVMDetector(detector);

    // Load the ground truth data
    load(groundimages_path, groundlist_file);
}

void TesterGround::run() {
    for(TestSet::iterator item = ground_lst_.begin(); item < ground_lst_.end(); item++) {
        float progress = (float)distance(ground_lst_.begin(), item)*100/ground_lst_.size();
        TESTER_LOG << "[" << setprecision(2) << progress << "%] File: " << (*item).file << endl;

        Mat img = (*item).img.clone();
        Mat output_img = img.clone();

        // Make the detection for this test item
//        double t0 = cv::getTickCount();
        vector<Rect> detections;
        hog_.detectMultiScale(img, detections);
//        double t1 = cv::getTickCount();

        // Test the detection accuracy checking each ground truth photo
        int positives = 0;
        for(GroundTruthList::const_iterator ground = (*item).faces.begin(); ground < (*item).faces.end(); ground++) {
            // Get info about the specified face and plot it
            Point topleft_ground     = Point((*ground).x1, (*ground).y1);
            Point bottomright_ground = Point((*ground).x2, (*ground).y2);
            Rect groundtruth         = Rect(topleft_ground, bottomright_ground);

            // Draw the ground truth box at the output image
            rectangle(output_img, groundtruth, Scalar(255, 0, 0), 1);

            // Test the detector accuracy (loop through what was detected and test it with the ground truth)
            for(vector<Rect>::const_iterator detection = detections.begin(); detection < detections.end(); ) {
                // Calculate the overlap of the bounding box with the ground bounding box
                // If higher than 50%, then it is a positive detection
                Rect intersectRoi = groundtruth & (*detection);
                Rect unionRoi = groundtruth | (*detection);
                float intersectArea = intersectRoi.area();
                float unionArea = unionRoi.area();
                float overlap = intersectArea / unionArea;

                // Positive detection only if overlap >= 0.5
                if (overlap >= 0.5) {
                    // Draw the positive detection
                    rectangle(output_img, *detection, Scalar(0, 255, 0), 1);

                    // Remove the detection from the detections vector (avoid it to test it again)
                    detections.erase(detection);
                    positives++;
                    break;
                }

                // Draw the false-positive detection
                rectangle(output_img, *detection, Scalar(0, 0, 255), 1);

                // Go to the next item
                detection++;
            }

        }

        stats_.positives       += positives;
        stats_.false_positives += detections.size();
        stats_.false_negatives += (*item).faces.size() - positives;

        // Output the image to the results folder
        saveOutput(output_img, (*item).name);

        // Hard mining (save the false positives)
        saveFalsePositives(img, detections);
    }

    falsepositive_count_ = 0;
}

void TesterGround::setOutput(const string &path) {
    output_path_ = path;
}

Stats TesterGround::getStats() {
    return stats_;
}

void TesterGround::saveReport(const string &output_path) {
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

void TesterGround::saveOutput(const Mat &img, const string &item_name) {
    string output = output_path_ + item_name;
    imwrite(output, img);
}

void TesterGround::saveFalsePositives(const Mat &img, const vector<Rect> &false_detections) {
    for(vector<Rect>::const_iterator roi = false_detections.begin(); roi < false_detections.end(); roi++) {
        Mat negative = img((*roi));
        string hardmining_output = output_path_ + "false-positives/" + to_string(falsepositive_count_) + ".jpg";

        // Save the image
        imwrite(hardmining_output, negative);

        falsepositive_count_++;
    }
}

void TesterGround::load(const string &files_path, const string &list_file) {

    ifstream file(list_file);
    if(!file) {
        ERROR_LOG << "Unable to open the list of images from " << list_file << " filename." << endl;
        exit( -1 );
    }
    TESTER_LOG << "- Loading images from: " << list_file << endl;

    TestInfo test_info;
    u_int id = 0;
    while(1) {
        // Get the line string
        string line;
        getline( file, line );

        // If the line is empty, then you reached the end of the file
        if( line.empty()) {
            break;
        }

        vector<string> info = split(line, ' ');
        string src = info[0];
        int x1 = atoi(info[1].c_str());
        int y1 = atoi(info[2].c_str());
        int x2 = atoi(info[3].c_str());
        int y2 = atoi(info[4].c_str());

        // Load file and check if it is invalid or empty
        Mat img = imread(files_path + src);
        if( img.empty() ) {
            continue;
        }

        // If the file is new
        if(test_info.name != src) {
            // Save previous test image info on the testImages vector
            if(test_info.faces.size() != 0) {
                ground_lst_.push_back(test_info);
                id = 0;
            }

            // Clean test image info and start with new info
            test_info.folder = files_path;
            test_info.name = src;
            test_info.img  = img.clone();
            test_info.file = files_path+src;

            test_info.faces.clear();
        }

        // Get the face bounding box
        BoundingBox bbox;
        bbox.id = id++;
        bbox.x1 = x1;
        bbox.y1 = y1;
        bbox.x2 = x2;
        bbox.y2 = y2;

        // Save the new face to the actual image info storage
        test_info.faces.push_back(bbox);

        groundboxes_count_++;
    }
}

vector<string> TesterGround::split(string str, char delimiter) {
    vector<string> internal;
    stringstream ss(str); // Turn the string into a stream.
    string tok;

    while(getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}
