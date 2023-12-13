// LFInteractive LLC. (c) 2020-2024 All Rights Reserved
#include <iostream>
#include <vector>
#include <filesystem>
#include <string>

#include "System.h"
#include "FileSystem.h"

using std::vector;
using std::string;
using std::filesystem::current_path;
using std::filesystem::path;
using ffbatch::FileSystem;

int main(int argc, char** argv)
{
	string executing_path = path(argv[0]).parent_path().string();
	if (!FileSystem::GetFFmpegExecutable(executing_path))
	{
		std::cerr << "ffmpeg.exe not found!" << std::endl;
		return 1;
	}
	std::vector<std::string> args;
	std::string input;
	string command;
	for (int i = 1; i < argc; i++)
	{
		char* arg = argv[i];
		if (strcmp(arg, "-i") == 0 && i != argc - 1)
		{
			input = argv[i + 1];
		}

		if (i == argc - 1)
		{
			command += "\"" + string(arg) + "\"";
		}
		else
		{
			command += arg;
			command += " ";
		}

		args.emplace_back(arg);
	}

	if (input.empty())
	{
		std::cerr << "No Input was found!" << std::endl;

		return 1;
	}

	string input_string = input;
	bool recursive = false;
	std::vector<std::string> extensions;
	FileSystem::Parse(input, recursive, extensions);
	vector<string> files = FileSystem::GetFilesInDirectory(input, extensions, recursive);
	for (const auto& ext : FileSystem::GenerateCommands(command, files, args, input_string))
	{
		std::cout << ext << std::endl;
	}
	return 0;
}