#include <opencv2/opencv.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "dataset.h"
#include "trainer_hog.h"
#include "trainer_svm.h"

using namespace std;

template<class T>
void print_vector(vector<T> & v) {
    cout << "[ ";
    for(uint i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << "]" << endl;
}

int main() {
    // HOG Parameters (which will be combined and tested)
    vector<vector<int>> params = {
        {16},    // Block size
        {16},    // Cell size
        {16}, // Block stride
        {32},          // Win size
    };

    MAIN_LOG << "Loading images sets..." << endl;
    DataSet pos_set("/home/brunoluiz/qt/FaceDetectionTools/builds/release/linux/x86/TrainHog/", "caltech.lst");
    DataSet neg_set("/home/brunoluiz/qt/FaceDetectionTools/builds/release/linux/x86/TrainHog/", "neg.lst", Size(32,32));
    vector<Mat> pos = pos_set.get();
    vector<Mat> neg = neg_set.get();
    MAIN_LOG << "Finished loading!" << endl;

    MAIN_LOG << "Starting process..." << endl;

    MAIN_LOG << "- First step: choosing the best HOG parameters" << endl;
    TrainerHog hog_train(pos, neg, 10, params);
    hog_train.run();
    HogBest hog_best = hog_train.getBest();
    hog_best.print();

    MAIN_LOG << "- Second step: choosing best SVM parameters" << endl;
    TrainerSvm svm_train(pos, neg, 10, hog_best.descriptor);
    svm_train.run();
    Ptr<SVM> svm = svm_train.getBest();
    svm->save("best-svm.xml");

    MAIN_LOG << " - Third step: testing against the test set #1 to get false positives" << endl;


    MAIN_LOG << " - Forth step: retrain using the new false positives samples (hard negative training)" << endl;

    MAIN_LOG << " - Final step: testing against the test set #2" << endl;

    MAIN_LOG << "~ Finished processing" << endl;

//    Testing only the train_auto
//    MAIN_LOG << "- Second step: choosing best SVM parameters" << endl;
//    vector<int> test_params = {16,16,16,32};
//    SvmTrain svm_train(pos, neg, 10, test_params);
//    svm_train.run();
//    Ptr<SVM> svm = svm_train.getBest();
//    svm->save("best-svm.xml");

    return 0;
}
