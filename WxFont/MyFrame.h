#pragma once

#include "FontWindow.h"
#include "wx/splitter.h"
#include "Commons.h"
#include "MyFontEnumerator.h"
#include "wx/fontmap.h"
#include "wx/textfile.h"
#include "wx/encconv.h"
#include "wx/fontdlg.h"
#include "sample.xpm"

namespace WxFont
{
    // Define a new frame type: this is going to be our main frame
    class MyFrame : public wxFrame
    {
    public:
        // ctor(s)
        MyFrame() : wxFrame(nullptr, wxID_ANY, "wxWidgets font sample")
        {
            SetIcon(wxICON(sample));

            // create a menu bar
            wxMenu* menuFile = new wxMenu;

            menuFile->Append(Font_TestTextValue, "&Test text value",
                             "Verify that getting and setting text value doesn't change it");
            menuFile->Append(Font_ViewMsg, "&View...\tCtrl-V",
                             "View an email message file");
            menuFile->AppendSeparator();
            menuFile->Append(Font_About, "&About\tCtrl-A", "Show about dialog");
            menuFile->AppendSeparator();
            menuFile->Append(Font_Quit, "E&xit\tAlt-X", "Quit this program");

            wxMenu* menuFont = new wxMenu;
            menuFont->Append(Font_IncSize, "&Increase font size by 2 points\tCtrl-I");
            menuFont->Append(Font_DecSize, "&Decrease font size by 2 points\tCtrl-D");
            menuFont->Append(Font_GetBaseFont, "Use &base version of the font\tCtrl-0");
            menuFont->AppendSeparator();
            menuFont->AppendCheckItem(Font_Bold, "&Bold\tCtrl-B", "Toggle bold state");
            menuFont->AppendCheckItem(Font_Light, "&Light\tCtrl-L", "Toggle light state");
            menuFont->AppendSeparator();
            menuFont->AppendCheckItem(Font_Italic, "&Oblique\tCtrl-O", "Toggle italic state");
#ifndef __WXMSW__
			// under wxMSW slant == italic so there's no reason to provide another menu item for the same thing
			menuFont->AppendCheckItem(Font_Slant, "&Slant\tCtrl-S", "Toggle slant state");
#endif
            menuFont->AppendSeparator();
            menuFont->AppendCheckItem(Font_Underlined, "&Underlined\tCtrl-U",
                                      "Toggle underlined state");
            menuFont->AppendCheckItem(Font_Strikethrough, "&Strikethrough",
                                      "Toggle strikethrough state");

            menuFont->AppendSeparator();
            menuFont->Append(Font_SetNativeDesc,
                             "Set native font &description\tShift-Ctrl-D");
            menuFont->Append(Font_SetNativeUserDesc,
                             "Set &user font description\tShift-Ctrl-U");
            menuFont->AppendSeparator();
            menuFont->Append(Font_SetFamily, "Set font family");
            menuFont->Append(Font_SetFaceName, "Set font face name");
            menuFont->Append(Font_SetEncoding, "Set font &encoding\tShift-Ctrl-E");

            wxMenu* menuSelect = new wxMenu;
            menuSelect->Append(Font_Choose, "&Select font...\tCtrl-S",
                               "Select a standard font");

            wxMenu* menuStdFonts = new wxMenu;
            menuStdFonts->Append(Font_wxNORMAL_FONT, "wxNORMAL_FONT", "Normal font used by wxWidgets");
            menuStdFonts->Append(Font_wxSMALL_FONT, "wxSMALL_FONT", "Small font used by wxWidgets");
            menuStdFonts->Append(Font_wxITALIC_FONT, "wxITALIC_FONT", "Italic font used by wxWidgets");
            menuStdFonts->Append(Font_wxSWISS_FONT, "wxSWISS_FONT", "Swiss font used by wxWidgets");
            menuStdFonts->Append(Font_wxFont_Default, "wxFont()", "wxFont constructed from default wxFontInfo");
            menuSelect->Append(Font_Standard, "Standar&d fonts", menuStdFonts);

            wxMenu* menuSettingFonts = new wxMenu;
            menuSettingFonts->Append(Font_wxSYS_OEM_FIXED_FONT, "wxSYS_OEM_FIXED_FONT",
                                     "Original equipment manufacturer dependent fixed-pitch font.");
            menuSettingFonts->Append(Font_wxSYS_ANSI_FIXED_FONT, "wxSYS_ANSI_FIXED_FONT",
                                     "Windows fixed-pitch (monospaced) font. ");
            menuSettingFonts->Append(Font_wxSYS_ANSI_VAR_FONT, "wxSYS_ANSI_VAR_FONT",
                                     "Windows variable-pitch (proportional) font.");
            menuSettingFonts->Append(Font_wxSYS_SYSTEM_FONT, "wxSYS_SYSTEM_FONT",
                                     "System font.");
            menuSettingFonts->Append(Font_wxSYS_DEVICE_DEFAULT_FONT, "wxSYS_DEVICE_DEFAULT_FONT",
                                     "Device-dependent font.");
            menuSettingFonts->Append(Font_wxSYS_DEFAULT_GUI_FONT, "wxSYS_DEFAULT_GUI_FONT",
                                     "Default font for user interface objects such as menus and dialog boxes. ");
            menuSelect->Append(Font_SystemSettings, "System fonts", menuSettingFonts);

            menuSelect->AppendSeparator();
            menuSelect->Append(Font_EnumFamilies, "Enumerate font &families\tCtrl-F");
            menuSelect->Append(Font_EnumFixedFamilies,
                               "Enumerate fi&xed font families\tCtrl-X");
            menuSelect->Append(Font_EnumEncodings,
                               "Enumerate &encodings\tCtrl-E");
            menuSelect->Append(Font_EnumFamiliesForEncoding,
                               "Find font for en&coding...\tCtrl-C",
                               "Find font families for given encoding");

#if wxUSE_PRIVATE_FONTS
            // Try to use a private font, under most platforms we just look for it in
            // the current directory but under OS X it must be in a specific location
            // so look for it there.
            //
            // For OS X you also need to ensure that you actually do put wxprivate.ttf
            // in font.app/Contents/Resources/Fonts and add the following snippet
            //
            //     <plist version="0.9">
            //       <dict>
            //         ...
            //         <key>ATSApplicationFontsPath</key>
            //         <string>Fonts</string>
            //         ...
            //       </dict>
            //     </plist>
            //
            // to your font.app/Contents/Info.plist.

            wxString privfont;
#ifdef __WXOSX__
			privfont << wxStandardPaths::Get().GetResourcesDir() << "/Fonts/";
#endif
            privfont << "wxprivate.ttf";

            if (!wxFont::AddPrivateFont(privfont))
            {
                wxLogWarning("Failed to add private font from \"%s\"", privfont);
            }
            else
            {
                menuSelect->AppendSeparator();
                menuSelect->Append(Font_Private,
                                   "Select private font",
                                   "Select a font available only in this application");
            }
#endif // wxUSE_PRIVATE_FONTS


            // now append the freshly created menu to the menu bar...
            wxMenuBar* menuBar = new wxMenuBar;
            menuBar->Append(menuFile, "&File");
            menuBar->Append(menuFont, "F&ont");
            menuBar->Append(menuSelect, "&Select");

            // ... and attach this menu bar to the frame
            SetMenuBar(menuBar);

            wxSplitterWindow* splitter = new wxSplitterWindow(this);

            m_fontWindow = new FontWindow(splitter);

            m_fontWindow->Bind(wxEVT_BUTTON, &MyFrame::OnFontPanelApply, this);

            m_textctrl = new wxTextCtrl(splitter, wxID_ANY,
                                        "Paste text here to see how it looks\nlike in the given font",
                                        wxDefaultPosition,
                                        wxSize(-1, 6 * GetCharHeight()),
                                        wxTE_MULTILINE);

            splitter->SplitHorizontally(m_fontWindow, m_textctrl, 0);

#if wxUSE_STATUSBAR
            // create a status bar just for fun (by default with 1 pane only)
            CreateStatusBar();
            SetStatusText("Welcome to wxWidgets font demo!");
#endif // wxUSE_STATUSBAR

            SetClientSize(splitter->GetBestSize());
            splitter->SetSashPosition(m_fontWindow->GetBestSize().y);
        }

