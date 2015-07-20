#include "File.h"

using namespace FileSystem;

File::File(const char* path)
{
	filePath = path;
};

std::string File::GetContent()
{
	std::ifstream fileStream(filePath);
	std::string fileContent((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());

	return fileContent;
};