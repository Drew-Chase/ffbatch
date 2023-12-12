// LFInteractive LLC. (c) 2020-2024 All Rights Reserved
#include "FileSystem.h"
#include <filesystem>
#include <iostream>
#include <regex>
using std::filesystem::directory_iterator;
using namespace ffbatch;

void FileSystem::Parse(string& path, bool& recursive, vector<string>& extensions)
{
	std::regex recurseRegex("\\*\\*");
	std::regex extensionRegex("\\*\\.([A-Za-z0-9][^|]*([^\\|]|$)*)");
	std::regex pathRegex("((?!\\*\\*).)*");

	recursive = regex_search(path, recurseRegex);
	string tmp = path;
	std::smatch match;
	while (std::regex_search(tmp, match, extensionRegex))
	{
		extensions.push_back(match.str().substr(1));
		tmp = match.suffix();
	}

	while (std::regex_search(path, match, pathRegex))
	{
		path = match[0];
		break;
	}
}

vector<string> FileSystem::GetFilesInDirectory(string directory, vector<string> extension, bool recursive)
{
	vector<string> files;

	for (const auto& entry : directory_iterator(directory))
	{
		if (entry.is_directory() && recursive)
		{
			vector<string> subFiles = GetFilesInDirectory(entry.path().string(), extension, recursive);
			files.insert(files.end(), subFiles.begin(), subFiles.end());
		}
		else if (entry.is_regular_file())
		{
			string path = entry.path().string();
			for (const string& ext : extension)
			{
				if (path.substr(path.length() - ext.length()) == ext)
				{
					files.push_back(path);
					break;
				}
			}
		}
	}

	return files;
}

vector<string> FileSystem::GenerateCommands(string command, vector<string> inputs, vector<string> args)
{
	vector<string> commands;

	for (const string& input : inputs)
	{
		string cmd = command;
		cmd = std::regex_replace(cmd, std::regex("\\$input"), input);
		cmd = std::regex_replace(cmd, std::regex("\\$output"), input.substr(0, input.length() - 4) + "_out.mp4");
		commands.push_back(cmd);
	}

	return commands;
}

bool FileSystem::GetFFmpegExecutable(string& path)
{
	for (const auto& entry : directory_iterator(path))
	{
		if (entry.is_regular_file())
		{
			auto filename = entry.path().filename().string();
			if (strcmp(filename.c_str(), "ffmpeg.exe") == 0)
			{
				path = entry.path().string();
				return true;
			}
		}
	}

	return system("where.exe ffmpeg.exe > nul 1>&2") == 0;
}
