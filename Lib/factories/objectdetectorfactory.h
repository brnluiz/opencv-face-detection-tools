#ifndef OBJECTDETECTORFACTORY_H
#define OBJECTDETECTORFACTORY_H

#include <string>

#include "objectdetectors/objectdetector.h"
#include "objectdetectors/cascadedetector.h"
#include "objectdetectors/hogsvmdetector.h"

using namespace std;

class ObjectDetectorFactory {
public:
    ObjectDetector* make(string type, string model, float *params);
};

#endif // OBJECTDETECTORFACTORY_H
