/*
 * Copyright(C) 0JackyMark0 2024
 * All Right Reserved
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "SVMScan.h"

struct Data {
    vector<double> features;
    int label;
};

struct SVMModel {
    double b;
    vector<double> alphas;
    vector<Data> support_vectors;
};

vector<Data> load_data(string filename) {
    vector<Data> data;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        bool has_duplicate = false;
        Data d;
        stringstream ss(line);
        string feature;
        while (getline(ss, feature, ',')) {
            d.features.push_back(stod(feature));
        }
        // Check if the features are duplicates
        for (int i = 0; i < data.size(); i++) {
            bool is_duplicate = true;
            for (int j = 0; j < d.features.size(); j++) {
                if (data[i].features[j] != d.features[j]) {
                    is_duplicate = false;
                    break;
                }
            }
            if (is_duplicate) {
                has_duplicate = true;
                break;
            }
        }
        if (has_duplicate) {
            continue; // Skip this data point
        }
        istringstream label_stream(feature);
        int label;
        label_stream >> label;
        d.label = label;
        data.push_back(d);
    }
    return data;
}

double kernel(vector<double>&x1, vector<double>&x2) {
    double dot_product = 0.0;
    for (int i = 0; i < x1.size(); i++) {
        dot_product += x1[i] * x2[i];
    }
    return dot_product;
}

SVMModel train_svm(vector<Data>&data) {
    int n = data.size();
    vector<double> alphas(n, 0.0);
    double b = 0.0;
    bool is_converged = false;
    while (!is_converged) {
        int num_changed_alphas = 0;
        for (int i = 0; i < n; i++) {
            double Ei = 0.0;
            for (int j = 0; j < n; j++) {
                Ei += alphas[j] * data[j].label * kernel(data[i].features, data[j].features);
            }
            Ei = Ei - (double)data[i].label;
            if ((data[i].label * Ei < -TOLERANCE && alphas[i] < C) ||
                (data[i].label * Ei > TOLERANCE && alphas[i] > 0)) {
                int j = i;
                while (j == i) {
                    j = rand() % n;
                }
                double Ej = 0.0;
                for (int k = 0; k < n; k++) {
                    Ej += alphas[k] * data[k].label * kernel(data[j].features, data[k].features);
                }
                Ej = Ej - (double)data[j].label;
                double alpha_i_old = alphas[i];
                double alpha_j_old = alphas[j];
                double L, H;
                if (data[i].label != data[j].label) {
                    L = max(0.0, alpha_j_old - alpha_i_old);
                    H = min(C, C + alpha_j_old - alpha_i_old);
                }
                else {
                    L = max(0.0, alpha_i_old + alpha_j_old - C);
                    H = min(C, alpha_i_old + alpha_j_old);
                }
                if (L == H) {
                    continue;
                }
                double eta = 2.0 * kernel(data[i].features, data[j].features) -
                    kernel(data[i].features, data[i].features) -
                    kernel(data[j].features, data[j].features);
                if (eta >= 0.0) {
                    continue;
                }
                alphas[j] = alpha_j_old - data[j].label * (Ei - Ej) / eta;
                if (alphas[j] > H) {
                    alphas[j] = H;
                }
                else if (alphas[j] < L) {
                    alphas[j] = L;
                }
                if (abs(alphas[j] - alpha_j_old) < TOLERANCE) {
                    continue;
                }
                alphas[i] = alpha_i_old + data[i].label * data[j].label *
                    (alpha_j_old - alphas[j]);
                double b1 = b - Ei - data[i].label * (alphas[i] - alpha_i_old) *
                    kernel(data[i].features, data[i].features) -
                    data[j].label * (alphas[j] - alpha_j_old) *
                    kernel(data[i].features, data[j].features);
                double b2 = b - Ej - data[i].label * (alphas[i] - alpha_i_old) *
                    kernel(data[i].features, data[j].features) -
                    data[j].label * (alphas[j] - alpha_j_old) *
                    kernel(data[j].features, data[j].features);
                if (0 < alphas[i] && alphas[i] < C) {
                    b = b1;
                }
                else if (0 < alphas[j] && alphas[j] < C) {
                    b = b2;
                }
                else {
                    b = (b1 + b2) / 2.0;
                }
                num_changed_alphas++;
            }
        }
        if (num_changed_alphas == 0) {
            is_converged = true;
        }
    }
    SVMModel model;
    model.b = b;
    model.alphas = alphas;
    for (int i = 0; i < n; i++) {
        if (alphas[i] > 0.0) {
            model.support_vectors.push_back(data[i]);
        }
    }
    return model;
}

double predict(SVMModel & model, vector<double>&x) {
    double f = -model.b;
    for (int i = 0; i < model.support_vectors.size(); i++) {
        f += model.alphas[i] * model.support_vectors[i].label *
            kernel(model.support_vectors[i].features, x);
    }
    if (f >= 0.0) {
        return 1.0;
    }
    else {
        return -1.0;
    }
}

vector<double> function_to_vector(function<double()> function) {
    vector<double> result;
    for (int i = 0; i < 3; i++) {
        double value = function();
        result.push_back(value);
    }
    return result;
}

double cosine_similarity(vector<double>&v1, vector<double>&v2) {
    double dot_product = 0.0, norm1 = 0.0, norm2 = 0.0;
    for (int i = 0; i < v1.size(); i++) {
        dot_product += v1[i] * v2[i];
        norm1 += v1[i] * v1[i];
        norm2 += v2[i] * v2[i];
    }
    return dot_product / (sqrt(norm1) * sqrt(norm2));
}

vector<function<double()>> get_exported_functions(string file_path) {
    vector<function<double()>> functions;
    HINSTANCE library = LoadLibraryA(file_path.c_str());
    if (library != NULL) {
        for (int i = 0; i < MAX_FEATURES; i++) {
            string function_name = "feature" + to_string(i);
            function<double()> f = (function<double()>)GetProcAddress(library, function_name.c_str());
            if (f != NULL) {
                functions.push_back(f);
            }
            else {
                break;
            }
        }
        FreeLibrary(library);
    }
    return functions;
}

bool get_feature_last(string file_path1) {
    vector<Data> training_data = load_data("data.csv");

    // Train SVM model
    SVMModel model = train_svm(training_data);

    // Load exported feature functions
    vector<function<double()>> feature_functions = get_exported_functions(file_path1);

    // Extract features from input
    vector<double> input_features;
    for (int i = 0; i < feature_functions.size(); i++) {
        double feature = feature_functions[i]();
        input_features.push_back(feature);
    }

    // Predict the label of input
    double prediction = predict(model, input_features);
    if (prediction >= 0.5) {
        return true;
    }
}
