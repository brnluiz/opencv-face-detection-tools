#ifndef SOURCEHANDLERFACTORY_H
#define SOURCEHANDLERFACTORY_H

#include "sourcehandlers/camhandler.h"
#include "sourcehandlers/imagehandler.h"
#include "sourcehandlers/sourcehandler.h"

#include <string>

using namespace std;

class SourceHandlerFactory {
public:
    SourceHandler* make(string name, string source) {
        if (name == "cam") {
            return new CamHandler(source);
        }
        else if (name == "image") {
            return new ImageHandler(source);
        }
        else {
            return new CamHandler("default");
        }
    }
};

#endif // SOURCEHANDLERFACTORY_H
