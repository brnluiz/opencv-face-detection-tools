#ifndef SOURCEHANDLER_H
#define SOURCEHANDLER_H

#include <opencv2/core/core.hpp>
#include <string>

using namespace std;
using namespace cv;

class SourceHandler {
public:
    SourceHandler(): pos_(0), finished_(0) {;}

    virtual void open(string source) = 0;
    virtual Mat get() = 0;
    virtual Mat get(int pos_) { return get(); }

    int getPos() { return pos_; }
    bool isFinished() { return finished_; }
protected:
    int pos_;
    bool finished_;
};

#endif // SOURCEHANDLER_H
