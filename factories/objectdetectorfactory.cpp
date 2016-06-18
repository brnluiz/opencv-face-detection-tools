#include <string>

#include "objectdetectors/objectdetector.h"
#include "objectdetectors/cascadedetector.h"
#include "objectdetectors/hogsvmdetector.h"
#include "factories/objectdetectorfactory.h"

using namespace std;

ObjectDetector *ObjectDetectorFactory::make(string type, string model, float *params) {
    if (type == "hogsvm") {
        return new HogSvmDetector("Face Detection", model, params);
    }
    else if (type == "cascade") {
        return new CascadeDetector("Face Detection", model, params);
    }
}
