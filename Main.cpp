#include <Windows.h> // include Windows.h only if using WinMain

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>


// Use standard main to have console background:
// int main()

// Use WinMain if you don't want the console in the background:

int __stdcall WinMain(
__in HINSTANCE hInstance,
__in_opt HINSTANCE hPrevInstance,
__in LPSTR lpCmdLine,
__in int nShowCmd
)
{
	Fl_Window* window = new Fl_Window(800, 600);
	Fl_Box* box = new Fl_Box(20, 40, 300, 100, "Hello, World!");

	box->box(FL_UP_BOX);
	box->labelfont(FL_BOLD + FL_ITALIC);
	box->labelsize(36);
	box->labeltype(FL_SHADOW_LABEL);

	window->end();
	window->show();
	return Fl::run();
}
