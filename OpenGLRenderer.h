#pragma once
#include "Headers.h"

namespace RendererBootstrap{
	class OpenGLRenderer : public IWindowRenderer
	{
	public:
		OpenGLRenderer();
		void BeforeStart(HDC WindowDeviceContext, bool isWindowed) override;
		void ClearWindow(double deltaTime) override;
		void Render(double deltaTime) override;
		virtual ~OpenGLRenderer();
	};
}