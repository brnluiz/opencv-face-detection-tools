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
                                 "{type||detector type (cascade, hogsvm)}"
                                 "{src||test list with the paths of the images (must match the ground list)}"
                                 "{ground||ground list with the bounding boxes}"
                                 );
    // Print help, if needed
    if (parser.has("help")) {
        parser.printMessage();
        exit(0);
    }

    // Check if the model, type, src and ground list were passed
    string model = parser.get<string>("model");
    string type = parser.get<string>("type");
    string src = parser.get<string>("src");
    string ground = parser.get<string>("ground");

    if(model.empty() || type.empty() || src.empty() || ground.empty()) {
        cerr << "Please specify a detector type, a model, a source list and a ground list" << endl;
        exit(-1);
    }

    return 0;
}
