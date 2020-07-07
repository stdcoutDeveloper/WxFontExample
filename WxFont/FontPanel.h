#pragma once

#include "pch.h"
#include "wx/spinctrl.h"

namespace WxFont
{
    // Unlike wxFontXXX, the elements of these enum are consecutive, which is
    // more convenient here.
    enum Family
    {
        Family_Default,
        Family_Decorative,
        Family_Roman,
        Family_Script,
        Family_Swiss,
        Family_Modern,
        Family_Teletype
    };

    enum Style
    {
        Style_Normal,
        Style_Italic,
        Style_Slant
    };

    enum Weight
    {
        Weight_Thin,
        Weight_Extralight,
        Weight_Light,
        Weight_Normal,
        Weight_Medium,
        Weight_Semibold,
        Weight_Bold,
        Weight_Extrabold,
        Weight_Heavy,
        Weight_Extraheavy
    };

    // FontPanel contains controls allowing to specify the font properties
    class FontPanel : public wxPanel
    {
    public:
        explicit FontPanel(wxWindow* parent) : wxPanel(parent)
        {
            m_useFamily =
                m_useNumericWeight = false;

            m_textFaceName = new wxTextCtrl(this, wxID_ANY, wxString(),
                                            wxDefaultPosition, wxDefaultSize,
                                            wxTE_PROCESS_ENTER);
            m_textFaceName->Bind(wxEVT_TEXT, &FontPanel::OnFacename, this);

            // Must be in sync with the Family enum.
            const wxString familiesNames[] =
            {
                "Default",
                "Decorative",
                "Roman",
                "Script",
                "Swiss",
                "Modern",
                "Teletype",
            };
            m_choiceFamily = new wxChoice(this, wxID_ANY,
                                          wxDefaultPosition, wxDefaultSize,
                                          WXSIZEOF(familiesNames), familiesNames);
            m_choiceFamily->Bind(wxEVT_CHOICE, &FontPanel::OnFamily, this);

            m_spinPointSize = new wxSpinCtrlDouble(this, wxID_ANY, wxString(),
                                                   wxDefaultPosition, wxDefaultSize,
                                                   wxSP_ARROW_KEYS,
                                                   1.0, 100.0, 10.0, 0.1);

            m_spinPointSize->SetInitialSize
            (
                m_spinPointSize->GetSizeFromTextSize(GetTextExtent("999.9").x)
            );

            // Must be in sync with the Style enum.
            const wxString stylesNames[] =
            {
                "Normal",
                "Italic",
                "Slant",
            };

            m_choiceStyle = new wxChoice(this, wxID_ANY,
                                         wxDefaultPosition, wxDefaultSize,
                                         WXSIZEOF(stylesNames), stylesNames);

            // Must be in sync with the Weight enum.
            const wxString weightsNames[] =
            {
                "Thin",
                "Extra light",
                "Light",
                "Normal",
                "Medium",
                "Semi-bold",
                "Bold",
                "Extra bold",
                "Heavy",
                "Extra heavy",
            };

            m_choiceWeight = new wxChoice(this, wxID_ANY,
                                          wxDefaultPosition, wxDefaultSize,
                                          WXSIZEOF(weightsNames), weightsNames);
            m_choiceWeight->Bind(wxEVT_CHOICE, &FontPanel::OnWeightChoice, this);

            m_spinWeight = new wxSpinCtrl(this, wxID_ANY, wxString(),
                                          wxDefaultPosition, wxDefaultSize,
                                          wxSP_ARROW_KEYS,
                                          1, wxFONTWEIGHT_MAX);
            m_spinWeight->SetInitialSize
            (
                m_spinWeight->GetSizeFromTextSize(GetTextExtent("9999").x)
            );
            m_spinWeight->Bind(wxEVT_SPINCTRL, &FontPanel::OnWeightSpin, this);

            m_checkUnderlined = new wxCheckBox(this, wxID_ANY, wxString());
            m_checkStrikethrough = new wxCheckBox(this, wxID_ANY, wxString());
            m_checkFixedWidth = new wxCheckBox(this, wxID_ANY, wxString());
            m_checkFixedWidth->Disable(); // Can't be changed by the user.

            m_labelInfo = new wxStaticText(this, wxID_ANY, "\n\n\n");


            const int border = wxSizerFlags::GetDefaultBorder();

            // Columns are: label, control, gap, label, control, label, control (there
            // is no second gap column because we don't want any gap in the weight row).
            wxFlexGridSizer* const sizer = new wxFlexGridSizer(7, wxSize(border, border));

            const wxSizerFlags flagsLabel = wxSizerFlags().CentreVertical();
            const wxSizerFlags flagsValue = wxSizerFlags().Expand().CentreVertical();

            sizer->Add(new wxStaticText(this, wxID_ANY, "Face &name:"), flagsLabel);
            sizer->Add(m_textFaceName, flagsValue);

            sizer->AddSpacer(2 * border);

            sizer->Add(new wxStaticText(this, wxID_ANY, "&Family:"), flagsLabel);
            sizer->Add(m_choiceFamily, flagsValue);

            sizer->Add(new wxStaticText(this, wxID_ANY, "&Point size:"),
                       wxSizerFlags().DoubleBorder(wxLEFT).CentreVertical());
            sizer->Add(m_spinPointSize, flagsValue);


            sizer->Add(new wxStaticText(this, wxID_ANY, "&Style:"), flagsLabel);
            sizer->Add(m_choiceStyle, flagsValue);

            sizer->AddSpacer(0);

            sizer->Add(new wxStaticText(this, wxID_ANY, "&Weight:"), flagsLabel);
            sizer->Add(m_choiceWeight, flagsValue);

            sizer->Add(new wxStaticText(this, wxID_ANY, "or &raw value:"), flagsLabel);
            sizer->Add(m_spinWeight, flagsValue);


            sizer->Add(new wxStaticText(this, wxID_ANY, "&Underlined:"), flagsLabel);
            sizer->Add(m_checkUnderlined, flagsValue);

            sizer->AddSpacer(0);

            sizer->Add(new wxStaticText(this, wxID_ANY, "&Strike through:"), flagsLabel);
            sizer->Add(m_checkStrikethrough, flagsValue);

            sizer->Add(new wxStaticText(this, wxID_ANY, "Fixed width:"), flagsLabel);
            sizer->Add(m_checkFixedWidth, flagsValue);

            wxSizer* const sizerTop = new wxBoxSizer(wxVERTICAL);
            sizerTop->Add(sizer, wxSizerFlags().Expand().Border(wxBOTTOM));
            sizerTop->Add(new wxButton(this, wxID_APPLY, "&Apply changes"),
                          wxSizerFlags().Border(wxBOTTOM).Centre());
            sizerTop->Add(m_labelInfo, wxSizerFlags().Expand().Border(wxTOP));
            SetSizer(sizerTop);

            ShowFont(*wxNORMAL_FONT);
        }

