#include <string>

#include "sourcehandlers/camhandler.h"
#include "sourcehandlers/imagehandler.h"
#include "sourcehandlers/sourcehandler.h"
#include "factories/sourcehandlerfactory.h"

using namespace std;

SourceHandler *SourceHandlerFactory::make(string name, string source) {
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
