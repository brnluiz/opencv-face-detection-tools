#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

#include "factories/sourcehandlerfactory.h"
#include "factories/objectdetectorfactory.h"

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

struct AccuracyInfo {
    u_int positives;
    u_int falsePositives;
    u_int missed;
};

struct TestInfo {
    Mat img;
    GroundTruthList faces;
    string folder;
    string name;

    string file;

    AccuracyInfo accuracy;
};
typedef vector<TestInfo> TestSet;

// You could also take an existing vector as a parameter.
vector<string> split(string str, char delimiter) {
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;

  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }

  return internal;
}

TestSet loadImages( const string & path, const string & list) {
    string listFile = path + list;
    ifstream file;

    file.open( (listFile).c_str() );
    if( !file.is_open() ) {
        cerr << "Unable to open the list of images from " << list << " filename." << endl;
        exit( -1 );
    }
    cout << "Loading images from: " << listFile << endl;

    TestSet testSet;
    TestInfo testInfo;
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
        Mat img = imread(path + src);
        if( img.empty() ) {
            cout << "Non valid image" << endl;
            continue;
        }

        // If the file is new
        if(testInfo.name != src) {
            // Save previous test image info on the testImages vector
            if(testInfo.faces.size() != 0) {
                testSet.push_back(testInfo);
                id = 0;
            }

            // Clean test image info and start with new info
            testInfo.folder = path;
            testInfo.name = src;
            testInfo.img  = img.clone();
            testInfo.file = path+src;
            testInfo.accuracy = {0, 0, 0};

            testInfo.faces.clear();
        }

        // Get the face bounding box
        BoundingBox bbox;
        bbox.id = id++;
        bbox.x1 = x1;
        bbox.y1 = y1;
        bbox.x2 = x2;
        bbox.y2 = y2;

        // Save the new face to the actual image info storage
        testInfo.faces.push_back(bbox);

    }

    return testSet;
}

