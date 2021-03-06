#include "trainer_hog.h"

#include <fstream>

#include "log.h"
#include "utils_hog.h"
#include "utils_svm.h"

TrainerHog::TrainerHog(vector<Mat> &pos, vector<Mat> &neg, const int &folds, HogParamList params): TrainerAbstract(pos, neg, folds) {
    // Generate the parameters combinations
    vector<int> temp;
    combineVector(params_, params, temp);

    besthog_.descriptor = HOGDescriptor();
    besthog_.acc = 0;
}

void TrainerHog::run() {
    // Loop through the parameters
    for(uint i = 0; i < params_.size(); i++) {
        TRAINERHOG_LOG << "Parameter set #" << i+1 << endl;
        // Get HOG Parameters Configs
        HOGDescriptor hog = makeDescriptor(params_[i]);

        // Print the training params
        TRAINERHOG_LOG << "- Selected parameters: [ ";
        for(uint j = 0; j < params_[i].size(); j++) {
            cout << params_[i][j] << " ";
        }
        cout << "]" << endl;

        // Prepare pos and negatives samples (if it is an invalid set, just continue to the next one)
        TRAINERHOG_LOG << "- Preparing samples: computing HOG and labeling the samples" << endl;
        vector<SampleInfo> pos_set, neg_set;
        try {
            pos_set = prepareSamples(pos_, hog, +1);
            neg_set = prepareSamples(neg_, hog, -1);
        } catch (cv::Exception &e) {
            ERROR_LOG << "Invalid set of parameters" << endl;
            continue;
        }


        Kfold<vector<SampleInfo>::const_iterator> kf_pos(folds_, pos_set.begin(), pos_set.end());
        Kfold<vector<SampleInfo>::const_iterator> kf_neg(folds_, neg_set.begin(), neg_set.end());

        // Cross-validation
        float acc = 0;
        TRAINERHOG_LOG << "- Cross validating..." << endl;
        for (int fold = 0; fold != folds_; fold++) {
            TRAINERHOG_LOG << " * Fold #" << fold+1 << ": training..."<< endl;

            // Get the k fold for both sets (training and testing)
            vector<SampleInfo> train_pos, test_pos;
            vector<SampleInfo> train_neg, test_neg;
            kf_pos.getFold(fold + 1, back_inserter(train_pos), back_inserter(test_pos));
            kf_neg.getFold(fold + 1, back_inserter(train_neg), back_inserter(test_neg));

            // Allocate the SVM parameters (labels and HOG)
            vector<int> labels;
            vector<Mat> gradient_lst;
            prepareSvmParameters(gradient_lst, labels, train_pos, train_neg);

            // Train a SVM using the actual HOGs
            Ptr<SVM> svm = SvmUtils::train(gradient_lst, labels);

            // Configure the detector to use the trained SVM
            vector<float> detector = SvmUtils::getDetector(svm);
            hog.setSVMDetector(detector);

            // Test the actual setting using the test set
            TRAINERHOG_LOG << " * Fold #" << fold+1 << ": testing..." << endl;
            Stats stat(hog, test_pos, test_neg);
            stat.test();

            // Save the accuracy
            acc += stat.getAccuracy();

            TRAINERHOG_LOG << " + Fold #" << fold+1 << " accuracy: " << stat.getAccuracy() << endl;
        }

        // Average accuracy (based on the number of folds)
        acc = acc / folds_;
        TRAINERHOG_LOG << "- Final accuracy: " << acc << endl << endl;

        // If the average accuracy is better than the last parameter set, then save it
        if (acc > besthog_.acc) {
            besthog_.descriptor = hog;
            besthog_.acc = acc;

            TRAINERHOG_LOG << "New best HOG!" << endl;
            besthog_.print();
        }
    }
}

void TrainerHog::saveReport(const string &output_path) {
    // Open a file for the report
    ofstream report;
    report.open(output_path);
    if(!report.is_open()) {
        exit(-1);
    }

    // Save the report
    report << besthog_;
    report.close();
}

HogBest TrainerHog::getBest() {
    return besthog_;
}

void TrainerHog::combineVector(vector<vector<int> > &output_perms, const vector<vector<int> > &input, vector<int> &cur_perm, unsigned cur_row) {
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
