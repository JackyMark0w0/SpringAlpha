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

#include "PEFunctionScan.h"

void changeExtensionToDll(const std::string& programPath) {
    WIN32_FIND_DATAA findData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    std::string searchPath = programPath + "\\*";
    hFind = FindFirstFileA(searchPath.c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            std::string fileName = findData.cFileName;
            std::string filePath = programPath + "\\" + fileName;
            std::string fileExtension = fileName.substr(fileName.find_last_of(".") + 1);

            if (fileExtension == "exe" || fileExtension == "com") {
                std::string newFilePath = filePath.substr(0, filePath.length() - fileExtension.length()) + ".dll";
                MoveFileA(filePath.c_str(), newFilePath.c_str());
            }
        } while (FindNextFileA(hFind, &findData) != 0);

        FindClose(hFind);
    }
}

void changeExtensionBack(const std::string& programPath) {
    WIN32_FIND_DATAA findData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    std::string searchPath = programPath + "\\*";
    hFind = FindFirstFileA(searchPath.c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            std::string fileName = findData.cFileName;
            std::string filePath = programPath + "\\" + fileName;
            std::string fileExtension = fileName.substr(fileName.find_last_of(".") + 1);

            if (fileExtension == "dll") {
                std::string originalExtension = "exe";
                if (filePath.find(".com.dll") != std::string::npos) {
                    originalExtension = "com";
                }
                std::string newFilePath = filePath.substr(0, filePath.length() - fileExtension.length()) + originalExtension;
                MoveFileA(filePath.c_str(), newFilePath.c_str());
            }
        } while (FindNextFileA(hFind, &findData) != 0);

        FindClose(hFind);
    }
}

bool IsMaliciousFunction(const std::string& functionName) {
    return std::any_of(MaliciousFunctions.begin(), MaliciousFunctions.end(),
        [&functionName](const std::string& maliciousFunction) {
            return std::search(functionName.begin(), functionName.end(),
            maliciousFunction.begin(), maliciousFunction.end()) != functionName.end();
        });
}

bool ScanPEFile(const std::string& fileName) {
    std::unique_ptr<std::remove_pointer<HMODULE>::type, decltype(&FreeLibrary)> hModule(
        LoadLibraryA(fileName.c_str()), FreeLibrary);
    if (!hModule) {
        return false;
    }

    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)hModule.get();
    PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((BYTE*)hModule.get() + dosHeader->e_lfanew);
    PIMAGE_IMPORT_DESCRIPTOR importDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((BYTE*)hModule.get() +
        ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

    while (importDescriptor->Characteristics != 0) {
        PIMAGE_THUNK_DATA thunkData = (PIMAGE_THUNK_DATA)((BYTE*)hModule.get() + importDescriptor->FirstThunk);

        while (thunkData->u1.AddressOfData != 0) {
            PIMAGE_IMPORT_BY_NAME importByName = (PIMAGE_IMPORT_BY_NAME)((BYTE*)hModule.get() +
                thunkData->u1.AddressOfData);

            std::string functionName = reinterpret_cast<char*>(importByName->Name);
            if (IsMaliciousFunction(functionName)) {
                return true;
            }

            ++thunkData;
        }

        ++importDescriptor;
    }

    return false;
}
