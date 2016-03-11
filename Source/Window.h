#pragma once

#include "Common.h"

namespace WindowBootstrap {
	struct WindowParameters
	{
		int Width;
		int Height;
	};

	class Window
	{
	public:
		bool IsRenderingLoopRunning = true;

		Window(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
		~Window();
		int Create(const int Width, const int Height, LPCSTR Title, bool isFullscreen);
		void SetRenderer(Renderer::IWindowRenderer *NewRenderer);
		static LRESULT CALLBACK StaticMessageLoopCallback(HWND WindowHandle, UINT Message, WPARAM wParam, LPARAM lParam);

	private:
		Renderer::IWindowRenderer* Renderer = nullptr;

		HDC WindowDeviceContext;
		HWND WindowHandle;
		HINSTANCE ApplicationInstance;
		int ShowStyle;

		LRESULT CALLBACK MessageLoopCallback(HWND WindowHandle, UINT Message, WPARAM wParam, LPARAM lParam);

		unsigned __int64 TimeBase;
		unsigned __int64 TimeDelta;
		unsigned __int64 TimeFrequency;
	};
}