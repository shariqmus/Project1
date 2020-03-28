#ifdef _WIN32
#include <Windows.h> // include Windows.h only if using WinMain
#endif

// std
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <cstring>
#include <array>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <iostream>
#include <cstring>

// Fl_Text_Editor example with color styles . -erco
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Output.H>
#include <FL/fl_message.H>

Fl_Output* status = 0;

#ifdef _WIN32

std::string ExecCmd_Win(std::string pCmdArg)
// Execute a command and get the results in a CString.
// Synchronously launches a child process and waits up to 2 seconds for completion.
// Uses a pipe to get the output of the child process.
// Does not pipe to stdin of child process.
// Example usage:
//   CString str;
//   str = ExecCmd( "ping 127.0.0.1 -n 99 " );  // This ping command will be terminated early before the -n 99 completes.
//   str.Replace( "\x0d\x0d\x0a", "\x0d\x0a" ); // fixes some ugly non-standard line terminators created by ping.
//
//   str = ExecCmd( "java -version" ); // A more practical usage.
//
{
	// Handle Inheritance - to pipe child's stdout via pipes to parent, handles must be inherited.
	//   SECURITY_ATTRIBUTES.bInheritHandle must be TRUE
	//   CreateProcess parameter bInheritHandles must be TRUE;
	//   STARTUPINFO.dwFlags must have STARTF_USESTDHANDLES set.

	std::string strResult; // Contains result of cmdArg.

	HANDLE hChildStdoutRd; // Read-side, used in calls to ReadFile() to get child's stdout output.
	HANDLE hChildStdoutWr; // Write-side, given to child process using si struct.

	BOOL fSuccess;

	// Create security attributes to create pipe.
	SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES) };
	saAttr.bInheritHandle = TRUE; // Set the bInheritHandle flag so pipe handles are inherited by child process. Required.
	saAttr.lpSecurityDescriptor = NULL;

	// Create a pipe to get results from child's stdout.
	// I'll create only 1 because I don't need to pipe to the child's stdin.
	if (!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0))
	{
		return strResult;
	}

	STARTUPINFO si = { sizeof(STARTUPINFO) }; // specifies startup parameters for child process.

	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES; // STARTF_USESTDHANDLES is Required.
	si.hStdOutput = hChildStdoutWr; // Requires STARTF_USESTDHANDLES in dwFlags.
	si.hStdError = hChildStdoutWr; // Requires STARTF_USESTDHANDLES in dwFlags.
								   // si.hStdInput remains null.
	si.wShowWindow = SW_HIDE; // Prevents cmd window from flashing. Requires STARTF_USESHOWWINDOW in dwFlags.

	PROCESS_INFORMATION pi = { 0 };

	// Create the child process.
	fSuccess = CreateProcess(
		NULL,
		&pCmdArg[0],     // command line
		NULL,               // process security attributes
		NULL,               // primary thread security attributes
		TRUE,               // TRUE=handles are inherited. Required.
		CREATE_NEW_CONSOLE, // creation flags
		NULL,               // use parent's environment
		NULL,               // use parent's current directory
		&si,                // __in, STARTUPINFO pointer
		&pi);               // __out, receives PROCESS_INFORMATION

	if (!fSuccess)
	{
		return strResult;
	}

	// Wait until child processes exit. Don't wait forever.
	WaitForSingleObject(pi.hProcess, 2000);
	TerminateProcess(pi.hProcess, 0); // Kill process if it is still running. Tested using cmd "ping blah -n 99"

									  // Close the write end of the pipe before reading from the read end of the pipe.
	if (!CloseHandle(hChildStdoutWr))
	{
		return strResult;
	}

	// Read output from the child process.
	for (;;)
	{
		DWORD dwRead;
		CHAR chBuf[4096];

		// Read from pipe that is the standard output for child process.
		bool done = !ReadFile(hChildStdoutRd, chBuf, 4096, &dwRead, NULL) || dwRead == 0;

		if (done)
		{
			break;
		}

		// Append result to string.
		strResult += std::string(chBuf, dwRead);
	}

	// Close process and thread handles.
	CloseHandle(hChildStdoutRd);

	// CreateProcess docs specify that these must be closed.
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return strResult;
}

#endif

std::string run_command(std::string command)
{
	std::string response = "";

#ifdef _WIN32

	response = ExecCmd_Win(command);

#elif defined(__unix__) 

#endif

    return response;
}

void set_status(std::string status_text)
{
	status->value(status_text.c_str());
	status->redraw();
	Fl::flush();
}

void runBuild_cb(Fl_Widget* w, void* p) 
{  
	set_status("Running command...");

	std::string str = run_command("aws --version");

	set_status("Ready.");

	fl_message(str.c_str());
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
	status = new Fl_Output(0, 830, 1140, 25, "StatusBar");
    status->color(FL_LIGHT1); // background color
    status->value("Ready.");
    
    //win->resizable(*disp);
    win->show();

    return(Fl::run());
}
