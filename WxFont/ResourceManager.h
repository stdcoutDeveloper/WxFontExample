#pragma once

#include <Windows.h>
#include "resource.h"
#include "Commons.h"

/**
 * \brief
 * reference: https://docs.microsoft.com/vi-vn/windows/win32/menurc/resource-types?redirectedfrom=MSDN
 */
namespace WxFont
{
    class ResourceManager
    {
    public:
        static void LoadFontFromResource()
        {
            auto hModuleRes = GetModuleHandle(nullptr);

            auto res = FindResource(hModuleRes, MAKEINTRESOURCE(IDR_WX_PRIVATE_FONT), L"BINARY");
            if (res)
            {
                auto size = SizeofResource(hModuleRes, res);
                auto handleDataRes = LoadResource(hModuleRes, res);
                auto data = LockResource(handleDataRes);

                auto path = GetCurrentExeFilePath() + L"\\wxprivate.ttf";
                DWORD dwBytesWritten = 0;

                auto hFile = CreateFile(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL,
                                        nullptr);
                auto rc = WriteFile(hFile, data, size, &dwBytesWritten, nullptr);
                CloseHandle(hFile); // important!!! finish writing file.
            }
        }

    private:
        ResourceManager() = default;
    };
}
