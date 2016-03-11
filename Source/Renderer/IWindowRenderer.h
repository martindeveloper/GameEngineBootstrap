#pragma once

#include "../Common.h"

namespace Renderer {
	struct RendererParameters
	{
		int Width;
		int Height;
	};

	class IWindowRenderer
	{
	public:
		virtual ~IWindowRenderer() {};

		RendererParameters Parameters;

		virtual void BeforeStart(HDC WindowDeviceContext, const bool isWindowed) = 0;
		virtual void ClearWindow(const double deltaTime) = 0;
		virtual void Update(const double deltaTime) = 0;
		virtual void Render(const double deltaTime) = 0;
	};
}