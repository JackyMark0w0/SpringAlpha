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

bool SAS2Scan(const string& filePath);

void scanDirectory(const string& path, const vector<string>& keywords);

void scanFile(const string& filePath, const vector<string>& keywords);
