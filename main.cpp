#include "head.h"

int virusCount = 0;
int scannedFileCount = 0;

bool isWindows7OrHigher()
{
    OSVERSIONINFOEX osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    osvi.dwMajorVersion = 6;
    osvi.dwMinorVersion = 1;

    DWORDLONG conditionMask = 0;
    VER_SET_CONDITION(conditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);
    VER_SET_CONDITION(conditionMask, VER_MINORVERSION, VER_GREATER_EQUAL);

    return VerifyVersionInfo(&osvi, VER_MAJORVERSION | VER_MINORVERSION, conditionMask);
}


void scanFile(const std::string& filePath)
{
    changeExtensionToDll(filePath);
    bool result2 = ScanPEFile(filePath);
    Sleep(900);
    changeExtensionBack(filePath);

    TCHAR* filepath = StringToTCHAR(filePath.c_str());
    bool result3 = VerifyEmbeddedSignature(filepath);
    bool result4 = get_feature_last(filePath);
    bool result1 = SAS2Scan(filePath);

    bool virusFound = false;
    if (result1) {
        std::cout << "Find The Virus : " + filePath + " | KeyWordScan || need to delete |\n";
        virusFound = true;
    }
    else if (result2) {
        std::cout << "Find The Virus : " + filePath + " | PEFunctionScan || need to delete |\n";
        virusFound = true;
    }
    else if (result3) {
        std::cout << "Find The Virus : " + filePath + " | SignScan || need to delete |\n";
        virusFound = true;
    }
    else if (result4) {
        std::cout << "Find The Virus : " + filePath + " | SVMScan || need to delete |\n";
        virusFound = true;
    }

    if (!virusFound) {
        std::cout << "This is a security program\n";
    }
    if (virusFound) {
        virusCount++; 
    }

    scannedFileCount++;
}

void scanDirectory(const std::string& directoryPath)
{
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA((directoryPath + "\\*").c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
                std::string filePath = directoryPath + "\\" + findData.cFileName;
                scanFile(filePath);
            }
        } while (FindNextFileA(hFind, &findData));

        FindClose(hFind);
    }
}

int main() {

    cout << "Please do not name the program with garbled characters, otherwise the scan engine will crash(请不要以乱码来命名程序，否则扫描引擎会崩溃)\n";

    if (!isWindows7OrHigher()) {
        std::cout << "This program requires Windows 7 or later." << std::endl;
        return 0;
    }

    std::string path;

    while (true) {
        std::cout << "Enter file or directory path (or 'exit' to quit): ";
        std::cin >> path;

        if (path == "exit") {
            break;
        }

        std::ifstream file(path);
        if (file) {
            file.close();
            scanFile(path);
        }
        else {
            scanDirectory(path);
        }

        std::cout << "      Scanned file numbers :  " << scannedFileCount << " \n" << "      Found the virus numbers : " << virusCount << std::endl;

        scannedFileCount = 0;
        virusCount = 0;
    }



    return 0;
}