int main(int argc, char *argv[]) {
    // Parse the arguments
    cv::CommandLineParser parser(argc, argv,
                                 "{help h|| show help message}"
                                 "{type||detector type (cascade, hogsvm)}"
                                 "{model||model file + parameters (open cv xml/yml format)}"
                                 "{path||test list with the paths of the images (must match the ground list)}"
                                 "{output||output folder)}"
                                 "{ground||ground list with the bounding boxes}"
                                 );
    // Print help, if needed
    if (parser.has("help")) {
        parser.printMessage();
        exit(0);
    }


//    // Check if the model, type, src and ground list were passed
    string type = parser.get<string>("type");
    string model = parser.get<string>("model");
    string inputPath = parser.get<string>("path");
    string ground = parser.get<string>("ground");
    string outputPath = parser.get<string>("output");

    if(model.empty() || type.empty() || inputPath.empty() || ground.empty() || outputPath.empty()) {
        cerr << "Please specify a detector type, a model, a source list and a ground list" << endl;
        exit(-1);
    }

    // Load all images and build the test list
    TestSet list = loadImages(inputPath, ground);

    // Prepare the CSV files
    ofstream report;
    report.open(outputPath + "individual.csv");
    if(!report.is_open()) {
        exit(-1);
    }
    report << "Test subject" << ","
           << "Total positives" << ","
           << "Total false positives" << ","
           << "Ground truth objects" << ","
           << "Total missed detections" << ","
           << "% positives" << ","
           << "% false positives" << ","
           << endl;

    // Open the config file
    FileStorage fs;
    fs.open(model, FileStorage::READ);

    // Init detector
    ObjectDetectorFactory detectorFactory;
    ObjectDetector *detector = detectorFactory.make(type, fs);
    AccuracyInfo accuracy = {0,0,0};

    for(TestSet::iterator testItem = list.begin(); testItem < list.end(); testItem++) {
        float progress = (float)distance(list.begin(), testItem)*100/list.size();
        cout << "[" << setprecision(2) << progress <<
                "%] File: " << (*testItem).file << endl;

        Mat outputImg = (*testItem).img.clone();
        Mat img = (*testItem).img.clone();

        // Make the detection and test it's accuracy
        Objects detections = detector->detect(img);
        for(GroundTruthList::iterator ground = (*testItem).faces.begin(); ground < (*testItem).faces.end(); ground++) {
            // Get info about the specified face and plot it
            Point tlGround = Point((*ground).x1, (*ground).y1);
            Point brGround = Point((*ground).x2, (*ground).y2);
            Rect groundTruth = Rect(tlGround, brGround);
            rectangle(outputImg, groundTruth, Scalar(255, 0, 0), 1);

            // Test the detector accuracy (loop through what was detected and check with the ground truth)
            for(Objects::iterator detection = detections.begin(); detection < detections.end(); ) {

                // Calculate the overlap of the bounding box with the ground bounding box
                // If higher than 50%, then it is a positive detection
                Rect intersectRoi = groundTruth & (*detection);
                Rect unionRoi = groundTruth | (*detection);
                float intersectArea = intersectRoi.area();
                float unionArea = unionRoi.area();
                float overlap = intersectArea / unionArea;

                // Positive detection
                if (overlap >= 0.4) {
                    // Remove the detection from the detections vector (to not test with other
                    // ground boxes)
                    rectangle(outputImg, *detection, Scalar(0, 255, 0), 1);
                    detections.erase(detection);
                    (*testItem).accuracy.positives++;
                    accuracy.positives++;
                    break;
                }

                // Otherwise, false-positive detection
                rectangle(outputImg, *detection, Scalar(0, 0, 255), 1);
                ++detection;

            }

        }
        // Get some data for the accuracy
        (*testItem).accuracy.falsePositives = detections.size();
        (*testItem).accuracy.missed += (*testItem).faces.size() - (*testItem).accuracy.positives;
        accuracy.falsePositives += (*testItem).accuracy.falsePositives;
        accuracy.missed += (*testItem).accuracy.missed;

        // Output the image to the results folder
        string output = outputPath + (*testItem).name;
        imwrite(output, outputImg);

        // Hard mining (save the negatives)
        for(Objects::iterator roi = detections.begin(); roi < detections.end(); roi++) {
            Mat negative = img((*roi));
            string index = to_string(distance(detections.begin(), roi));
            string output = outputPath + "hardmining/"  + (*testItem).name + index + ".jpg";
            cout << output << endl;
            imwrite(output, negative);
        }

        // Write to the report
        int totalItems = (*testItem).faces.size();
        report << (*testItem).name << ","
               << (*testItem).accuracy.positives << ","
               << (*testItem).accuracy.falsePositives << ","
               << totalItems << ","
               << totalItems - (*testItem).accuracy.positives << ","
               << setprecision(4) << (float)(*testItem).accuracy.positives / totalItems << ","
               << setprecision(4) << (float)(*testItem).accuracy.falsePositives / totalItems
               << endl;
    }
    report.close();

    // Prepare the CSV files
    ofstream accReport;
    accReport.open(outputPath + "accuracy.csv");
    if(!accReport.is_open()) {
        exit(-1);
    }
    accReport << "Model file" << ","
           << "Positives" << ","
           << "False positives" << ","
           << "Missed"
           << "Accuracy"
           << endl;

    accReport << model << ","
             << (float)accuracy.positives << ","
             << (float)accuracy.falsePositives << ","
             << (float)accuracy.missed << ","
             << (float)accuracy.positives / (accuracy.missed + accuracy.positives)
             << endl;

    delete detector;

    return 0;
}

// ./faces_modeltester --model="/home/brunoluiz/qt/FaceDetectionTools/Data/models/specs_model_2016_06_17_08_20_39_faces94.lst.xml" --type=hogsvm --path="/home/brunoluiz/qt/FaceDetectionTools/Data/test/test_jpg/" --ground="ground_truth_bboxes.txt" --output="/home/brunoluiz/results/"
