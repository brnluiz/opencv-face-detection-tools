#include <iostream>

#include "factories/sourcehandlerfactory.h"
#include "factories/objectdetectorfactory.h"

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

int main( int argc, char** argv ) {
    // Parse the arguments
    cv::CommandLineParser parser(
        argc, argv,
        "{help h|| show help message}"
        "{type||detector type (cascade, hogsvm)}"
        "{config||config file with all required parameters (open cv xml/yml format)}"
        "{src||source file/mode (cam, path+image.jpg, path+video.mpeg)}"
        );

    // Print help, if needed
    if (parser.has("help")) {
        parser.printMessage();
        exit(0);
    }

    // Check if the model and type were passed
    string config = parser.get<string>("config");
    string type = parser.get<string>("type");
    string src = parser.get<string>("src");
    if(config.empty() || type.empty() || src.empty()) {
        cerr << "Please specify a detector type, a source and a model" << endl;
        exit(-1);
    }

    // Configure the source handler and load the source
    SourceHandlerFactory sourceFactory;
    SourceHandler* source = sourceFactory.make(src);

    // Open the config file
    FileStorage fs;
    fs.open(config, FileStorage::READ);

    // Init the ObjectDetector
    ObjectDetectorFactory detectorFactory;
    ObjectDetector *detector = detectorFactory.make(type, fs, "FaceDetection Sandbox");

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
