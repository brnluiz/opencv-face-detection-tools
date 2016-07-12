#include <iostream>
#include <memory>
#include <stdexcept>

#include "gui/viewer.h"
#include "measurementtools/measuredistancetool.h"
#include "sourcehandlers/sourcehandlerfactory.h"
#include "objectdetectors/objectdetectorfactory.h"

//#define KNOWN_DISTANCE    100.0
//#define KNOWN_WIDTH       17.0
//#define KNOWN_WIDTH_PIXEL 2080
#define KNOWN_DISTANCE    50
#define KNOWN_WIDTH       17.0
#define KNOWN_WIDTH_PIXEL 190
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
        SourceHandlerFactory source_factory;
        shared_ptr<SourceHandler> source(source_factory.make(src));

        // Open the config file
        FileStorage fs;
        fs.open(config, FileStorage::READ);

        // Init the ObjectDetector
        ObjectDetectorFactory detector_factory;
        shared_ptr<ObjectDetector> detector(detector_factory.make(type, fs));

        // Calculate the focal length depending on the parameters
        MeasureDistanceTool distance(KNOWN_DISTANCE, KNOWN_WIDTH, KNOWN_WIDTH_PIXEL);

        // Start a Window
        Viewer window("FaceDectection Sandbox");

        // Start processing
        cout << "Press ESC to stop processing task" << endl;
        while(!source->isFinished()) {
            // Stop processing if ESC is pressed
            if (waitKey(1) == 27) {
                cout << "Stopping processing task..." << endl;
                break;
            }

            // Get the actual frame
            Mat frame = source->get();

            // Save it to the viewer window (it will be used to draw the results on it)
            window.setFrame(frame);

            // Detect the face
            Objects detections = detector->detect(frame);

            // If nothing was detected, update the window frame and then continue the loop
            if(detections.size() == 0) {
                window.show();
                continue;
            }

            // Draw the detections on the results window
            window.draw(detections);

            // Detect the biggest available ROI
            Rect face = detections[0];
            if (detections.size() > 1) {
                for(Objects::iterator it = detections.begin(); it < detections.end(); it++) {
                    if ((*it).size().area() > face.size().area()) {
                        face = (*it);
                    }
                }
            }

            // Draw the selected ROI on a blue square (to differentiate from the other detections)
            vector<Rect> real;
            real.push_back(face);
            window.draw(real, Scalar(255, 0, 0));

            // Output information about the ROI
            cout << face.size().width << "x" << face.size().height << endl;

            // Output the distance of the detected face to the source
            cout << "Distance from the source: " << distance.get(face) << "cm" << endl;
            cout << "###" << endl;

            // Show the results window
            window.show();
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
