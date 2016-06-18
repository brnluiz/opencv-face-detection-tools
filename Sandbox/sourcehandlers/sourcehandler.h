#ifndef SOURCEHANDLER_H
#define SOURCEHANDLER_H

#include <opencv2/core/core.hpp>
#include <string>

using namespace std;
using namespace cv;

class SourceHandler {
public:
    SourceHandler(): finished_(0) {;}
    SourceHandler(string source): finished_(0), source_(source) {
    }
    ~SourceHandler() {}

    virtual void open(string source) = 0;
    virtual Mat get() = 0;

    bool isFinished() { return finished_; }
protected:
    bool finished_;
    string source_;
};

#endif // SOURCEHANDLER_H
