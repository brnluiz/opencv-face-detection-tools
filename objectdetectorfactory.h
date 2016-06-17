#ifndef OBJECTDETECTORFACTORY_H
#define OBJECTDETECTORFACTORY_H

#include "objectdetectors/objectdetector.h"
#include "objectdetectors/cascadedetector.h"
#include "objectdetectors/hogsvmdetector.h"

#include <string>

using namespace std;

class ObjectDetectorFactory {
public:
    ObjectDetector* make(string type, string model, float *params) {
        if (type == "hogsvm") {
            return new HogSvmDetector("Window", model, params);
        }
        else if (type == "cascade") {
            return new CascadeDetector("Window", model, params);
        }
    }
};

#endif // OBJECTDETECTORFACTORY_H
