#include "Window.h"

using namespace RendererBootstrap;

static Window *WindowInstanceSingleton = nullptr;

Window::Window(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WindowInstanceSingleton = this;

	ApplicationInstance = hInstance;
	ShowStyle = SW_SHOW;

	QueryPerformanceFrequency((LARGE_INTEGER*)&TimeFrequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&TimeBase);
}

int Window::Create(const int Width, const int Height, wchar_t const *Title, bool isFullscreen)
{
	WNDCLASSEX WindowClassHandle;
	ZeroMemory(&WindowClassHandle, sizeof(WNDCLASSEX));

	WindowClassHandle.cbSize = sizeof(WNDCLASSEX);
	WindowClassHandle.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	WindowClassHandle.lpfnWndProc = Window::StaticMessageLoopCallback;
	WindowClassHandle.hInstance = ApplicationInstance;
	WindowClassHandle.hCursor = LoadCursor(NULL, IDC_ARROW);
	WindowClassHandle.hbrBackground = (HBRUSH)COLOR_WINDOW;
	WindowClassHandle.lpszClassName = L"WindowsOpenGL";

	RegisterClassEx(&WindowClassHandle);

	int WindowX = (GetSystemMetrics(SM_CXSCREEN) / 2) - (Width / 2);
	int WindowY = (GetSystemMetrics(SM_CYSCREEN) / 2) - (Height / 2);

	DWORD windowExtendedStyle;
	DWORD windowStyle;

	windowExtendedStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	windowStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	RECT windowRect;
	windowRect.left = (long)0;
	windowRect.right = (long)Width;
	windowRect.top = (long)0;
	windowRect.bottom = (long)Height;

	// Fullscreen
	if(isFullscreen)
	{
		DEVMODE screenSettings;
		ZeroMemory(&screenSettings, sizeof(screenSettings));

		screenSettings.dmSize = sizeof(screenSettings);
		screenSettings.dmPelsWidth = Width;
		screenSettings.dmPelsHeight = Height;
		screenSettings.dmBitsPerPel = 32;
		screenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			OutputDebugString(L"Can not create fullscreen window!");
			DebugBreak();

			return ERROR_INVALID_FUNCTION;
		}

		windowExtendedStyle = WS_EX_APPWINDOW;
		windowStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	}

	//AdjustWindowRectEx(&windowRect, windowStyle, FALSE, windowExtendedStyle);

	WindowHandle = CreateWindowEx(windowExtendedStyle, L"WindowsOpenGL", Title, windowStyle, WindowX, WindowY, Width, Height, NULL, NULL, ApplicationInstance, NULL);

	ShowWindow(WindowHandle, ShowStyle);

	Renderer->BeforeStart(WindowDeviceContext, !isFullscreen);

	// Windows Message loop
	MSG Message = { 0 };
	
	while (IsRenderingLoopRunning)
	{
		while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessageA(&Message);
		}

		try
		{
			unsigned __int64 time;
			QueryPerformanceCounter((LARGE_INTEGER*)&time);

			double deltaTime = (double)(time - TimeBase)*(1.0 / (double)TimeFrequency);

			Renderer->ClearWindow(deltaTime);
			Renderer->Render(deltaTime);

			SwapBuffers(WindowDeviceContext);
		}
		catch (std::exception* e)
		{
			OutputDebugStringA(e->what());
			IsRenderingLoopRunning = false;
			break;
		}
	}

	return Message.wParam;
}

LRESULT CALLBACK Window::StaticMessageLoopCallback(HWND WindowHandle, UINT Message, WPARAM wParam, LPARAM lParam)
{
	return WindowInstanceSingleton->MessageLoopCallback(WindowHandle, Message, wParam, lParam);
}

LRESULT CALLBACK Window::MessageLoopCallback(HWND WindowHandle, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_CREATE:
		// Get window client area DC
		WindowDeviceContext = ::GetDC(WindowHandle);
		break;
	case WM_DESTROY:
		IsRenderingLoopRunning = false;
		::PostQuitMessage(0);
		break;

	case WM_TIMER:
		FLASHWINFO pfwi;
		ZeroMemory(&pfwi, sizeof(pfwi));

		pfwi.cbSize = sizeof(FLASHWINFO);
		pfwi.hwnd = WindowHandle;
		pfwi.uCount = 1;
		pfwi.dwFlags = FLASHW_ALL;
		pfwi.dwTimeout = 0;

		FlashWindowEx((PFLASHWINFO)&pfwi);
		break;

	default:
		return ::DefWindowProc(WindowHandle, Message, wParam, lParam);
		break;
	}

	return 0;
}

void Window::SetRenderer(IWindowRenderer *NewRenderer)
{
	delete Renderer;

	Renderer = NewRenderer;
}

Window::~Window()
{
	delete Renderer;
}