        // event handlers (these functions should _not_ be virtual)
        void OnQuit(wxCommandEvent& event)
        {
            // true is to force the frame to close
            Close(true);
        }

        void OnAbout(wxCommandEvent& event)
        {
            wxMessageBox("wxWidgets font sample\n"
                         "(c) 1999-2006 Vadim Zeitlin",
                         wxString("About ") + GetSampleTitle(),
                         wxOK | wxICON_INFORMATION, this);
        }

        void OnGetBaseFont(wxCommandEvent& WXUNUSED(event))
        {
            DoChangeFont(m_fontWindow->GetTextFont().GetBaseFont());
        }

        void OnIncFont(wxCommandEvent& WXUNUSED(event))
        {
            DoResizeFont(+2);
        }

        void OnDecFont(wxCommandEvent& WXUNUSED(event))
        {
            DoResizeFont(-2);
        }

        void OnBold(wxCommandEvent& event)
        {
            wxFont font = m_fontWindow->GetTextFont();

            font.SetWeight(event.IsChecked() ? wxFONTWEIGHT_BOLD : wxFONTWEIGHT_NORMAL);
            DoChangeFont(font);
        }

        void OnLight(wxCommandEvent& event)
        {
            wxFont font = m_fontWindow->GetTextFont();

            font.SetWeight(event.IsChecked() ? wxFONTWEIGHT_LIGHT : wxFONTWEIGHT_NORMAL);
            DoChangeFont(font);
        }

