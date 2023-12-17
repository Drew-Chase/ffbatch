// LFInteractive LLC. (c) 2020-2024 All Rights Reserved
#pragma once
#include <string>
using std::string;
namespace ffbatch
{
	class System
	{
	public:
		/// <summary>
		/// Shows the console.
		/// </summary>
		static void ShowConsole();

		/// <summary>
		/// Hides the console.
		/// </summary>
		static void HideConsole();

		/// <summary>
		/// Checks if the console is visible.
		/// </summary>
		/// <returns>True if the console is visible, false otherwise.</returns>
		static bool IsConsoleVisible();

		/// <summary>
		/// Executes a command and gets its output.
		/// </summary>
		/// <param name="command">The command to execute.</param>
		/// <returns>The output of the command.</returns>
		static string GetCommandOutput(string command);
	};
}