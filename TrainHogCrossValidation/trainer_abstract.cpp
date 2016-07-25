#include "trainer_abstract.h"

#include "log.h"
#include "utils_hog.h"
#include "utils_svm.h"

TrainerAbstract::TrainerAbstract(vector<Mat> &pos, vector<Mat> &neg, int folds): pos_(pos), neg_(neg), folds_(folds) {
    ;
}

void TrainerAbstract::setFolds(int k) {
    folds_ = k;
}

HOGDescriptor TrainerAbstract::makeDescriptor(const HogParam &params) {
    HOGDescriptor hog;
    hog.blockSize   = Size(params[BLOCK_SIZE], params[BLOCK_SIZE]);
    hog.cellSize    = Size(params[CELL_SIZE], params[CELL_SIZE]);
    hog.blockStride = Size(params[BLOCK_STRIDE], params[BLOCK_STRIDE]);
    hog.winSize     = Size(params[WIN_SIZE], params[WIN_SIZE]);

    return hog;
}

void TrainerAbstract::computeMultipleHog(const Mat &img, const int &type, const HOGDescriptor &hog, vector<SampleInfo>& samples) {
    SampleInfo s;
    s.image = img.clone();
    s.type = type;

    // Compute the HOG (normal image)
    s.hog = HogUtils::compute(img, hog).clone();
    samples.push_back(s);

    // Get the flipped version as well
    s.hog = HogUtils::compute(img, hog, true).clone();
    samples.push_back(s);
}

Kfold<vector<SampleInfo>::const_iterator> TrainerAbstract::prepareSamples(vector<Mat> &pos, vector<Mat> &neg, const HOGDescriptor& hog) {
    vector<SampleInfo> samples;

    TRAIN_LOG << "Starting HOG computing..." << endl;

    // Positive samples
    for (vector<Mat>::iterator img = pos.begin(); img < pos.end(); img++) {
        computeMultipleHog(*img, +1, hog, samples);
    }

    // Negative samples
    for (vector<Mat>::iterator img = neg.begin(); img < neg.end(); img++) {
        computeMultipleHog(*img, -1, hog, samples);
    }

    TRAIN_LOG << "Finished HOG computing" << endl;
    TRAIN_LOG << "Samples size: " << samples.size() << endl;

    Kfold<vector<SampleInfo>::const_iterator> kf(folds_, samples.begin(), samples.end());

    return kf;
}

vector<SampleInfo> TrainerAbstract::prepareSamples(const vector<Mat> &set, const HOGDescriptor &hog, const int &type) {
    vector<SampleInfo> samples;
    for(vector<Mat>::const_iterator img = set.begin(); img < set.end(); img++) {
        SampleInfo s;
        s.image = (*img).clone();
        s.type = type;

        // Normal, not flipped
        s.hog = HogUtils::compute(s.image, hog, false).clone();
        samples.push_back(s);

        // Normal, flipped
        s.hog = HogUtils::compute(s.image, hog, false).clone();
        samples.push_back(s);
    }

    return samples;
}

void TrainerAbstract::prepareSvmParameters(vector<Mat>& gradient_lst, vector<int>& labels, const vector<SampleInfo>& pos, const vector<SampleInfo>& neg) {
    vector<SampleInfo>::const_iterator info;
    for(info = pos.begin(); info < pos.end(); info++) {
        gradient_lst.push_back((*info).hog.clone());
        labels.push_back((*info).type);
    }

    for(info = neg.begin(); info < neg.end(); info++) {
        gradient_lst.push_back((*info).hog.clone());
        labels.push_back((*info).type);
    }
}
