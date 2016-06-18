#include <string>

#include "objectdetectors/objectdetector.h"
#include "objectdetectors/cascadedetector.h"
#include "objectdetectors/hogsvmdetector.h"
#include "factories/objectdetectorfactory.h"

using namespace std;

ObjectDetector *ObjectDetectorFactory::make(string type, DetectorParams params, string window) {
    ObjectDetector *detector;
    detector = make(type, params);
    detector->setWindow(window);
    cout << "Window set" << endl;

    return detector;
}

ObjectDetector *ObjectDetectorFactory::make(string type, DetectorParams params) {
    if (type == "hogsvm") {
        return new HogSvmDetector(params);
    }
    else if (type == "cascade") {
        return new CascadeDetector(params);
    }
}
