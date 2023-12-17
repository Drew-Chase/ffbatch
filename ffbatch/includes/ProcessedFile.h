// LFInteractive LLC. (c) 2020-2024 All Rights Reserved
#pragma once
#include <string>
using std::string;
namespace ffbatch
{
	struct ProcessedFile
	{
		string input;
		string output;
		string command;
		bool overwrite = false;
		bool success = false;
	};
};