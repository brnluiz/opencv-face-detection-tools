#include "generaltrain.h"
#include "log.h"

GeneralTrain::GeneralTrain(vector<Mat> &pos, vector<Mat> &neg, int folds): pos_(pos), neg_(neg), folds_(folds) {
    ;
}

void GeneralTrain::setFolds(int k) {
    folds_ = k;
}

HOGDescriptor GeneralTrain::makeDescriptor(HogParam &params) {
    HOGDescriptor hog;
    hog.blockSize   = Size(params[BLOCK_SIZE], params[BLOCK_SIZE]);
    hog.cellSize    = Size(params[CELL_SIZE], params[CELL_SIZE]);
    hog.blockStride = Size(params[BLOCK_STRIDE], params[BLOCK_STRIDE]);
    hog.winSize     = Size(params[WIN_SIZE], params[WIN_SIZE]);

    return hog;
}

void GeneralTrain:: computeMultipleHog(Mat img, int type, const HOGDescriptor &hog, vector<SampleInfo>& samples) {
    Trainer trainer;
    SampleInfo s;

    // Compute the HOG (normal image)
    s.image = img.clone();
    s.hog = Mat(trainer.computeHog(img, hog));
    s.type = type;
    samples.push_back(s);

    // Get the flipped version as well
    s.hog = Mat(trainer.computeHog(img, hog, true));
    samples.push_back(s);
}

Kfold<vector<SampleInfo>::const_iterator> GeneralTrain::prepareSamples(vector<Mat> &pos, vector<Mat> &neg, const HOGDescriptor& hog) {
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

    Kfold<vector<SampleInfo>::const_iterator> kf(10, samples.begin(), samples.end());

    return kf;
}
