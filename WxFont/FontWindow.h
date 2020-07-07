#pragma once

#include "FontPanel.h"
#include "FontCanvas.h"
#include "wx/statline.h"

namespace WxFont
{
    // FontWindow contains both FontPanel and FontCanvas
    class FontWindow : public wxWindow
    {
    public:
        explicit FontWindow(wxWindow* parent) : wxWindow(parent, wxID_ANY), m_panel(new FontPanel(this)),
                                                m_canvas(new FontCanvas(this))
        {
            wxSizer* const sizer = new wxBoxSizer(wxVERTICAL);
            sizer->Add(m_panel, wxSizerFlags().Expand().Border());
            sizer->Add(new wxStaticLine(this), wxSizerFlags().Expand());
            sizer->Add(m_canvas, wxSizerFlags(1).Expand());
            SetSizer(sizer);
        }

        const wxFont& GetTextFont() const
        {
            return m_canvas->GetTextFont();
        }

        const wxColour& GetColour() const
        {
            return m_canvas->GetColour();
        }

        wxFont MakeNewFont() const
        {
            return m_panel->GetFontInfo();
        }

        void UpdateFont(const wxFont& font, const wxColour& colour)
        {
            m_panel->ShowFont(font);

            m_canvas->SetTextFont(font);
            if (colour.IsOk())
                m_canvas->SetColour(colour);
            m_canvas->Refresh();
        }

    private:
        FontPanel* const m_panel;
        FontCanvas* const m_canvas;
    };
}