        void OnItalic(wxCommandEvent& event)
        {
            wxFont font = m_fontWindow->GetTextFont();

            font.SetStyle(event.IsChecked() ? wxFONTSTYLE_ITALIC : wxFONTSTYLE_NORMAL);
            DoChangeFont(font);
        }

        void OnSlant(wxCommandEvent& event)
        {
            wxFont font = m_fontWindow->GetTextFont();

            font.SetStyle(event.IsChecked() ? wxFONTSTYLE_SLANT : wxFONTSTYLE_NORMAL);
            DoChangeFont(font);
        }

        void OnUnderline(wxCommandEvent& event)
        {
            wxFont font = m_fontWindow->GetTextFont();

            font.SetUnderlined(event.IsChecked());
            DoChangeFont(font);
        }

        void OnStrikethrough(wxCommandEvent& event)
        {
            wxFont font = m_fontWindow->GetTextFont();
            font.SetStrikethrough(event.IsChecked());
            DoChangeFont(font);
        }

        void OnwxPointerFont(wxCommandEvent& event)
        {
            wxFont font;

            switch (event.GetId())
            {
            case Font_wxNORMAL_FONT:
                font = *wxNORMAL_FONT;
                break;

            case Font_wxSMALL_FONT:
                font = *wxSMALL_FONT;
                break;

            case Font_wxITALIC_FONT:
                font = *wxITALIC_FONT;
                break;

            case Font_wxSWISS_FONT:
                font = *wxSWISS_FONT;
                break;

            default:
                wxFAIL_MSG("unknown standard font");
                return;
            }

            DoChangeFont(font);
        }

        void OnFontDefault(wxCommandEvent& event)
        {
            DoChangeFont(wxFont(wxFontInfo()));
        }

        void OnwxSystemSettingsFont(wxCommandEvent& event)
        {
            wxFont font;

            switch (event.GetId())
            {
            case Font_wxSYS_OEM_FIXED_FONT:
                font = wxSystemSettings::GetFont(wxSYS_OEM_FIXED_FONT);
                break;

            case Font_wxSYS_ANSI_FIXED_FONT:
                font = wxSystemSettings::GetFont(wxSYS_ANSI_FIXED_FONT);
                break;

            case Font_wxSYS_ANSI_VAR_FONT:
                font = wxSystemSettings::GetFont(wxSYS_ANSI_VAR_FONT);
                break;

            case Font_wxSYS_SYSTEM_FONT:
                font = wxSystemSettings::GetFont(wxSYS_SYSTEM_FONT);
                break;

            case Font_wxSYS_DEVICE_DEFAULT_FONT:
                font = wxSystemSettings::GetFont(wxSYS_DEVICE_DEFAULT_FONT);
                break;

            case Font_wxSYS_DEFAULT_GUI_FONT:
                font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
                break;

            default:
                wxFAIL_MSG("unknown standard font");
                return;
            }

            DoChangeFont(font);
        }

