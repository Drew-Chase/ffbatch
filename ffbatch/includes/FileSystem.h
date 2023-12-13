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
		static void Parse(string& path, bool& recursive, vector<string>& extensions);
		static vector<string> GetFilesInDirectory(string directory, vector<string> extension, bool recursive);
		static vector<string> GenerateCommands(string command, vector<string> inputs, vector<string> args, string input);
		static bool GetFFmpegExecutable(string& path);
	};
}