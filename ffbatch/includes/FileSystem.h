// LFInteractive LLC. (c) 2020-2024 All Rights Reserved
#pragma once

#include <string>
#include <vector>
using std::vector;
using std::string;

namespace ffbatch
{
	class FileSystem
	{
	public:
		/// <summary>
		/// Parses the given path, sets the recursive flag and fills the extensions vector.
		/// </summary>
		/// <param name="path">The path to parse.</param>
		/// <param name="recursive">The recursive flag to set.</param>
		/// <param name="extensions">The extensions vector to fill.</param>
		static void Parse(string& path, bool& recursive, vector<string>& extensions);

		/// <summary>
		/// Gets the files in the given directory that match the given extensions.
		/// </summary>
		/// <param name="directory">The directory to search in.</param>
		/// <param name="extension">The extensions to match.</param>
		/// <param name="recursive">Whether to search recursively.</param>
		/// <returns>A vector of file paths.</returns>
		static vector<string> GetFilesInDirectory(string directory, vector<string> extension, bool recursive);

		/// <summary>
		/// Generates commands based on the given command, inputs, args and input.
		/// </summary>
		/// <param name="command">The command to use.</param>
		/// <param name="inputs">The inputs to use.</param>
		/// <param name="args">The arguments to use.</param>
		/// <param name="input">The input to use.</param>
		/// <returns>A vector of generated commands.</returns>
		static vector<string> GenerateCommands(string command, vector<string> inputs, vector<string> args, string input);

		/// <summary>
		/// Gets the FFmpeg executable path.
		/// </summary>
		/// <param name="path">The path to fill with the FFmpeg executable path.</param>
		/// <returns>Whether the FFmpeg executable was found.</returns>
		static bool GetFFmpegExecutable(string& path);
	};
}