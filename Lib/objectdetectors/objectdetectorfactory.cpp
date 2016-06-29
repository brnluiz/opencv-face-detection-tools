#include <string>
#include <stdexcept>

#include "objectdetectorfactory.h"
#include "objectdetector.h"
#include "cascadedetector.h"
#include "hogsvmdetector.h"

using namespace std;

ObjectDetector *ObjectDetectorFactory::make(string type, DetectorParams params) {
    if (type == "hogsvm") {
        return new HogSvmDetector(params);
    }
    else if (type == "cascade") {
        return new CascadeDetector(params);
    } else {
        throw invalid_argument("Invalid detector type");
    }
}