        void OnTestTextValue(wxCommandEvent& event)
        {
            wxString value = m_textctrl->GetValue();
            m_textctrl->SetValue(value);
            if (m_textctrl->GetValue() != value)
            {
                wxLogError("Text value changed after getting and setting it");
            }
        }

        void OnViewMsg(wxCommandEvent& event)
        {
#if wxUSE_FILEDLG
            // first, choose the file
            static wxString s_dir, s_file;
            wxFileDialog dialog(this, "Open an email message file",
                                s_dir, s_file);
            if (dialog.ShowModal() != wxID_OK)
                return;

            // save for the next time
            s_dir = dialog.GetDirectory();
            s_file = dialog.GetFilename();

            wxString filename = dialog.GetPath();

            // load it and search for Content-Type header
            wxTextFile file(filename);
            if (!file.Open())
                return;

            wxString charset;

            wxString prefix = "Content-Type: text/plain; charset=";
            const size_t len = wxStrlen(prefix);

            size_t n, count = file.GetLineCount();
            for (n = 0; n < count; n++)
            {
                wxString line = file[n];

                if (!line)
                {
                    // if it is an email message, headers are over, no need to parse
                    // all the file
                    break;
                }

                if (line.Left(len) == prefix)
                {
                    // found!
                    const wxChar* pc = line.c_str() + len;
                    if (*pc == '"')
                        pc++;

                    while (*pc && *pc != '"')
                    {
                        charset += *pc++;
                    }

                    break;
                }
            }

            if (!charset)
            {
                wxLogError("The file '%s' doesn't contain charset information.",
                           filename);

                return;
            }

            // ok, now get the corresponding encoding
            wxFontEncoding fontenc = wxFontMapper::Get()->CharsetToEncoding(charset);
            if (fontenc == wxFONTENCODING_SYSTEM)
            {
                wxLogError("Charset '%s' is unsupported.", charset);
                return;
            }

            m_textctrl->LoadFile(filename);

            if (fontenc == wxFONTENCODING_UTF8 ||
                !wxFontMapper::Get()->IsEncodingAvailable(fontenc))
            {
                // try to find some similar encoding:
                wxFontEncoding encAlt;
                if (wxFontMapper::Get()->GetAltForEncoding(fontenc, &encAlt))
                {
                    wxEncodingConverter conv;

                    if (conv.Init(fontenc, encAlt))
                    {
                        fontenc = encAlt;
                        m_textctrl->SetValue(conv.Convert(m_textctrl->GetValue()));
                    }
                    else
                    {
                        wxLogWarning("Cannot convert from '%s' to '%s'.",
                                     wxFontMapper::GetEncodingDescription(fontenc),
                                     wxFontMapper::GetEncodingDescription(encAlt));
                    }
                }
                else
                    wxLogWarning("No fonts for encoding '%s' on this system.",
                                 wxFontMapper::GetEncodingDescription(fontenc));
            }

            // and now create the correct font
            if (!DoEnumerateFamilies(false, fontenc, true /* silent */))
            {
                wxFont font(wxFontInfo(wxNORMAL_FONT->GetPointSize()).Encoding(fontenc));
                if (font.IsOk())
                {
                    DoChangeFont(font);
                }
                else
                {
                    wxLogWarning("No fonts for encoding '%s' on this system.",
                                 wxFontMapper::GetEncodingDescription(fontenc));
                }
            }
#endif // wxUSE_FILEDLG
        }

        void OnSelectFont(wxCommandEvent& event)
        {
            wxFontData data;
            data.SetInitialFont(m_fontWindow->GetTextFont());
            data.SetColour(m_fontWindow->GetColour());

            wxFontDialog dialog(this, data);
            if (dialog.ShowModal() == wxID_OK)
            {
                wxFontData retData = dialog.GetFontData();
                wxFont font = retData.GetChosenFont();
                wxColour colour = retData.GetColour();

                DoChangeFont(font, colour);
            }
        }

        void OnEnumerateFamiliesForEncoding(wxCommandEvent& WXUNUSED(event))
        {
            wxFontEncoding enc = GetEncodingFromUser();
            if (enc != wxFONTENCODING_SYSTEM)
            {
                DoEnumerateFamilies(false, enc);
            }
        }

