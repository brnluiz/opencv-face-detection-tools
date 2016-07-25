#ifndef HOGTRAIN_H
#define HOGTRAIN_H

#include "stats.h"
#include "trainer.h"
#include "kfold.h"
#include "train_abstract.h"

#include <vector>

using namespace std;
using namespace cv;

class HogTrain: public AbstractTrain {
public:

    HogTrain(vector<Mat>& pos, vector<Mat>& neg, const int &folds, HogParamList params);

    void run();

    HogBest getBest();

private:
    void combineVector (
        vector<vector<int> > & output_perms,
        vector<vector<int> > const & input,
        vector<int> & cur_perm,
        unsigned cur_row = 0
    );

protected:
    HogParamList params_;

    HogBest besthog_;
};

#endif // HOGTRAIN_H
