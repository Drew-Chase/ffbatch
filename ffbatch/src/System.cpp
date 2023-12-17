// LFInteractive LLC. (c) 2020-2024 All Rights Reserved
#include "System.h"
#include <Windows.h>

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

using namespace ffbatch;

void System::ShowConsole()
{
	// Call the ShowWindow function from the Windows API
	// GetConsoleWindow() retrieves a handle to the window used by the console associated with the calling process
	// SW_SHOW is a command to make the window visible if it was previously hidden
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
}

void System::HideConsole()
{
	// The ShowWindow function changes the state of the specified window.
	// The GetConsoleWindow function retrieves a handle to the window used by the console associated with the calling process.
	// SW_HIDE is a command to hide the window.
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

bool System::IsConsoleVisible()
{
	// The IsWindowVisible function retrieves the visibility state of the specified window.
	// The GetConsoleWindow function retrieves a handle to the window used by the console associated with the calling process.
	// The result is compared with FALSE to return a boolean value.
	return ::IsWindowVisible(::GetConsoleWindow()) != FALSE;
}

string System::GetCommandOutput(string command)
{
	// Declare a buffer of 128 characters to store the output of the command
	std::array<char, 128> buffer;
	// Declare a string to store the final result of the command
	std::string result;
	// Declare a unique_ptr to manage the FILE pointer returned by _popen
	// _popen is used to create a pipe and execute the command
	// The second parameter to the unique_ptr is a custom deleter, in this case _pclose, which is used to close the pipe
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(command.c_str(), "r"), _pclose);
	// If the pipe was not successfully created, throw a runtime error
	if (!pipe)
	{
		throw std::runtime_error("popen() failed!");
	}
	// Read the output of the command line by line into the buffer
	// fgets reads up to 128 characters (size of the buffer) from the pipe into the buffer
	// This loop continues until fgets returns nullptr, which means it has reached the end of the output
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
	{
		// Append the contents of the buffer to the result string
		result += buffer.data();
	}
	// Return the result string, which contains the output of the command
	return result;
}