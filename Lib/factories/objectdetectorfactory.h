#ifndef OBJECTDETECTORFACTORY_H
#define OBJECTDETECTORFACTORY_H

#include <string>

#include "objectdetectors/objectdetector.h"
#include "objectdetectors/cascadedetector.h"
#include "objectdetectors/hogsvmdetector.h"

using namespace std;

class ObjectDetectorFactory {
public:
    ObjectDetector *make(string type, DetectorParams params);
    ObjectDetector *make(string type, DetectorParams params, string window);
};

#endif // OBJECTDETECTORFACTORY_H
