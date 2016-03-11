#pragma once

#include "../Common.h"

#include "Image.h"
#include "ImageHeaderBMP.h"
#include "../FileSystem/File.h"

namespace Image
{
	class ImageBMP : Image
	{
	public:
		ImageBMP::ImageBMP(FileSystem::File* sourceFile);
		virtual ~ImageBMP();

		unsigned __int32 GetSize() const override;
		const char* GetDataPointer() const override;

		virtual unsigned __int32 GetWidth() const;
		virtual unsigned __int32 GetHeight() const;

		bool Load() override;
		void Free() override;

	private:
		FileSystem::File* SourceFile;
		BitmapFileHeader Header;
		BitmapInfoHeader InfoHeader;
	};
}