#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/highgui/highgui.hpp>

#include "factories/sourcehandlerfactory.h"
#include "factories/objectdetectorfactory.h"

using namespace std;
using namespace cv;

struct BoundingBox {
    int x;
    int y;
    int w;
    int h;
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
        int x = atoi(info[1].c_str());
        int y = atoi(info[2].c_str());
        int w = atoi(info[3].c_str());
        int h = atoi(info[4].c_str());

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
        bbox.x = x;
        bbox.y = y;
        bbox.w = w;
        bbox.h = h;

        // Save the new face to the actual image info storage
        imgInfo.faces.push_back(bbox);

    }

    return testImages;
}

int main(int argc, char *argv[]) {
    // Parse the arguments
    cv::CommandLineParser parser(argc, argv,
                                 "{help h|| show help message}"
                                 "{model||model file + parameters (open cv xml/yml format)}"
                                 "{type||detector type (cascade, hogsvm)}"
                                 "{src||test list with the paths of the images (must match the ground list)}"
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

    for(u_int i = 0; i < list.size(); i++){
        cout << "> File: " << list[i].file << endl;
        for(u_int j = 0; j < list[i].faces.size(); j++) {
            cout << ">> Face #" << j << ": " <<
                    list[i].faces[j].x << ", " <<
                    list[i].faces[j].y << ", " <<
                    list[i].faces[j].w << ", " <<
                    list[i].faces[j].h <<
                    endl;
        }
        imshow("Image", list[i].img);

        if (waitKey(1) == 27) {
            cout << "Stop processing" << endl;
            break;
        }
    }

    return 0;
}
