#include "HelloForm.h"
#include <Windows.h>

using namespace HelloWorld;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew HelloForm);
	return 0;
}