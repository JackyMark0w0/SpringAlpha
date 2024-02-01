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

#include <fstream>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

bool scanFile(const string& filePath, const vector<string>& keywords) {
    ifstream file(filePath);
    if (!file.is_open()) {
        return false;
    }
    string line;
    while (getline(file, line)) {
        for (const auto& keyword : keywords) {
            if (line.find(keyword) != string::npos) {
                return true;
            }
        }
    }
    return false;
}

bool scanDirectory(const string& path, const vector<string>& keywords) {
    string searchPath = path + "/*.*";
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        return false;
    }
    bool found = false;
    do {
        string fileName = findData.cFileName;
        string filePath = path + "/" + fileName;
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (fileName != "." && fileName != "..") {
                if (scanDirectory(filePath, keywords)) {
                    found = true;
                }
            }
        }
        else {
            if (scanFile(filePath, keywords)) {
                found = true;
            }
        }
    } while (FindNextFileA(hFind, &findData) != 0);

    FindClose(hFind);
    return found;
}

bool SAS2Scan(const string& filePath) {
    vector<string> keywords = { "upx", "www", "com","https","Global","Sign","Heap","Destroy","Size","Alloc","Load","Library","Close","Handle","Resource","Find","Ex","Local","Get","Module","Name","Delete","File","Current","Process","Write","Read","Create","System","Time","As","Version","Virtual","Free","Protect","Computer","Exit","Code","Cmd","Terminate","Thread","Command","Line","Current","Id","Post","KERNEL32.dll","kernel32.dll","Window","Text","Set","Pos","Client","Rect","Image","Send","show","Cursor","Translate","Reg","Key","Shell","Execute","Exit","Console","Mode","Type","Module","Open","Main","main","Core","core","Bit","Net","public" };

    const int bufferSize = 1024 * 1024; 
    char* buffer = new char[bufferSize]; 
    ifstream file(filePath, ios::binary);
    if (!file.is_open()) {
        delete[] buffer; 
        return false;
    }
    bool found = false;
    while (file.read(buffer, bufferSize)) {
        string content(buffer, file.gcount());
        size_t pos = 0;
        while ((pos = content.find('\n', pos)) != string::npos) {
            ++pos;
            string line = content.substr(0, pos);
            if (scanFile(line, keywords)) {
                found = true;
                break;
            }
            content.erase(0, pos);
            pos = 0;
        }
        if (found) {
            break;
        }
    }
    if (!found) {
        string content(buffer, file.gcount());
        if (scanFile(content, keywords)) {
            found = true;
        }
    }
    file.close();

    delete[] buffer; 

    return found;
}
