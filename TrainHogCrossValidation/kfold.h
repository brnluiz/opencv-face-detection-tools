#ifndef KFOLD_H
#define KFOLD_H

#include <string>
#include <vector>

using namespace std;

template<class In>
class Kfold {

public:
    Kfold(int k, In beg_, In end_): beg_(beg_), end_(end_), k_(k) {
        if (k_ <= 0) {
            throw runtime_error("The supplied value of K is ="+to_string(k_)+
                                ". One cannot create "+to_string(k)+"no of folds");
        }

        // Create the vector of integers
        int foldNo = 0;
        for (In i = beg_; i != end_; i++) {
            whichFoldToGo.push_back(++foldNo);

            if (foldNo == k_) {
                foldNo = 0;
            }
        }

        if (!k_) {
            throw runtime_error("With this value of k (="+to_string(k)+")Equal division of the data is not possible");
        }

        //        random_shuffle(whichFoldToGo.begin(), whichFoldToGo.end());
    }

    template<class Out>
    void getFold(int foldNo, Out training, Out testing) {

        int k = 0;
        In i = beg_;
        while (i != end_) {
            if (whichFoldToGo[k++] == foldNo) {
                *testing++ = *i++;
            } else
                *training++ = *i++;
        }
    }

private:
    In beg_;
    In end_;
    int k_; //how many folds in this
    vector<int> whichFoldToGo;

};

#endif // KFOLD_H
