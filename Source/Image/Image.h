#pragma once

#include "../Common.h"

namespace Image
{
	class Image : Core::IResource
	{
	public:
		virtual uint32 GetSize() const = 0;
		virtual const char* GetDataPointer() const = 0;

		virtual uint32 GetWidth() const = 0;
		virtual uint32 GetHeight() const = 0;

		virtual bool Load() = 0;
		virtual void Free() = 0;

	protected:
		bool IsLoaded;
		uint32 Size;
		const char* DataPointer;
		const char* FilePath;
	};
}