        void OnEnumerateFamilies(wxCommandEvent& WXUNUSED(event))
        {
            DoEnumerateFamilies(false);
        }

        void OnEnumerateFixedFamilies(wxCommandEvent& WXUNUSED(event))
        {
            DoEnumerateFamilies(true);
        }

        void OnEnumerateEncodings(wxCommandEvent& event)
        {
            MyEncodingEnumerator fontEnumerator;

            fontEnumerator.EnumerateEncodings();

            wxLogMessage("Enumerating all available encodings:\n%s",
                         fontEnumerator.GetText());
        }

        void OnSetNativeDesc(wxCommandEvent& WXUNUSED(event))
        {
            wxString fontInfo = wxGetTextFromUser
            (
                "Enter native font string",
                "Input font description",
                m_fontWindow->GetTextFont().GetNativeFontInfoDesc(),
                this
            );
            if (fontInfo.empty())
                return; // user clicked "Cancel" - do nothing

            wxFont font;
            font.SetNativeFontInfo(fontInfo);
            if (!font.IsOk())
            {
                wxLogError("Font info string \"%s\" is invalid.",
                           fontInfo);
                return;
            }

            DoChangeFont(font);
        }

        void OnSetNativeUserDesc(wxCommandEvent& WXUNUSED(event))
        {
            wxString fontdesc = m_fontWindow->GetTextFont().GetNativeFontInfoUserDesc();
            wxString fontUserInfo = wxGetTextFromUser(
                "Here you can edit current font description",
                "Input font description", fontdesc,
                this);
            if (fontUserInfo.IsEmpty())
                return; // user clicked "Cancel" - do nothing

            wxFont font;
            if (font.SetNativeFontInfoUserDesc(fontUserInfo))
            {
                wxASSERT_MSG(font.IsOk(), "The font should now be valid");
                DoChangeFont(font);
            }
            else
            {
                wxASSERT_MSG(!font.IsOk(), "The font should now be invalid");
                wxMessageBox("Error trying to create a font with such description...");
            }
        }

        void OnSetFamily(wxCommandEvent& event)
        {
            wxFontFamily f = GetFamilyFromUser();

            wxFont font = m_fontWindow->GetTextFont();
            font.SetFamily(f);
            DoChangeFont(font);
        }

        void OnSetFaceName(wxCommandEvent& event)
        {
            wxString facename = m_fontWindow->GetTextFont().GetFaceName();
            wxString newFaceName = wxGetTextFromUser(
                "Here you can edit current font face name.",
                "Input font facename", facename,
                this);
            if (newFaceName.IsEmpty())
                return; // user clicked "Cancel" - do nothing

            wxFont font(m_fontWindow->GetTextFont());
            if (font.SetFaceName(newFaceName)) // change facename only
            {
                wxASSERT_MSG(font.IsOk(), "The font should now be valid");
                DoChangeFont(font);
            }
            else
            {
                wxASSERT_MSG(!font.IsOk(), "The font should now be invalid");
                wxMessageBox("There is no font with such face name...",
                             "Invalid face name", wxOK | wxICON_ERROR, this);
            }
        }

        void OnSetEncoding(wxCommandEvent& event)
        {
            wxFontEncoding enc = GetEncodingFromUser();
            if (enc == wxFONTENCODING_SYSTEM)
                return;

            wxFont font = m_fontWindow->GetTextFont();
            font.SetEncoding(enc);
            DoChangeFont(font);
        }

        void OnPrivateFont(wxCommandEvent& event)
        {
            wxFont font(m_fontWindow->GetTextFont());
            if (font.SetFaceName("wxprivate"))
            {
                wxASSERT_MSG(font.IsOk(), "The font should now be valid");
                DoChangeFont(font);
            }
            else
            {
                wxLogError("Failed to use private font.");
            }
        }

        void OnFontPanelApply(wxCommandEvent& WXUNUSED(event))
        {
            DoChangeFont(m_fontWindow->MakeNewFont());
        }

