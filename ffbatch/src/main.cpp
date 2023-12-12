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
	}
	else
	{
		std::vector<std::string> args;
		std::string input;
		string command;
		for (int i = 1; i < argc; i++)
		{
			char* arg = argv[i];
			command += arg;
			command += " ";
			args.emplace_back(arg);
			if (strcmp(arg, "-i") == 0 && i != argc - 1)
			{
				input = argv[i + 1];
			}
		}

		bool recursive = false;
		std::vector<std::string> extensions;
		FileSystem::Parse(input, recursive, extensions);
		std::cout << "Recursive: " << recursive << std::endl;
		vector<string> files = FileSystem::GetFilesInDirectory(input, extensions, recursive);
		for (const auto& ext : FileSystem::GenerateCommands(command, files, args))
		{
			std::cout << ext << std::endl;
		}
	}
	return 0;
}