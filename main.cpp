/**
 * @file objectDetection2.cpp
 * @author A. Huaman ( based in the classic facedetect.cpp in samples/c )
 * @brief A simplified version of facedetect.cpp, show how to load a cascade classifier and how to find objects (Face + eyes) in a video stream - Using LBP here
 */

#include "sourcehandlers/imagehandler.h"
#include "objectdetectors/haarfacedetector.h"
#include "settings.h"

using namespace std;
using namespace cv;

/**
 * @function main
 */

int main( void )
{
    HaarFaceDetector faceDetector(WINDOW_NAME);

    ImageHandler handler;
    handler.open("/home/brunoluiz/qt/DetectorsSandbox/faces/005.jpg");

    while(!handler.isFinished()) {
        // Get the actual frame
        Mat frame = handler.get();

        // Detect the face
        faceDetector.detect(frame);

        // Show the face for the user
        faceDetector.show();
    }

    waitKey(0);

    return 0;
}
