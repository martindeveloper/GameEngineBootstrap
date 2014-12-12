#pragma once

#include "Headers.h"

namespace OpenGLBootstrap {
	class Window
	{
	public:
		bool IsRenderingLoopRunning = true;

		Window(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
		~Window();
		int Create(const int Width, const int Height, wchar_t const *Title);
		void SetRenderer(IWindowRenderer *NewRenderer);
		static LRESULT CALLBACK StaticMessageLoopCallback(HWND WindowHandle, UINT Message, WPARAM wParam, LPARAM lParam);

	private:
		IWindowRenderer *Renderer;

		HDC WindowDeviceContext;
		HWND WindowHandle;
		HINSTANCE ApplicationInstance;
		int ShowStyle;

		LRESULT CALLBACK MessageLoopCallback(HWND WindowHandle, UINT Message, WPARAM wParam, LPARAM lParam);
	};
}