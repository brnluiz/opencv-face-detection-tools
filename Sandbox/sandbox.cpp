#include <iostream>
#include <memory>
#include <stdexcept>

#include "measurementtools/measuredistancetool.h"
#include "factories/sourcehandlerfactory.h"
#include "factories/objectdetectorfactory.h"

#define KNOWN_DISTANCE    57.0
#define KNOWN_WIDTH       17.0
#define KNOWN_WIDTH_PIXEL 176
#define INTERVAL_COUNTER  10

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
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

    try {
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
        shared_ptr<SourceHandler> source(sourceFactory.make(src));

        // Open the config file
        FileStorage fs;
        fs.open(config, FileStorage::READ);

        // Init the ObjectDetector
        ObjectDetectorFactory detectorFactory;
        shared_ptr<ObjectDetector> detector(detectorFactory.make(type, fs, "FaceDetection Sandbox"));

        // Calculate the focal length depending on the parameters
        MeasureDistanceTool distance(KNOWN_DISTANCE, KNOWN_WIDTH, KNOWN_WIDTH_PIXEL);
        int counter = 0;

        // Start processing
        cout << "Press ESC to stop processing task" << endl;
        while(!source->isFinished()) {
            // Get the actual frame
            Mat frame = source->get();

            // Detect the face
            Objects detections = detector->detect(frame);

            // Show the face for the user
            detector->show();

            // Measure the distance of the detected face to the source
            if(detections.size() == 1 && counter == 0) {
                Rect face = detections[0];
                cout << face.size().width << "x" << face.size().height << endl;
                cout << "Distance: " << distance.get(face) << endl;
                cout << "###" << endl;
            }

            // Counter for periodic tasks
            counter++;
            if (counter > INTERVAL_COUNTER) {
                counter = 0;
            }

            // Stop processing if ESC is pressed
            if (waitKey(1) == 27) {
                cout << "Stopping processing task..." << endl;
                break;
            }
        }

        cout << "Processing task finished" << endl;

        // If the source type is not a camera, then wait after the end of the processing (in
        // videos or images sources, the above while will not be infinite)
        if (source->getType() != SourceHandler::Cam) {
            cout << "Press ESC to leave" << endl;
            if (waitKey(0) == 27) {
                cout << "Leaving..." << endl;
            }
        }

        waitKey(1);
    }
    catch (cv::Exception &e) {
        cerr << "[opencv][error] " << e.what() << endl;
    }
    catch (exception &e) {
        cerr << "[error] " << e.what() << endl;
    }
    catch (...) {
        cerr << "[error] An exception was thrown!" << endl;
    }

    return 0;
}
