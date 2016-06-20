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
typedef vector<BoundingBox> FacesList;

struct ImageInfo {
    Mat img;
    FacesList faces;
    string folder;
    string name;

    string file;
};
typedef vector<ImageInfo> TestImages;

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

TestImages loadImages( const string & path, const string & list, const string & imagesFolder ) {
    string listFile = path + list;
    string testPath = path + imagesFolder;
    ifstream file;

    file.open( (listFile).c_str() );
    if( !file.is_open() ) {
        cerr << "Unable to open the list of images from " << list << " filename." << endl;
        exit( -1 );
    }
    cout << "Loading images from: " << listFile << endl;

    TestImages testImages;
    ImageInfo imgInfo;
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
        Mat img = imread(testPath + src);
        if( img.empty() ) {
            cout << "Non valid image" << endl;
            continue;
        }

        // If the file is new
        if(imgInfo.name != src) {
            // Save previous test image info on the testImages vector
            if(imgInfo.faces.size() != 0) {
                testImages.push_back(imgInfo);
                id = 0;
            }

            // Clean test image info and start with new info
            imgInfo.folder = testPath;
            imgInfo.name = src;
            imgInfo.img  = img.clone();
            imgInfo.file = testPath+src;

            imgInfo.faces.clear();
        }

        // Get the face bounding box
        BoundingBox bbox;
        bbox.id = id++;
        bbox.x1 = x1;
        bbox.y1 = y1;
        bbox.x2 = x2;
        bbox.y2 = y2;

        // Save the new face to the actual image info storage
        imgInfo.faces.push_back(bbox);

    }

    return testImages;
}

float euclideanDist(Point& a, Point& b, int width, int height) {
    Point diff = a - b;

    return ((float)(diff.x*diff.x)/(width*width) +
            (float)(diff.y*diff.y)/(height*height));
}

float intersection(Rect& a, Rect &b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
             (abs(a.y - b.y) * 2 < (a.height + b.height));
}

int main(int argc, char *argv[]) {
    // Parse the arguments
    cv::CommandLineParser parser(argc, argv,
                                 "{help h|| show help message}"
                                 "{model||model file + parameters (open cv xml/yml format)}"
                                 "{type||detector type (cascade, hogsvm)}"
                                 "{src||test list with the paths of the images (must match the ground list)}"
                                 "{output||output folder)}"
                                 "{ground||ground list with the bounding boxes}"
                                 );
    // Print help, if needed
    if (parser.has("help")) {
        parser.printMessage();
        exit(0);
    }

//    // Check if the model, type, src and ground list were passed
//    string model = parser.get<string>("model");
//    string type = parser.get<string>("type");
//    string src = parser.get<string>("src");
//    string ground = parser.get<string>("ground");

//    if(model.empty() || type.empty() || src.empty() || ground.empty()) {
//        cerr << "Please specify a detector type, a model, a source list and a ground list" << endl;
//        exit(-1);
//    }

    TestImages list = loadImages("/home/brunoluiz/qt/FaceDetectionTools/Data/test/", "ground_truth_bboxes.txt", "test_jpg/");

    ObjectDetectorFactory detectorFactory;

    // Open the config file
    FileStorage fs;
    fs.open("/home/brunoluiz/qt/FaceDetectionTools/Data/models/specs_haarcascade.xml", FileStorage::READ);

    // Init detector
    ObjectDetector *detector = detectorFactory.make("cascade", fs);

    for(TestImages::iterator testItem = list.begin(); testItem < list.end(); testItem++) {
        cout << "> File: " << (*testItem).file << endl;

        Mat outputImg = (*testItem).img.clone();
        Mat img = (*testItem).img.clone();

        // Make the detection and test it's accuracy
        Objects detections = detector->detect(img);
        for(FacesList::iterator face = (*testItem).faces.begin(); face < (*testItem).faces.end(); face++) {
            // Get info about the specified face and plot it
            Point tlGround = Point((*face).x1, (*face).y1);
            Point brGround = Point((*face).x2, (*face).y2);
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
                if (overlap >= 0.5) {
                    // Remove the detection from the detections vector (to not test with other
                    // ground boxes)
                    cout << "Positive! Ground truth id: " << (*face).id <<
                            " New size: " << detections.size() <<
                            " Detection: " << (*detection) <<
                            endl;
                    rectangle(outputImg, *detection, Scalar(0, 255, 0), 1);
                    detections.erase(detection);
                    break;
                }

                // Otherwise, false-positive detection
                rectangle(outputImg, *detection, Scalar(0, 0, 255), 1);
                cout << "Negative! Ground truth id: " << (*face).id <<
                        " Box:" << (*detection) <<
                        " Status: " << std::distance(detections.begin(), detection) << " of " << detections.size() <<
                        endl;
                ++detection;

            }
        }

        // Output the image to the results folder
        string output = "/home/brunoluiz/results/" + (*testItem).name;
        imwrite(output, outputImg);

        if (waitKey(1) == 27) {
            cout << "Stop processing" << endl;
            break;
        }
    }

    delete detector;

    return 0;
}
