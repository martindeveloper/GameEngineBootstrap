#pragma once

#include "Headers.h"

namespace OpenGLBootstrap {
	class IWindowRenderer
	{
	public:
		virtual void BeforeStart(HDC WindowDeviceContext) = 0;
		virtual void ClearWindow() = 0;
		virtual void Render() = 0;
	};
}