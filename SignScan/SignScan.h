#pragma once
#include <iostream>
#include <windows.h>
#include <softpub.h>
#include <wintrust.h>
#include <tchar.h>
#include <vector>

#pragma comment (lib,"wintrust")
#pragma comment (lib,"Iphlpapi.lib")

using namespace std;

TCHAR* StringToTCHAR(const string& str);

bool VerifyEmbeddedSignature(const TCHAR* pwszSourceFile);