    protected:
        bool DoEnumerateFamilies(bool fixedWidthOnly, wxFontEncoding encoding = wxFONTENCODING_SYSTEM,
                                 bool silent = false)
        {
            MyFontEnumerator fontEnumerator;

            fontEnumerator.EnumerateFacenames(encoding, fixedWidthOnly);

            if (fontEnumerator.GotAny())
            {
                int nFacenames = fontEnumerator.GetFacenames().GetCount();
                if (!silent)
                {
                    wxLogStatus(this, "Found %d %sfonts",
                                nFacenames, fixedWidthOnly ? "fixed width " : "");
                }

                wxString facename;

                if (silent)
                {
                    // choose the first
                    facename = fontEnumerator.GetFacenames().Item(0);
                }
                else
                {
                    // let the user choose
                    wxString* facenames = new wxString[nFacenames];
                    int n;
                    for (n = 0; n < nFacenames; n++)
                        facenames[n] = fontEnumerator.GetFacenames().Item(n);

                    n = wxGetSingleChoiceIndex
                    (
                        "Choose a facename",
                        GetSampleTitle(),
                        nFacenames,
                        facenames,
                        this
                    );

                    if (n != -1)
                        facename = facenames[n];

                    delete[] facenames;
                }

                if (!facename.empty())
                {
                    wxFont font(wxFontInfo().FaceName(facename).Encoding(encoding));

                    DoChangeFont(font);
                }

                return true;
            }
            if (!silent)
            {
                wxLogWarning("No such fonts found.");
            }

            return false;
        }

        void DoResizeFont(int diff)
        {
            wxFont font = m_fontWindow->GetTextFont();

            font.SetPointSize(font.GetPointSize() + diff);
            DoChangeFont(font);
        }

        void DoChangeFont(const wxFont& font, const wxColour& col = wxNullColour)
        {
            m_fontWindow->UpdateFont(font, col);

            m_textctrl->SetFont(font);
            if (col.IsOk())
                m_textctrl->SetForegroundColour(col);
            m_textctrl->Refresh();

            // update the state of the bold/italic/underlined menu items
            wxMenuBar* mbar = GetMenuBar();
            if (mbar)
            {
                mbar->Check(Font_Light, font.GetWeight() == wxFONTWEIGHT_LIGHT);
                mbar->Check(Font_Bold, font.GetWeight() == wxFONTWEIGHT_BOLD);

                mbar->Check(Font_Italic, font.GetStyle() == wxFONTSTYLE_ITALIC);
#ifndef __WXMSW__
				mbar->Check(Font_Slant, font.GetStyle() == wxFONTSTYLE_SLANT);
#endif

                mbar->Check(Font_Underlined, font.GetUnderlined());
                mbar->Check(Font_Strikethrough, font.GetStrikethrough());
            }
        }

        // ask the user to choose an encoding and return it or
        // wxFONTENCODING_SYSTEM if the dialog was cancelled
        wxFontEncoding GetEncodingFromUser()
        {
            wxArrayString names;
            wxArrayInt encodings;

            const size_t count = wxFontMapper::GetSupportedEncodingsCount();
            names.reserve(count);
            encodings.reserve(count);

            for (size_t n = 0; n < count; n++)
            {
                wxFontEncoding enc = wxFontMapper::GetEncoding(n);
                encodings.push_back(enc);
                names.push_back(wxFontMapper::GetEncodingName(enc));
            }

            int i = wxGetSingleChoiceIndex
            (
                "Choose the encoding",
                GetSampleTitle(),
                names,
                this
            );

            return i == -1 ? wxFONTENCODING_SYSTEM : static_cast<wxFontEncoding>(encodings[i]);
        }

        // ask the user to choose a font family and return it or
        // wxFONTFAMILY_DEFAULT if the dialog was cancelled
        wxFontFamily GetFamilyFromUser()
        {
            wxArrayString names;
            wxArrayInt families;

            families.push_back(wxFONTFAMILY_DECORATIVE);
            families.push_back(wxFONTFAMILY_ROMAN);
            families.push_back(wxFONTFAMILY_SCRIPT);
            families.push_back(wxFONTFAMILY_SWISS);
            families.push_back(wxFONTFAMILY_MODERN);
            families.push_back(wxFONTFAMILY_TELETYPE);

            names.push_back("DECORATIVE");
            names.push_back("ROMAN");
            names.push_back("SCRIPT");
            names.push_back("SWISS");
            names.push_back("MODERN");
            names.push_back("TELETYPE");

            int i = wxGetSingleChoiceIndex
            (
                "Choose the family",
                GetSampleTitle(),
                names,
                this
            );

            return i == -1 ? wxFONTFAMILY_DEFAULT : static_cast<wxFontFamily>(families[i]);
        }

