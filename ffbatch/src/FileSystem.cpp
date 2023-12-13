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

vector<string> FileSystem::GenerateCommands(string command, vector<string> files, vector<string> args, string input_string)
{
	vector<string> commands;

	for (const string& input : files)
	{
		string cmd = command;
		std::filesystem::path file(input);
		string filename = file.filename().string();
		string extension = file.extension().string();

		string search_string = input_string;
		size_t pos = cmd.find(search_string);
		while (pos != std::string::npos)
		{
			cmd = cmd.replace(pos, search_string.length(), "\"" + input + "\"");
			pos = cmd.find(search_string);
		}

		search_string = "%FILEPATH%";
		pos = cmd.find(search_string);
		while (pos != std::string::npos)
		{
			cmd = cmd.replace(pos, search_string.length(), file.parent_path().string());
			pos = cmd.find(search_string);
		}

		pos = filename.find(extension);
		while (pos != std::string::npos)
		{
			filename = filename.erase(pos, extension.length());
			pos = filename.find(extension);
		}

		search_string = "%FILENAME%";
		pos = cmd.find(search_string);
		while (pos != std::string::npos)
		{
			cmd = cmd.replace(pos, search_string.length(), filename + "_tmp");
			pos = cmd.find(search_string);
		}

		search_string = "%FILEEXTENSION%";
		pos = cmd.find(search_string);
		while (pos != std::string::npos)
		{
			cmd = cmd.replace(pos, search_string.length(), file.extension().string());
			pos = cmd.find(search_string);
		}

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

	return system("where.exe ffmpeg.exe > nul 2>&1") == 0;
}