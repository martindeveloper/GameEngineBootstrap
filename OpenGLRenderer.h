#pragma once
#include "Headers.h"

namespace OpenGLBootstrap{
	class OpenGLRenderer : public IWindowRenderer
	{
	public:
		OpenGLRenderer();
		void BeforeStart(HDC WindowDeviceContext) override;
		void ClearWindow() override;
		void Render() override;
		virtual ~OpenGLRenderer();
	};
}