#ifndef CAMHANDLER_H
#define CAMHANDLER_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <string>

#include "sourcehandler.h"

using namespace cv;
using namespace std;

class CamHandler: public SourceHandler {
public:
    CamHandler();
    CamHandler(const string &source);
    ~CamHandler();

    void open(const string &source = "default");

    Mat get();

private:
    VideoCapture stream_;

};

#endif // CAMHANDLER_H
