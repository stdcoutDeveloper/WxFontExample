#pragma once

#include "MyFrame.h"

namespace WxFont
{
    // Define a new application type, each program should derive a class from wxApp
    class MyApp : public wxApp
    {
    public:
        // override base class virtuals
        // ----------------------------
        // this one is called on application startup and is a good place for the app
        // initialization (doing it here and not in the ctor allows to have an error
        // return: if OnInit() returns false, the application terminates)
        // `Main program' equivalent: the program execution "starts" here
        bool OnInit() wxOVERRIDE
        {
            if (!wxApp::OnInit())
                return false;

            // Create the main application window
            MyFrame* frame = new MyFrame();

            // Show it
            frame->Show(true);

            // success: wxApp::OnRun() will be called which will enter the main message
            // loop and the application will run. If we returned 'false' here, the
            // application would exit immediately.
            return true;
        }
    };
}