        void ShowFont(const wxFont& font)
        {
            m_font = font;
            DoUpdate();
        }

        wxFontInfo GetFontInfo() const
        {
            wxFontInfo info(m_spinPointSize->GetValue());

            if (m_useFamily)
            {
                const wxFontFamily families[] =
                {
                    wxFONTFAMILY_DEFAULT,
                    wxFONTFAMILY_DECORATIVE,
                    wxFONTFAMILY_ROMAN,
                    wxFONTFAMILY_SCRIPT,
                    wxFONTFAMILY_SWISS,
                    wxFONTFAMILY_MODERN,
                    wxFONTFAMILY_TELETYPE,
                };
                info.Family(families[m_choiceFamily->GetSelection()]);
            }
            else
            {
                info.FaceName(m_textFaceName->GetValue());
            }

            switch (m_choiceStyle->GetSelection())
            {
            case Style_Normal:
                break;

            case Style_Italic:
                info.Italic();
                break;

            case Style_Slant:
                info.Slant();
                break;
            }

            info.Weight(m_useNumericWeight
                            ? m_spinWeight->GetValue()
                            : (m_choiceWeight->GetSelection() + 1) * 100);

            if (m_checkUnderlined->GetValue())
                info.Underlined();
            if (m_checkStrikethrough->GetValue())
                info.Strikethrough();

            return info;
        }

    private:
        // Update m_useXXX flags depending on which control was changed last.
        void OnFacename(wxCommandEvent& e)
        {
            m_useFamily = false;
            e.Skip();
        }

