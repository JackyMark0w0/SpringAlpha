#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <memory>
#include <algorithm>
#include <windows.h>

#if defined(_WIN32)
#define DLLEXPORT   __declspec(dllexport)
#else
#define DLLEXPORT   __attribute__((visibility("default")))
#endif

const std::vector<std::string> MaliciousFunctions = {
    "Create", "File", "Delete", "Get", "Mode", "Console", "Output",
    "Heap", "DC", "Release", "Write", "Alloc", "Size", "of", "Cmd",
    "cmd", "Type", "Handle", "Close", "Load", "Free", "library",
    "Process", "Module", "Exit", "Local", "Thread", "System", "Byte",
    "Open", "Current", "Id", "Find", "Next", "Second", "First", "Device",
    "Set", "Run", "Pen", "Value", "Virtual", "Protect", "Command",
    "Environment", "mouse", "Mouse", "Key", "key", "Manager", "Icon",
    "Cursor", "Draw", "draw", "hook", "BitBlt", "Select", "Object",
    "Stretch", "Gen", "Crypt", "Cryptor", "Crypto", "Look", "Token",
    "Head", "Out", "Prepare", "Name", "@", "#", "_", "?", "*"
};

DLLEXPORT bool IsMaliciousFunction(const std::string& functionName);
DLLEXPORT bool ScanPEFile(const std::string& fileName);
DLLEXPORT void changeExtensionToDll(const std::string& programPath);
DLLEXPORT void changeExtensionBack(const std::string& programPath);
