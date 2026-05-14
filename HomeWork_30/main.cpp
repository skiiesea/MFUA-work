#include "CryptoForm.h"
#include <Windows.h>

using namespace CryptoSquareApp;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew CryptoForm());
    return 0;
}