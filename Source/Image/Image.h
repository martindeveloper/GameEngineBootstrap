#pragma once

#include "../Common.h"

namespace Image
{
	class Image
	{
	public:
		virtual unsigned __int32 GetSize() const = 0;
		virtual const char* GetDataPointer() const = 0;

		virtual unsigned __int32 GetWidth() const = 0;
		virtual unsigned __int32 GetHeight() const = 0;

		virtual bool Load() = 0;
		virtual void Free() = 0;

	protected:
		bool IsLoaded;
		unsigned __int32 Size;
		const char* DataPointer;
		const char* FilePath;
	};
}