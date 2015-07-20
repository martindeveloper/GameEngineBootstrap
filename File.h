#pragma once

#include "Headers.h"

namespace FileSystem
{
	class File
	{
	public:
		File(const char* path);
		std::string GetContent();

	private:
		const char* filePath;
	};
}