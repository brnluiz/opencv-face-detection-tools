#ifndef LOG_H
#define LOG_H

#include <iostream>

using std::cout;
using std::cerr;

#define TRAINER_LOG cout << "[trainer] "
#define TRAINERHOG_LOG cout << "[trainer][hog] "
#define TRAINERSVM_LOG cout << "[trainer][svm] "
#define STAT_LOG cout << "[stat] "
#define DATA_LOG cout << "[data] "
#define TESTER_LOG cout << "[tester] "
#define MAIN_LOG cout << "[main] "

#define ERROR_LOG cerr << "[error] "


#endif // LOG_H