        wxTextCtrl* m_textctrl;
        FontWindow* m_fontWindow;

    private:
        // any class wishing to process wxWidgets events must use this macro
    wxDECLARE_EVENT_TABLE();
    };

    // the event tables connect the wxWidgets events with the functions (event
    // handlers) which process them. It can be also done at run-time, but for the
    // simple menu events like this the static method is much simpler.
    wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
        EVT_MENU(Font_Quit, MyFrame::OnQuit)
        EVT_MENU(Font_TestTextValue, MyFrame::OnTestTextValue)
        EVT_MENU(Font_ViewMsg, MyFrame::OnViewMsg)
        EVT_MENU(Font_About, MyFrame::OnAbout)

        EVT_MENU(Font_GetBaseFont, MyFrame::OnGetBaseFont)
        EVT_MENU(Font_IncSize, MyFrame::OnIncFont)
        EVT_MENU(Font_DecSize, MyFrame::OnDecFont)

        EVT_MENU(Font_Bold, MyFrame::OnBold)
        EVT_MENU(Font_Light, MyFrame::OnLight)

        EVT_MENU(Font_Italic, MyFrame::OnItalic)
        EVT_MENU(Font_Slant, MyFrame::OnSlant)

        EVT_MENU(Font_Underlined, MyFrame::OnUnderline)
        EVT_MENU(Font_Strikethrough, MyFrame::OnStrikethrough)

        EVT_MENU(Font_wxNORMAL_FONT, MyFrame::OnwxPointerFont)
        EVT_MENU(Font_wxSMALL_FONT, MyFrame::OnwxPointerFont)
        EVT_MENU(Font_wxITALIC_FONT, MyFrame::OnwxPointerFont)
        EVT_MENU(Font_wxSWISS_FONT, MyFrame::OnwxPointerFont)
        EVT_MENU(Font_wxFont_Default, MyFrame::OnFontDefault)

        EVT_MENU(Font_wxSYS_OEM_FIXED_FONT, MyFrame::OnwxSystemSettingsFont)
        EVT_MENU(Font_wxSYS_ANSI_FIXED_FONT, MyFrame::OnwxSystemSettingsFont)
        EVT_MENU(Font_wxSYS_ANSI_VAR_FONT, MyFrame::OnwxSystemSettingsFont)
        EVT_MENU(Font_wxSYS_SYSTEM_FONT, MyFrame::OnwxSystemSettingsFont)
        EVT_MENU(Font_wxSYS_DEVICE_DEFAULT_FONT, MyFrame::OnwxSystemSettingsFont)
        EVT_MENU(Font_wxSYS_DEFAULT_GUI_FONT, MyFrame::OnwxSystemSettingsFont)

        EVT_MENU(Font_SetNativeDesc, MyFrame::OnSetNativeDesc)
        EVT_MENU(Font_SetNativeUserDesc, MyFrame::OnSetNativeUserDesc)
        EVT_MENU(Font_SetFamily, MyFrame::OnSetFamily)
        EVT_MENU(Font_SetFaceName, MyFrame::OnSetFaceName)
        EVT_MENU(Font_SetEncoding, MyFrame::OnSetEncoding)

        EVT_MENU(Font_Choose, MyFrame::OnSelectFont)
        EVT_MENU(Font_EnumFamiliesForEncoding, MyFrame::OnEnumerateFamiliesForEncoding)
        EVT_MENU(Font_EnumFamilies, MyFrame::OnEnumerateFamilies)
        EVT_MENU(Font_EnumFixedFamilies, MyFrame::OnEnumerateFixedFamilies)
        EVT_MENU(Font_EnumEncodings, MyFrame::OnEnumerateEncodings)
        EVT_MENU(Font_Private, MyFrame::OnPrivateFont)
    wxEND_EVENT_TABLE()
}
