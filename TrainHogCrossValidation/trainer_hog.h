#ifndef HOGTRAIN_H
#define HOGTRAIN_H

#include <vector>

#include "stats.h"
#include "kfold.h"
#include "trainer_abstract.h"

using namespace std;
using namespace cv;

class TrainerHog: public TrainerAbstract {
public:

    TrainerHog(vector<Mat>& pos, vector<Mat>& neg, const int &folds, HogParamList params);

    void run();
    void saveReport(const string & output_path);

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
