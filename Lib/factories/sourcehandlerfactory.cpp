#include <string>

#include "sourcehandlers/camhandler.h"
#include "sourcehandlers/imagehandler.h"
#include "sourcehandlers/sourcehandler.h"
#include "factories/sourcehandlerfactory.h"

using namespace std;

SourceHandler *SourceHandlerFactory::make(string source) {
    if (source == "cam") {
        return new CamHandler("default");
    }
    else if (imread(source).data != NULL) {
        return new ImageHandler(source);
    }
    else {
        return new CamHandler("default");
    }
}
