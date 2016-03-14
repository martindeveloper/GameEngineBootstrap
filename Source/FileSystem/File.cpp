#include "File.h"

using namespace FileSystem;

File::File(const char* path)
{
	filePath = path;

	DataStream = std::ifstream(filePath, std::ios::binary | std::ios::ate);
};

File::~File()
{
	Free();

	DataStream.close();
};

void File::Free()
{
	Buffer.clear();
};

void File::Load()
{
	std::streamsize size = DataStream.tellg();
	DataStream.seekg(0, std::ios::beg);

	Buffer.resize(size);
	DataStream.read(Buffer.data(), size);
};

std::string File::GetContent()
{
	std::string fileContent(Buffer.begin(), Buffer.end());

	return fileContent;
};

std::vector<char> File::GetBinaryContent()
{
	return Buffer;
};

std::vector<char>* File::GetBinaryContentPointer()
{
	return &Buffer;
};

bool File::IsLoaded()
{
	bool isLoaded = Buffer.size() != 0;
	return isLoaded;
};

bool File::IsExists()
{
	bool isExists = DataStream.good();
	return isExists;
};