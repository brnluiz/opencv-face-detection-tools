/**
 * @file objectDetection2.cpp
 * @author A. Huaman ( based in the classic facedetect.cpp in samples/c )
 * @brief A simplified version of facedetect.cpp, show how to load a cascade classifier and how to find objects (Face + eyes) in a video stream - Using LBP here
 */

#include "sourcehandlerfactory.h"
#include "objectdetectorfactory.h"
#include "settings.h"
#include <iostream>

using namespace std;
using namespace cv;

void checkParameters(float *params, int size) {
    for(int i = 0; i < size; i++) {
        if (params[i] == 0) {
            cerr << "Parameter " << i << " is missing!" << endl;
            exit(-1);
        }
    }
}

int main( int argc, char** argv )
{
    // Parse the arguments
    cv::CommandLineParser parser(argc, argv,
                                 "{help h|| show help message}"
                                 "{model||model file (path+file)}{type||detector type (cascade, hogsvm)}{src||source mode (cam, image, video)}"
                                 "{hogblock||hog block size}{hogstride||hog block stride}{hogcell||hog cell size}{hogwh||hog window height}{hogww||hog window width}"
                                 );
    string model = parser.get<string>("model");
    string type = parser.get<string>("type");
    string src = parser.get<string>("src");

    // Print help, if needed
    if (parser.has("help")) {
        parser.printMessage();
        exit(0);
    }

    // Check if the model and type were passed
    if(model.empty() || type.empty() || src.empty()) {
        cerr << "Please specify a detector type, a source and a model" << endl;
        exit(-1);
    }

    // Configure the detector based on the type variable
    ObjectDetectorFactory detectorFactory;
    ObjectDetector *detector;
    float params[10] = {0};

    if (type == "cascade") {
        detector = detectorFactory.make("cascade", model, params);
    } else if (type == "hogsvm") {
        int size = 5;
        params[0] = parser.get<float>("hogww");
        params[1] = parser.get<float>("hogwh");
        params[2] = parser.get<float>("hogblock");
        params[3] = parser.get<float>("hogstride");
        params[4] = parser.get<float>("hogcell");
        checkParameters(params, size);

        detector = detectorFactory.make("hogsvm", model, params);
    } else {
        cerr << "Model not available" << endl;
        exit(-1);
    }

    // Configure the source handler
    SourceHandlerFactory sourceFactory;
    SourceHandler *source;

    if (src == "cam") {
        source = sourceFactory.make("cam", "default");
    } else if (src.find("jpg") != std::string::npos) {
        source = sourceFactory.make("image", src);
    } else {
        cerr << "Source handler not available" << endl;
        exit(-1);
    }

    // Start processing
    cout << "Press ESC to stop processing" << endl;
    while(!source->isFinished()) {
        // Get the actual frame
        Mat frame = source->get();

        // Detect the face
        detector->detect(frame);

        // Show the face for the user
        detector->show();

        // Stop if ESC is pressed
        if (waitKey(1) == 27) {
            cout << "Stop processing" << endl;
            break;
        }
    }

    cout << "Press ESC to exit" << endl;
    if (waitKey(0) == 27) {
        cout << "Good bye!" << endl;
    }

    delete detector;
    delete source;

    return 0;
}
