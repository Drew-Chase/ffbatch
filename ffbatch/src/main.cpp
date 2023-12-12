// LFInteractive LLC. (c) 2020-2024 All Rights Reserved
#include <iostream>
#include <vector>
#include "../FileSystem.h"
int main(int argc, char** argv)
{
	int code = system("where.exe ffmpeg > nul 2>&1");
	if (code != 0)
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
			string arg = argv[i];
			command += arg;
			args.emplace_back(arg);
			if (arg == "-i")
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