#include "ImageBMP.h"

using namespace FileSystem;

ImageBMP::ImageBMP(File* sourceFile)
{
	SourceFile = sourceFile;
};

unsigned __int32 ImageBMP::GetSize()
{
	return 0;
};

void* ImageBMP::GetDataPointer()
{
	return NULL;
};

void ImageBMP::Free()
{
	// TODO(martin.pernica): Remove loaded resources
	delete SourceFile;
};

ImageBMP::~ImageBMP()
{
	Free();
};