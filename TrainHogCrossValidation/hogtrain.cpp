#include "hogtrain.h"
#include "log.h"

HogTrain::HogTrain(vector<Mat> &pos, vector<Mat> &neg, HogParamList params): GeneralTrain(pos, neg) {
    // Generate the parameters combinations
    vector<int> temp;
    combineVector(params_, params, temp);
}

void HogTrain::run() {
    // Loop through the parameters
    for(uint i = 0; i < params_.size(); i++) {
        Trainer trainer;

        // Get HOG Parameters Configs
        HOGDescriptor hog = makeDescriptor(params_[i]);

        // Prepare pos and negatives samples
        vector<SampleInfo> pos_set = prepareSamples(pos_, hog, +1);
        vector<SampleInfo> neg_set = prepareSamples(neg_, hog, -1);

        Kfold<vector<SampleInfo>::const_iterator> kf_pos(folds_, pos_set.begin(), pos_set.end());
        Kfold<vector<SampleInfo>::const_iterator> kf_neg(folds_, neg_set.begin(), neg_set.end());

        // Cross-validation
        float acc;
        for (int fold = 0; fold != folds_; fold++) {
            HOGTRAIN_LOG << "Training Fold #" << fold+1 << endl;

            // Get the k fold for both sets (training and testing)
            vector<SampleInfo> train_pos, test_pos;
            vector<SampleInfo> train_neg, test_neg;
            kf_pos.getFold(fold + 1, back_inserter(train_pos), back_inserter(test_pos));
            kf_neg.getFold(fold + 1, back_inserter(train_neg), back_inserter(test_neg));

            // Allocate the SVM parameters (labels and HOG)
            HOGTRAIN_LOG << "Preparing SVM parameters on fold #" << fold+1 << endl;
            vector<int> labels;
            vector<Mat> gradient_lst;
            prepareSvmParameters(gradient_lst, labels, train_pos, train_neg);

            // Train a SVM using the actual HOGs
            HOGTRAIN_LOG << "Training SVM on fold #" << fold+1 << endl;
            Ptr<SVM> svm = SVM::create();
            trainer.trainSvm(gradient_lst, labels, svm);

            // Configure the detector to use the trained SVM
            vector<float> detector = trainer.getSvmDetector(svm);
            hog.setSVMDetector(detector);

            // Test the actual setting using the test set
            HOGTRAIN_LOG << "Testing Fold #" << fold+1 << endl;
            Stats stat(hog, test_pos, test_neg);
            stat.test();

            // Save the accuracy
            acc += stat.get_accuracy();

            HOGTRAIN_LOG << "Accuracy of fold #" << fold+1 << ": " << stat.get_accuracy() << endl;
        }

        // If the average accuracy is better than the last parameter set, then save it
        acc = acc / folds_;
        if (acc > besthog_.acc) {
            besthog_.descriptor = hog;
            besthog_.acc = acc;

            HOGTRAIN_LOG << "New best HOG!" << endl;
            besthog_.print();
        }
    }
}

HogBest HogTrain::getBest() {
    return besthog_;
}

void HogTrain::combineVector(vector<vector<int> > &output_perms, const vector<vector<int> > &input, vector<int> &cur_perm, unsigned cur_row) {
    if (cur_row >= input.size()) {
        // This is where you have found a new permutation.
        // Do whatever you want with it.
        output_perms.push_back (cur_perm);
        return;
    }

    for (unsigned i = 0; i < input[cur_row].size(); ++i) {
        cur_perm.push_back (input[cur_row][i]);
        combineVector(output_perms, input, cur_perm, cur_row + 1);
        cur_perm.pop_back ();
    }
}
