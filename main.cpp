/**
 * @file objectDetection2.cpp
 * @author A. Huaman ( based in the classic facedetect.cpp in samples/c )
 * @brief A simplified version of facedetect.cpp, show how to load a cascade classifier and how to find objects (Face + eyes) in a video stream - Using LBP here
 */

#include "sourcehandlerfactory.h"
#include "objectdetectors/cascadefacedetector.h"
#include "settings.h"
#include <iostream>

using namespace std;
using namespace cv;

/**
 * @function main
 */

int main( void )
{
    CascadeFaceDetector detector(WINDOW_NAME);
    SourceHandlerFactory factory;

    SourceHandler *handler = factory.make("cam", "default");
//    SourceHandler *handler = factory.make("image", "/home/brunoluiz/qt/DetectorsSandbox/faces/005.jpg");

    cout << "Press ESC to stop processing" << endl;
    while(!handler->isFinished()) {
        // Get the actual frame
        Mat frame = handler->get();

        // Detect the face
        detector.detect(frame);

        // Show the face for the user
        detector.show();

        // Stop if ESC is pressed
        if (waitKey(1) == 27) {
            cout << "Stop processing" << endl;
            break;
        }
    }

    cout << "Press any key to exit" << endl;
    if (waitKey(0)) {
        cout << "Good bye!" << endl;
    }

    delete handler;

    return 0;
}
