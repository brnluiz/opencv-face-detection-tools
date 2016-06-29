#include <string>
#include <stdexcept>

#include "objectdetectors/objectdetector.h"
#include "objectdetectors/cascadedetector.h"
#include "objectdetectors/hogsvmdetector.h"
#include "factories/objectdetectorfactory.h"

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
