#pragma once

#include "../Headers.h"

namespace Renderer {
	class IWindowRenderer
	{
	public:
		virtual void BeforeStart(HDC WindowDeviceContext, const bool isWindowed) = 0;
		virtual void ClearWindow(const double deltaTime) = 0;
		virtual void Update(const double deltaTime) = 0;
		virtual void Render(const double deltaTime) = 0;
	};
}