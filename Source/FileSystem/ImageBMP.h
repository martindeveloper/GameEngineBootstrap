#pragma once

#include "../Headers.h"

namespace FileSystem
{
	class ImageBMP : Image
	{
	public:
		ImageBMP::ImageBMP(class File* sourceFile);

		unsigned __int32 GetSize() override;
		void* GetDataPointer() override;
		void Free() override;

		virtual ~ImageBMP();

	private:
		bool IsLoaded;
		int Size;
		void* DataPointer;
		class File* SourceFile;
	};
}