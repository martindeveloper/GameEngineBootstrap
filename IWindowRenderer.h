#pragma once

#include "Headers.h"

namespace RendererBootstrap {
	class IWindowRenderer
	{
	public:
		virtual void BeforeStart(HDC WindowDeviceContext, bool isWindowed) = 0;
		virtual void ClearWindow(double deltaTime) = 0;
		virtual void Update(double deltaTime) = 0;
		virtual void Render(double deltaTime) = 0;
	};
}