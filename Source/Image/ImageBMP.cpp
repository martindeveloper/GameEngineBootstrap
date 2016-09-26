#include "ImageBMP.h"

using namespace Image;

ImageBMP::ImageBMP(FileSystem::File* sourceFile)
{
	SourceFile = sourceFile;
};

unsigned __int32 ImageBMP::GetSize() const
{
	return InfoHeader.biSizeImage;
};

const char* ImageBMP::GetDataPointer() const
{
	return DataPointer;
};

unsigned __int32 ImageBMP::GetWidth() const
{
	return InfoHeader.biWidth;
};

unsigned __int32 ImageBMP::GetHeight() const
{
	return InfoHeader.biHeight;
};

bool ImageBMP::Load()
{
	if (SourceFile->IsExists())
	{
		SourceFile->Load();

		std::vector<char>* dataVector = SourceFile->GetBinaryContentPointer();
		const char* rawData = dataVector->data();
		const bool isBMP = rawData[0] == 'B' && rawData[1] == 'M';

		if (isBMP)
		{
			Header = *(BitmapFileHeader*)(rawData);
			InfoHeader = *(BitmapInfoHeader*)(rawData + sizeof(BitmapFileHeader));

			DataPointer = reinterpret_cast<const char*>(rawData) + Header.bfOffBits;

			return true;
		}
	}

	return false;
};

void ImageBMP::Free()
{
	// TODO(martin.pernica): Remove loaded resources
	SourceFile->Free();
};

ImageBMP::~ImageBMP()
{
	Free();

	// NOTE(martin.pernica): Maybe delete source file after uploading to GPU as texture
	delete SourceFile;
};