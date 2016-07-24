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

void GeneralTrain::computeMultipleHog(Mat img, int type, const HOGDescriptor &hog, vector<SampleInfo>& samples) {
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

    Kfold<vector<SampleInfo>::const_iterator> kf(folds_, samples.begin(), samples.end());

    return kf;
}

vector<SampleInfo> GeneralTrain::prepareSamples(const vector<Mat> &set, const HOGDescriptor &hog, const int &type) {
    Trainer trainer;

    vector<SampleInfo> samples;
    for(vector<Mat>::const_iterator img = set.begin(); img < set.end(); img++) {
        SampleInfo s;
        s.image = (*img).clone();
        s.type = type;

        // Normal, not flipped
        s.hog = trainer.computeHog(s.image, hog, false).clone();
        samples.push_back(s);

        // Normal, flipped
        s.hog = trainer.computeHog(s.image, hog, false).clone();
        samples.push_back(s);
    }

    return samples;
}

void GeneralTrain::prepareSvmParameters(vector<Mat>& gradient_lst, vector<int>& labels, const vector<SampleInfo>& pos, const vector<SampleInfo>& neg) {
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
