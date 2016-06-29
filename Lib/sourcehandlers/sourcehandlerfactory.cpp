#include <string>
#include <stdexcept>

#include "sourcehandlerfactory.h"
#include "camhandler.h"
#include "imagehandler.h"
#include "sourcehandler.h"

using namespace std;

SourceHandler *SourceHandlerFactory::make(string source) {
    if (source == "cam") {
        return new CamHandler("default");
    }
    else if (imread(source).data != NULL) {
        return new ImageHandler(source);
    }
    else {
        throw invalid_argument("Invalid source type");
    }
}
