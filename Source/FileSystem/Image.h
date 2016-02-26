#pragma once

#include "../Headers.h"

namespace FileSystem
{
	class Image
	{
	public:
		virtual unsigned __int32 GetSize() = 0;
		virtual void* GetDataPointer() = 0;
		virtual void Free() = 0;

	private:
		bool IsLoaded;
		int Size;
		void* DataPointer;
		const char* FilePath;
	};
}