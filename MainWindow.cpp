#include <Windows.h> // include Windows.h only if using WinMain

// Fl_Text_Editor example with color styles . -erco
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Output.H>
#include <FL/fl_message.H>

void runBuild_cb(Fl_Widget*, void*) 
{  
    fl_message("runBuild_cb");
}

int __stdcall WinMain(
    __in HINSTANCE hInstance,
    __in_opt HINSTANCE hPrevInstance,
    __in LPSTR lpCmdLine,
    __in int nShowCmd
)
{
    Fl_Window* win = new Fl_Window(1141, 854, "CodeBuild BuildSpec Tester");
    
    win->position(400, 100);

    Fl_Text_Editor* buildSpecEdior = new Fl_Text_Editor(30, 25, 1075, 475, "BuildSpec");
    buildSpecEdior->align(FL_ALIGN_TOP | FL_ALIGN_LEFT);

    Fl_Text_Buffer* buff_buildSpecEdior = new Fl_Text_Buffer();      // text buffer
    
    buildSpecEdior->buffer(buff_buildSpecEdior);

    buff_buildSpecEdior->text("line 0\nline 1");

    Fl_Button* runBuild = new Fl_Button(525, 505, 85, 25, "Run Build");
    runBuild->callback(runBuild_cb);
    
    Fl_Text_Display* buildLogs = new Fl_Text_Display(30, 550, 1075, 265, "Build Logs");
    buildLogs->align(FL_ALIGN_TOP  | FL_ALIGN_LEFT);
    Fl_Text_Buffer* buff_buildLogs = new Fl_Text_Buffer();      // text buffer
    buildLogs->buffer(buff_buildLogs);
    buff_buildLogs->text("ced");

    // Create output to show scrollbar's value
    Fl_Output* status = new Fl_Output(0, 830, 1140, 25, "StatusBar");
    status->color(FL_LIGHT1); // background color
    status->value("Ready.");
    
    //win->resizable(*disp);
    win->show();

    return(Fl::run());
}
