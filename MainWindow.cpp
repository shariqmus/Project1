#include <Windows.h> // include Windows.h only if using WinMain

// Fl_Text_Editor example with color styles . -erco
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Output.H>
#include <FL/fl_message.H>

void runBuild_cb(Fl_Widget* w, void* p) 
{  

    fl_message("runBuild_cb");
}

void sampleBuildSpec_cb(Fl_Widget* w, void* p)
{
    //fl_message("runBuild_cb");

    //Fl_Text_Buffer* buff_buildSpecEdior = new Fl_Text_Buffer();      // text buffer    
    //buff_buildSpecEdior->text("yaml");

    Fl_Text_Buffer* buffer = (Fl_Text_Buffer*) p;
    buffer->text("version: 0.2\n\nphases:\n  install:\n    runtime-versions:\n      nodejs: 10\n    commands:\n      - npm install\n  build:\n    commands:\n      - npm test\nartifacts:\n  files:\n    - '**/*'\n");
}

int __stdcall WinMain(
    __in HINSTANCE hInstance,
    __in_opt HINSTANCE hPrevInstance,
    __in LPSTR lpCmdLine,
    __in int nShowCmd
)
{
    // Main Window ----------------------------------------------------    
    Fl_Window* win = new Fl_Window(1141, 854, "CodeBuild BuildSpec Tester");    
    win->position(400, 100);

    // BuildSpec Editor ----------------------------------------------------
    Fl_Text_Editor* buildSpecEdior = new Fl_Text_Editor(30, 25, 1075, 475, "BuildSpec");
    buildSpecEdior->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);
    buildSpecEdior->textfont(FL_COURIER);
    buildSpecEdior->textsize(16);
    Fl_Text_Buffer* buff_buildSpecEdior = new Fl_Text_Buffer();      // text buffer    
    buildSpecEdior->buffer(buff_buildSpecEdior);
    //buff_buildSpecEdior->text("line 0\nline 1");

    // RunBuild button ----------------------------------------------------
    Fl_Button* runBuild = new Fl_Button(525, 505, 85, 25, "Run Build");
    runBuild->callback(runBuild_cb);
    
    // SampleBuildSpec button ----------------------------------------------------
    Fl_Button* sampleBuildSpec = new Fl_Button(410, 505, 100, 25, "Sample BSpec");
    sampleBuildSpec->callback(sampleBuildSpec_cb, buff_buildSpecEdior);

    // Build Log display ----------------------------------------------------
    Fl_Text_Display* buildLogs = new Fl_Text_Display(30, 550, 1075, 265, "Build Logs");
    buildLogs->align(FL_ALIGN_TOP  | FL_ALIGN_LEFT);
    //buildLogs->textfont(FL_SCREEN);
    buildLogs->textsize(16);
    Fl_Text_Buffer* buff_buildLogs = new Fl_Text_Buffer();      // text buffer
    buildLogs->buffer(buff_buildLogs);
    buff_buildLogs->text("[no logs]");

    // Status Bar ----------------------------------------------------
    Fl_Output* status = new Fl_Output(0, 830, 1140, 25, "StatusBar");
    status->color(FL_LIGHT1); // background color
    status->value("Ready.");
    
    //win->resizable(*disp);
    win->show();

    return(Fl::run());
}
