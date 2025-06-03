#ifdef _MSC_VER

#include <le-cpp-utils/winutils.h>


#pragma comment( lib, "Version" )

#include <windows.h>
#include <string>
#include <vector>
#include <sstream>

static std::string version;

std::string le::GetVersionString()
{
    if (!version.empty())
        return version;

    char szFilePath[MAX_PATH];
    GetModuleFileNameA(nullptr, szFilePath, MAX_PATH);

    DWORD dwDummy;
    DWORD dwFVISize = GetFileVersionInfoSizeA(szFilePath, &dwDummy);

    std::vector<BYTE> data(dwFVISize);

    if (GetFileVersionInfoA(szFilePath, 0, dwFVISize, &data[0]))
    {
        LPVOID lpBuffer;
        UINT uSize;
        if (VerQueryValueA(&data[0], "\\", &lpBuffer, &uSize))
        {
            VS_FIXEDFILEINFO* verInfo = (VS_FIXEDFILEINFO*)lpBuffer;
            if (verInfo->dwSignature == 0xfeef04bd)
            {
                // Doesn't need to be freed
                std::ostringstream ss;
                ss << ((verInfo->dwFileVersionMS >> 16) & 0xffff) << '.'
                    << ((verInfo->dwFileVersionMS >> 0) & 0xffff) << '.'
                    << ((verInfo->dwFileVersionLS >> 16) & 0xffff) << '.'
                    << ((verInfo->dwFileVersionLS >> 0) & 0xffff);
                version = ss.str();
                return version;
            }
        }
    }
    return "";
}

#endif