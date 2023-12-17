// LFInteractive LLC. (c) 2020-2024 All Rights Reserved
#include "FileSystem.h"
#include "System.h"

#include <filesystem>
#include <iostream>
#include <regex>

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

using std::filesystem::directory_iterator;
using namespace ffbatch;

void FileSystem::Parse(string& path, bool& recursive, vector<string>& extensions)
{
	// Define a regular expression to match "**"
	std::regex recurseRegex("\\*\\*");

	// Define a regular expression to match "*.[extension]"
	// This will match any string that starts with "*" followed by a dot and then any alphanumeric characters
	std::regex extensionRegex("\\*\\.([A-Za-z0-9][^|]*([^\\|]|$)*)");

	// Define a regular expression to match any string that does not contain "**"
	std::regex pathRegex("((?!\\*\\*).)*");

	// Use the recurseRegex to search the path. If "**" is found, recursive is set to true
	recursive = regex_search(path, recurseRegex);

	// Create a temporary string from the path
	string tmp = path;

	// Declare a match object to hold the results of a regex search
	std::smatch match;

	// While there are matches for the extensionRegex in the tmp string
	while (std::regex_search(tmp, match, extensionRegex))
	{
		// Add the matched extension (without the leading "*") to the extensions vector
		extensions.push_back(match.str().substr(1));

		// Update tmp to be the part of the string after the match
		tmp = match.suffix();
	}

	// While there are matches for the pathRegex in the path string
	while (std::regex_search(path, match, pathRegex))
	{
		// Update path to be the matched string
		path = match[0];

		// Break after the first match
		break;
	}
}

vector<string> FileSystem::GetFilesInDirectory(string directory, vector<string> extension, bool recursive)
{
	// Declare a vector of strings to hold the file paths
	vector<string> files;

	// Iterate over each entry in the directory
	for (const auto& entry : directory_iterator(directory))
	{
		// If the entry is a directory and recursive search is enabled
		if (entry.is_directory() && recursive)
		{
			// Recursively get the files in the subdirectory
			vector<string> subFiles = GetFilesInDirectory(entry.path().string(), extension, recursive);
			
			// Insert the files from the subdirectory into the main files vector
			files.insert(files.end(), subFiles.begin(), subFiles.end());
		}
		// If the entry is a regular file
		else if (entry.is_regular_file())
		{
			// Get the path of the file as a string
			string path = entry.path().string();
			
			// Iterate over each extension in the extension vector
			for (const string& ext : extension)
			{
				// If the file has the current extension
				if (path.substr(path.length() - ext.length()) == ext)
				{
					// Add the file to the files vector
					files.push_back(path);
					
					// Break the loop as the file has been added
					break;
				}
			}
		}
	}

	// Return the vector of file paths
	return files;
}

vector<string> FileSystem::GenerateCommands(string command, vector<string> files, vector<string> args, string input_string)
{
	// Declare a vector of strings to hold the commands
	vector<string> commands;

	// Iterate over each file in the files vector
	for (const string& input : files)
	{
		// Initialize the command string with the command template
		string cmd = command;

		// Create a filesystem path object from the input file path
		std::filesystem::path file(input);

		// Get the filename (including extension) from the path
		string filename = file.filename().string();

		// Get the extension from the path
		string extension = file.extension().string();

		// Define a search string for the full file path placeholder
		string search_string = input_string;

		// Find the first occurrence of the search string in the command
		size_t pos = cmd.find(search_string);

		// While the search string is found in the command
		while (pos != std::string::npos)
		{
			// Replace the placeholder with the actual file path, enclosed in quotes
			cmd = cmd.replace(pos, search_string.length(), "\"" + input + "\"");

			// Find the next occurrence of the search string in the command
			pos = cmd.find(search_string);
		}

		// Define a search string for the file path without the filename placeholder
		search_string = "%FILEPATH%";

		// Find the first occurrence of the search string in the command
		pos = cmd.find(search_string);

		// While the search string is found in the command
		while (pos != std::string::npos)
		{
			// Replace the placeholder with the actual file path without the filename
			cmd = cmd.replace(pos, search_string.length(), file.parent_path().string());

			// Find the next occurrence of the search string in the command
			pos = cmd.find(search_string);
		}

		// Remove the extension from the filename
		pos = filename.find(extension);
		while (pos != std::string::npos)
		{
			filename = filename.erase(pos, extension.length());
			pos = filename.find(extension);
		}

		// Define a search string for the filename without extension placeholder
		search_string = "%FILENAME%";

		// Find the first occurrence of the search string in the command
		pos = cmd.find(search_string);

		// While the search string is found in the command
		while (pos != std::string::npos)
		{
			// Replace the placeholder with the actual filename without extension, followed by "_tmp"
			cmd = cmd.replace(pos, search_string.length(), filename + "_tmp");

			// Find the next occurrence of the search string in the command
			pos = cmd.find(search_string);
		}

		// Define a search string for the file extension placeholder
		search_string = "%FILEEXTENSION%";

		// Find the first occurrence of the search string in the command
		pos = cmd.find(search_string);

		// While the search string is found in the command
		while (pos != std::string::npos)
		{
			// Replace the placeholder with the actual file extension
			cmd = cmd.replace(pos, search_string.length(), file.extension().string());

			// Find the next occurrence of the search string in the command
			pos = cmd.find(search_string);
		}

		// Add the command to the commands vector
		commands.push_back(cmd);
	}

	// Return the vector of commands
	return commands;
}

bool FileSystem::GetFFmpegExecutable(string& path)
{
	// Iterate over each entry in the directory specified by the path
	for (const auto& entry : directory_iterator(path))
	{
		// If the entry is a regular file
		if (entry.is_regular_file())
		{
			// Get the filename of the entry
			auto filename = entry.path().filename().string();

			// If the filename is "ffmpeg.exe"
			if (strcmp(filename.c_str(), "ffmpeg.exe") == 0)
			{
				// Update the path to be the full path of the entry
				path = entry.path().string();

				// Return true, indicating that "ffmpeg.exe" was found
				return true;
			}
		}
	}

	// If "ffmpeg.exe" was not found in the directory, try to find it using the "where.exe" command
	path = System::GetCommandOutput("where.exe ffmpeg");

	// Remove the last character from the path (which is likely a newline character)
	path = path.substr(0, path.length() - 1);

	// Return whether the path is not empty, indicating that "ffmpeg.exe" was found
	return path.length() > 0;
}