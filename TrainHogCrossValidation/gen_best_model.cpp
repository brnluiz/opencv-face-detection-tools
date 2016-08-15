#include <opencv2/opencv.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "dataset.h"
#include "trainer_hog.h"
#include "trainer_svm.h"
#include "tester_ground.h"

using namespace std;

template<class T>
void print_vector(vector<T> & v) {
    cout << "[ ";
    for(uint i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << "]" << endl;
}

#define PROJECT_PATH (string)"/Users/brunoluiz/Git/opencv-face-detection-tools/"

int main() {

    // Use the following block of code if you need to test anything without doing the whole training
//    HOGDescriptor hog;
//    hog.blockSize   = Size(16,16);
//    hog.blockStride = Size(16,16);
//    hog.cellSize    = Size(16,16);
//    hog.winSize     = Size(32,32);
//    HogBest best_hog;
//    best_hog.descriptor = hog;
//    Ptr<SVM> svm = SVM::load<SVM>("best-svm.xml");

    // HOG Parameters (which will be combined and tested)
    vector<vector<int>> params = {
        {8,16},   // Block size
        {4,8,16}, // Cell size
        {4,8,16},   // Block stride
        {32},     // Win size
    };

    MAIN_LOG << "Loading images sets..." << endl;
    DataSet pos_set(PROJECT_PATH+"Data/train/", "caltech.lst");
    DataSet neg_set(PROJECT_PATH+"Data/train/", "neg.lst", Size(32,32));
    vector<Mat> pos = pos_set.get();
    vector<Mat> neg = neg_set.get();
    MAIN_LOG << "Finished loading!" << endl;

    MAIN_LOG << "Starting process..." << endl;

    // --------------------------------------------------------------------------------------------

    // This is not a good way to program: if you continue like this, you are not going to receive xmas gifts
    system((PROJECT_PATH+"clean.sh").c_str());

    MAIN_LOG << "- First step: choosing the best HOG parameters" << endl;
    TrainerHog hog_train(pos, neg, 10, params);
    hog_train.run();
    hog_train.saveReport(PROJECT_PATH+"tmp/best-hog.csv");

    BestHog best_hog = hog_train.getBest();
    cout << best_hog;

    // --------------------------------------------------------------------------------------------

    MAIN_LOG << "- Second step: choosing best SVM parameters" << endl;
    TrainerSvm svm_train(pos, neg, 10, best_hog.descriptor);
    svm_train.run();
    svm_train.saveReport(PROJECT_PATH+"tmp/best-svm.csv");
    BestSvm best_svm = svm_train.getBest();

    Ptr<SVM> svm = best_svm.svm;
    svm->save(PROJECT_PATH+"tmp/best-svm.xml");

    // --------------------------------------------------------------------------------------------

    MAIN_LOG << " - Third step: testing against the test set #1 to get false positives" << endl;
    TesterGround tester1(PROJECT_PATH+"Data/test/set1/",
                        PROJECT_PATH+"Data/test/ground_truth_set1.csv",
                        PROJECT_PATH+"tmp/set1/",
                        best_hog.descriptor, svm);
    tester1.run();
    tester1.saveReport(PROJECT_PATH+"tmp/set1/report_set1.csv");

    // --------------------------------------------------------------------------------------------

    // This is not a good way to program: if you continue like this, you are not going to receive xmas gifts
    system((PROJECT_PATH+"gen_set2.sh").c_str());

    MAIN_LOG << " - Forth step: retrain using the new false positives samples (hard negative training)" << endl;
    // Loads the negative samples + hard negative mining samples
    DataSet hard_set(PROJECT_PATH+"tmp/set2/", "set2.lst", Size(32,32));
    vector<Mat> hard = hard_set.get();

    // Train the SVM again
    TrainerSvm svm_train_hard(pos, hard, 10, best_hog.descriptor);
    svm_train_hard.run();
    svm_train.saveReport(PROJECT_PATH+"tmp/best-svm-hard.csv");
    best_svm = svm_train_hard.getBest();

    Ptr<SVM> svm_hard = best_svm.svm;
    svm_hard->save(PROJECT_PATH+"best-svm-hard.xml");

    // --------------------------------------------------------------------------------------------

    MAIN_LOG << " - Final step: testing against the test set #2" << endl;
    TesterGround tester2(PROJECT_PATH+"Data/test/set2/",
                        PROJECT_PATH+"Data/test/ground_truth_set2.csv",
                        PROJECT_PATH+"tmp/set2/",
                        best_hog.descriptor, svm_hard);
    tester2.run();
    tester2.saveReport(PROJECT_PATH+"tmp/set2/report_set2.csv");

    MAIN_LOG << "~ Finished processing" << endl;

    Stats stats = tester2.getStats();
    cout << stats;

    return 0;
}
