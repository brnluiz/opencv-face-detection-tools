#include <iostream>

#include "factories/sourcehandlerfactory.h"
#include "factories/objectdetectorfactory.h"

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {
    // Parse the arguments
    cv::CommandLineParser parser(argc, argv,
                                 "{help h|| show help message}"
                                 "{model||model file + parameteres (open cv xml/yml format)}"
                                 "{ground||ground list with the bounding boxes}"
                                 "{test||test list with the paths of the images (must match the ground list)}"
                                 );
    // Print help, if needed
    if (parser.has("help")) {
        parser.printMessage();
        exit(0);
    }

    return 0;
}
