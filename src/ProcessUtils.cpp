#include "ProcessUtils.h"

#include <TlHelp32.h>

/**
    @brief : Function that retrieve the process name from the PID.
    @param dPID : PID of the process.
    @retval : the executable name (UTF-8), or an empty string if the process has not been found.
**/
std::string ProcessUtils::GetProcessName(const DWORD dPID)
{
    const HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
        return {};

    std::string sName;
    PROCESSENTRY32W entry = {};
    entry.dwSize = sizeof(entry);

    for (BOOL bFound = Process32FirstW(hSnapshot, &entry); bFound; bFound = Process32NextW(hSnapshot, &entry))
    {
        if (entry.th32ProcessID != dPID)
            continue;

        char szName[MAX_PATH] = {};
        if (WideCharToMultiByte(CP_UTF8, 0, entry.szExeFile, -1, szName, sizeof(szName), nullptr, nullptr) > 0)
            sName = szName;
        break;
    }

    CloseHandle(hSnapshot);
    return sName;
}
