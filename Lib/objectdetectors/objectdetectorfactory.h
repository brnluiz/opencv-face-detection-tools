#ifndef OBJECTDETECTORFACTORY_H
#define OBJECTDETECTORFACTORY_H

#include <string>

#include "objectdetector.h"
#include "cascadedetector.h"
#include "hogsvmdetector.h"

using namespace std;

class ObjectDetectorFactory {
public:
    ObjectDetector *make(string type, DetectorParams params);
};

#endif // OBJECTDETECTORFACTORY_H
