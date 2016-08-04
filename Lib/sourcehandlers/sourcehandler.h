#ifndef SOURCEHANDLER_H
#define SOURCEHANDLER_H

#include <opencv2/core/core.hpp>
#include <string>

using namespace std;
using namespace cv;

class SourceHandler {
public:
    SourceHandler(): finished_(0) {;}
    SourceHandler(const string &source): finished_(0), source_(source) {
    }
    virtual ~SourceHandler() {}

    virtual void open(const string &source) = 0;
    virtual Mat get() = 0;

    bool isFinished() { return finished_; }

    enum Type {Cam, Image, Video};
    Type getType() { return type_; }

protected:
    bool finished_;
    string source_;
    Type type_;
};

#endif // SOURCEHANDLER_H
