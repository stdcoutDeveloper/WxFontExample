#pragma once

#include "pch.h"

namespace WxFont
{
    // FontCanvas shows the font characters.
    class FontCanvas : public wxWindow
    {
    public:
        explicit FontCanvas(wxWindow* parent) : wxWindow(parent, wxID_ANY), m_colour(*wxRED), m_font(*wxNORMAL_FONT)
        {
        }

        // accessors for FontWindow
        const wxFont& GetTextFont() const
        {
            return m_font;
        }

        const wxColour& GetColour() const
        {
            return m_colour;
        }

        void SetTextFont(const wxFont& font)
        {
            m_font = font;
        }

        void SetColour(const wxColour& colour)
        {
            m_colour = colour;
        }

        // event handlers
        void OnPaint(wxPaintEvent& event)
        {
            wxPaintDC dc(this);
            PrepareDC(dc);

            // set background
            dc.SetBackground(*wxWHITE_BRUSH);
            dc.Clear();
            dc.SetFont(m_font);

            // the current text origin
            wxCoord x = 5,
                    y = 5;

            // prepare to draw the font
            dc.SetTextForeground(m_colour);

            // the size of one cell (Normally biggest char + small margin)
            wxCoord maxCharWidth, maxCharHeight;
            dc.GetTextExtent("W", &maxCharWidth, &maxCharHeight);
            int w = maxCharWidth + 5,
                h = maxCharHeight + 4;


            // print all font symbols from 32 to 256 in 7 rows of 32 chars each
            for (int i = 0; i < 7; i++)
            {
                for (int j = 0; j < 32; j++)
                {
                    wxChar c = static_cast<wxChar>(32 * (i + 1) + j);

                    wxCoord charWidth, charHeight;
                    dc.GetTextExtent(c, &charWidth, &charHeight);
                    dc.DrawText
                    (
                        c,
                        x + w * j + (maxCharWidth - charWidth) / 2 + 1,
                        y + h * i + (maxCharHeight - charHeight) / 2
                    );
                }
            }

            // draw the lines between them
            dc.SetPen(*wxBLUE_PEN);
            int l;

            // horizontal
            for (l = 0; l < 8; l++)
            {
                int yl = y + h * l - 2;
                dc.DrawLine(x - 2, yl, x + 32 * w - 1, yl);
            }

            // and vertical
            for (l = 0; l < 33; l++)
            {
                int xl = x + w * l - 2;
                dc.DrawLine(xl, y - 2, xl, y + 7 * h - 1);
            }
        }

    protected:
        wxSize DoGetBestClientSize() const wxOVERRIDE
        {
            return wxSize(80 * GetCharWidth(), 15 * GetCharHeight());
        }

    private:
        wxColour m_colour;
        wxFont m_font;

    wxDECLARE_EVENT_TABLE();
    };

    wxBEGIN_EVENT_TABLE(FontCanvas, wxWindow)
        EVT_PAINT(FontCanvas::OnPaint)
    wxEND_EVENT_TABLE()
}
