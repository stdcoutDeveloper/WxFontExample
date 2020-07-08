#pragma once

#include "pch.h"
#include <Windows.h>
#include <string>

namespace WxFont
{
    // IDs for the controls and the menu commands
    enum
    {
        // menu items
        Font_Quit = wxID_EXIT,
        Font_About = wxID_ABOUT,

        Font_ViewMsg = wxID_HIGHEST + 1,
        Font_TestTextValue,

        Font_IncSize,
        Font_DecSize,

        Font_GetBaseFont,

        Font_Bold,
        Font_Light,

        Font_Italic,
        Font_Slant,

        Font_Underlined,
        Font_Strikethrough,

        // standard global wxFont objects:
        Font_wxNORMAL_FONT,
        Font_wxSMALL_FONT,
        Font_wxITALIC_FONT,
        Font_wxSWISS_FONT,
        Font_wxFont_Default,
        Font_Standard,

        // wxSystemSettings::GetFont possible objects:
        Font_wxSYS_OEM_FIXED_FONT,
        Font_wxSYS_ANSI_FIXED_FONT,
        Font_wxSYS_ANSI_VAR_FONT,
        Font_wxSYS_SYSTEM_FONT,
        Font_wxSYS_DEVICE_DEFAULT_FONT,
        Font_wxSYS_DEFAULT_GUI_FONT,
        Font_SystemSettings,

        Font_Choose = 100,
        Font_EnumFamiliesForEncoding,
        Font_EnumFamilies,
        Font_EnumFixedFamilies,
        Font_EnumEncodings,
        Font_SetNativeDesc,
        Font_SetNativeUserDesc,
        Font_SetFamily,
        Font_SetFaceName,
        Font_SetEncoding,

        Font_Private,
        Font_Max
    };

    // used as title for several dialog boxes
    inline wxString GetSampleTitle()
    {
        return "wxWidgets Font Sample";
    }

    inline std::wstring GetCurrentExeFilePath()
    {
        TCHAR buffer[MAX_PATH] = {};
        GetModuleFileName(nullptr, buffer, sizeof(buffer));
        auto pos = std::wstring(buffer).find_last_of(L"\\/");
        return std::wstring(buffer).substr(0, pos);
    }
}