        void OnFamily(wxCommandEvent& e)
        {
            m_useFamily = true;
            e.Skip();
        }

        void OnWeightChoice(wxCommandEvent& e)
        {
            m_useNumericWeight = false;
            e.Skip();
        }

        void OnWeightSpin(wxCommandEvent& e)
        {
            m_useNumericWeight = true;
            e.Skip();
        }

        void DoUpdate()
        {
            m_textFaceName->ChangeValue(m_font.GetFaceName());

            Family family = Family_Default;
            switch (m_font.GetFamily())
            {
            case wxFONTFAMILY_DECORATIVE: family = Family_Decorative;
                break;
            case wxFONTFAMILY_ROMAN: family = Family_Roman;
                break;
            case wxFONTFAMILY_SCRIPT: family = Family_Script;
                break;
            case wxFONTFAMILY_SWISS: family = Family_Swiss;
                break;
            case wxFONTFAMILY_MODERN: family = Family_Modern;
                break;
            case wxFONTFAMILY_TELETYPE: family = Family_Teletype;
                break;

            case wxFONTFAMILY_DEFAULT:
            case wxFONTFAMILY_UNKNOWN:
                // Leave family as Family_Default, what else can we do.
                break;
            }
            m_choiceFamily->SetSelection(family);

            m_spinPointSize->SetValue(m_font.GetFractionalPointSize());

            Style style = Style_Normal;
            switch (m_font.GetStyle())
            {
            case wxFONTSTYLE_ITALIC: style = Style_Italic;
                break;
            case wxFONTSTYLE_SLANT: style = Style_Slant;
                break;

            case wxFONTSTYLE_NORMAL:
            case wxFONTSTYLE_MAX:
                break;
            }
            m_choiceStyle->SetSelection(style);

            Weight weight = Weight_Normal;
            switch (m_font.GetWeight())
            {
            case wxFONTWEIGHT_THIN: weight = Weight_Thin;
                break;
            case wxFONTWEIGHT_EXTRALIGHT: weight = Weight_Extralight;
                break;
            case wxFONTWEIGHT_LIGHT: weight = Weight_Light;
                break;
            case wxFONTWEIGHT_MEDIUM: weight = Weight_Medium;
                break;
            case wxFONTWEIGHT_SEMIBOLD: weight = Weight_Semibold;
                break;
            case wxFONTWEIGHT_BOLD: weight = Weight_Bold;
                break;
            case wxFONTWEIGHT_EXTRABOLD: weight = Weight_Extrabold;
                break;
            case wxFONTWEIGHT_HEAVY: weight = Weight_Heavy;
                break;
            case wxFONTWEIGHT_EXTRAHEAVY: weight = Weight_Extraheavy;
                break;

            case wxFONTWEIGHT_NORMAL:
            case wxFONTWEIGHT_INVALID:
                break;
            }
            m_choiceWeight->SetSelection(weight);
            m_spinWeight->SetValue(m_font.GetNumericWeight());

            m_checkUnderlined->SetValue(m_font.GetUnderlined());
            m_checkStrikethrough->SetValue(m_font.GetStrikethrough());
            m_checkFixedWidth->SetValue(m_font.IsFixedWidth());

            const wxSize pixelSize = m_font.GetPixelSize();
            wxClientDC dc(this);
            dc.SetFont(m_font);

            m_labelInfo->SetLabelText
            (
                wxString::Format
                (
                    "Font info string: %s\n"
                    "Size in pixels: %d*%d, "
                    "average char size: %d*%d",
                    m_font.GetNativeFontInfoDesc(),
                    pixelSize.x, pixelSize.y,
                    dc.GetCharWidth(), dc.GetCharHeight()
                )
            );
        }

        wxFont m_font;

        wxTextCtrl* m_textFaceName;
        wxChoice* m_choiceFamily;
        wxSpinCtrlDouble* m_spinPointSize;
        wxChoice* m_choiceStyle;
        wxChoice* m_choiceWeight;
        wxSpinCtrl* m_spinWeight;
        wxCheckBox* m_checkUnderlined;
        wxCheckBox* m_checkStrikethrough;
        wxCheckBox* m_checkFixedWidth;
        wxStaticText* m_labelInfo;

        bool m_useFamily;
        bool m_useNumericWeight;
    };
}
