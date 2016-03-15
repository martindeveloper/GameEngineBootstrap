#pragma once

namespace Core
{
	class IResource
	{
	public:
		virtual ~IResource() {};
		virtual bool Load() = 0;
		virtual void Free() = 0;
	};
}