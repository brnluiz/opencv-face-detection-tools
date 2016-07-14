#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <string>

#include "kfold.h"

using namespace std;

//template<class In>
//Kfold<In>::Kfold(int k, In beg_, In end_): beg_(beg_), end_(end_), k_(k) {
//    if (k_ <= 0) {
//        throw runtime_error("The supplied value of K is ="+to_string(k_)+
//                            ". One cannot create "+to_string(k)+"no of folds");
//    }

//    // Create the vector of integers
//    int foldNo = 0;
//    for (In i = beg_; i != end_; i++) {
//        whichFoldToGo.push_back(++foldNo);

//        if (foldNo == k_) {
//            foldNo = 0;
//        }
//    }

//    if (!k_) {
//        throw runtime_error("With this value of k (="+to_string(k)+")Equal division of the data is not possible");
//    }

//    //        random_shuffle(whichFoldToGo.begin(), whichFoldToGo.end());
//}
