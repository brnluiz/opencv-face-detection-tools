#ifndef SOURCEHANDLERFACTORY_H
#define SOURCEHANDLERFACTORY_H

#include <string>

#include "sourcehandlers/camhandler.h"
#include "sourcehandlers/imagehandler.h"
#include "sourcehandlers/sourcehandler.h"


using namespace std;

class SourceHandlerFactory {
public:
    SourceHandler* make(string name, string source);
};

#endif // SOURCEHANDLERFACTORY_H
