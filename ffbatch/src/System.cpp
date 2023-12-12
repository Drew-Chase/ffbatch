#include "System.h"
#include <Windows.h>
using namespace ffbatch;

void System::ShowConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
}

void System::HideConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

bool System::IsConsoleVisible()
{
	return ::IsWindowVisible(::GetConsoleWindow()) != FALSE;
}
