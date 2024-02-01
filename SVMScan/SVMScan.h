#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <Windows.h>

using namespace std;

const int MAX_FEATURES = 1000;
const double C = 1.0;
const double TOLERANCE = 0.001;

bool get_feature_last(string file_path1);
