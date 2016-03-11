#pragma once

#include "../Common.h"

namespace FileSystem
{
	class File
	{
	public:
		File(const char* path);
		virtual ~File();
		std::string GetContent();
		std::vector<char> GetBinaryContent();
		std::vector<char>* GetBinaryContentPointer();
		bool IsExists();
		void Load();
		void Free();

	private:
		const char* filePath;
		std::vector<char> Buffer;
		std::ifstream DataStream;
	};
}