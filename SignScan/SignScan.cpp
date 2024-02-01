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

#include "SignScan.h"

TCHAR* StringToTCHAR(const string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    TCHAR* wstr = new TCHAR[size_needed];
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr, size_needed);
    return wstr;
}


bool VerifyEmbeddedSignature(LPCWSTR pwszSourceFile)
{
    LONG lStatus;
    DWORD dwLastError;

    WINTRUST_FILE_INFO FileData;
    memset(&FileData, 0, sizeof(FileData));
    FileData.cbStruct = sizeof(WINTRUST_FILE_INFO);
    FileData.pcwszFilePath = pwszSourceFile;
    FileData.hFile = NULL;
    FileData.pgKnownSubject = NULL;

    GUID WVTPolicyGUID = WINTRUST_ACTION_GENERIC_VERIFY_V2;
    WINTRUST_DATA WinTrustData;

    memset(&WinTrustData, 0, sizeof(WinTrustData));

    WinTrustData.cbStruct = sizeof(WinTrustData);

    WinTrustData.pPolicyCallbackData = NULL;

    WinTrustData.pSIPClientData = NULL;

    WinTrustData.dwUIChoice = WTD_UI_NONE;

    WinTrustData.fdwRevocationChecks = WTD_REVOKE_NONE;

    WinTrustData.dwUnionChoice = WTD_CHOICE_FILE;

    WinTrustData.dwStateAction = WTD_STATEACTION_VERIFY;

    WinTrustData.hWVTStateData = NULL;

    WinTrustData.pwszURLReference = NULL;

    WinTrustData.dwUIContext = 0;

    WinTrustData.pFile = &FileData;

    lStatus = WinVerifyTrust(
        NULL,
        &WVTPolicyGUID,
        &WinTrustData);

    switch (lStatus)
    {
    case ERROR_SUCCESS:
        return false;
        break;

    case TRUST_E_NOSIGNATURE:
        dwLastError = GetLastError();
        if (TRUST_E_NOSIGNATURE == dwLastError ||
            TRUST_E_SUBJECT_FORM_UNKNOWN == dwLastError ||
            TRUST_E_PROVIDER_UNKNOWN == dwLastError)
        {
            return true;
        }
        else
        {
            return true;
        }

        break;

    case TRUST_E_EXPLICIT_DISTRUST:
        return true;
        break;

    case TRUST_E_SUBJECT_NOT_TRUSTED:
        return true;
        break;

    case CRYPT_E_SECURITY_SETTINGS:
        return true;
        break;

    default:
        return true;
        break;
    }

    WinTrustData.dwStateAction = WTD_STATEACTION_CLOSE;

    lStatus = WinVerifyTrust(
        NULL,
        &WVTPolicyGUID,
        &WinTrustData);

}
