#include <Windows.h> // include Windows.h only if using WinMain

// Fl_Text_Editor example with color styles . -erco
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Output.H>


int __stdcall WinMain(
    __in HINSTANCE hInstance,
    __in_opt HINSTANCE hPrevInstance,
    __in LPSTR lpCmdLine,
    __in int nShowCmd
)
{

    Fl_Window* win = new Fl_Window(1141, 854);
    
    win->position(400, 100);

    Fl_Text_Editor* buildSpecEdior = new Fl_Text_Editor(30, 25, 1075, 470, "BuildSpec");
    Fl_Text_Buffer* tbuff = new Fl_Text_Buffer();      // text buffer
    Fl_Text_Buffer* sbuff = new Fl_Text_Buffer();      // style buffer

    buildSpecEdior->buffer(tbuff);

    // Text
    tbuff->text("");

    Fl_Button* ruNButton = new Fl_Button(525, 505, 85, 25, "Run Build");
    
    Fl_Text_Display* buildLogs = new Fl_Text_Display(30, 550, 1075, 265, "Build Logs");

    Fl_Output* statusBar = new Fl_Output(0, 830, 1140, 25, "StatusBar");
    
    //win->resizable(*disp);
    win->show();

    return(Fl::run());
}
