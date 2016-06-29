#ifndef SOURCEHANDLERFACTORY_H
#define SOURCEHANDLERFACTORY_H

#include <string>

#include "camhandler.h"
#include "imagehandler.h"
#include "sourcehandler.h"

using namespace std;

class SourceHandlerFactory {
public:
    SourceHandler* make(string source);
};

#endif // SOURCEHANDLERFACTORY_H
