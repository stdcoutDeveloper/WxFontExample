#pragma once

#include "pch.h"
#include "wx/fontenum.h"

namespace WxFont
{
    class MyEncodingEnumerator : public wxFontEnumerator
    {
    public:
        MyEncodingEnumerator()
        {
            m_n = 0;
        }

        const wxString& GetText() const
        {
            return m_text;
        }

    protected:
        bool OnFontEncoding(const wxString& facename,
                            const wxString& encoding) wxOVERRIDE
        {
            wxString text;
            text.Printf("Encoding %u: %s (available in facename '%s')\n",
                        static_cast<unsigned int>(++m_n), encoding, facename);
            m_text += text;
            return true;
        }

    private:
        size_t m_n;
        wxString m_text;
    };

    class MyFontEnumerator : public wxFontEnumerator
    {
    public:
        bool GotAny() const
        {
            return !m_facenames.IsEmpty();
        }

        const wxArrayString& GetFacenames() const
        {
            return m_facenames;
        }

    protected:
        bool OnFacename(const wxString& facename) wxOVERRIDE
        {
            m_facenames.Add(facename);
            return true;
        }

    private:
        wxArrayString m_facenames;
    };
}
