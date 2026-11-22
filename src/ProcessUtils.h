#pragma once

#include <Windows.h>
#include <string>

namespace ProcessUtils
{
    std::string GetProcessName(DWORD dPID);
}
