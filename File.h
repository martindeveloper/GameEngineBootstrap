#pragma once

#include "Headers.h"

namespace FileSystem
{
	class File
	{
	public:
		File(const char* path);
		std::string GetContent();
		std::vector<char> GetBinaryContent();

	private:
		const char* filePath;
	};
}