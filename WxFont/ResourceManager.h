#pragma once

#include <Windows.h>
#include "resource.h"

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

            auto res = FindResource(hModuleRes, MAKEINTRESOURCE(IDR_WX_PRIVATE_FONT), RT_FONT);
            if (res)
            {
                auto handleDataRes = LoadResource(hModuleRes, res);
                auto data = LockResource(handleDataRes);
                auto size = SizeofResource(hModuleRes, res);

                DWORD numbFonts;
                auto fontHandle = AddFontMemResourceEx(data, size, nullptr, &numbFonts);

                if (!fontHandle)
                {
                    // failed to load font resource.
                }
            }
        }

    private:
        ResourceManager() = default;
    };
}
