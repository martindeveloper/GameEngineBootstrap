#include "Window.h"

using namespace WindowBootstrap;

static Window* WindowInstanceSingleton = nullptr;

Window::Window(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WindowInstanceSingleton = this;

	ApplicationInstance = hInstance;
	ShowStyle = SW_SHOW;

	QueryPerformanceFrequency((LARGE_INTEGER*)&TimeFrequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&TimeBase);
}

int Window::Create(const int Width, const int Height, LPCWSTR Title, bool isFullscreen)
{
	WNDCLASSEX WindowClassHandle;
	ZeroMemory(&WindowClassHandle, sizeof(WNDCLASSEX));

	LPCWSTR windowClassName = TEXT("GameEngineBootstrap");

	WindowClassHandle.cbSize = sizeof(WNDCLASSEX);
	WindowClassHandle.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	WindowClassHandle.lpfnWndProc = Window::StaticMessageLoopCallback;
	WindowClassHandle.hInstance = ApplicationInstance;
	WindowClassHandle.hCursor = LoadCursor(NULL, IDC_ARROW);
	WindowClassHandle.hbrBackground = (HBRUSH)COLOR_WINDOW;
	WindowClassHandle.lpszClassName = windowClassName;

	RegisterClassEx(&WindowClassHandle);

	int WindowX = (GetSystemMetrics(SM_CXSCREEN) / 2) - (Width / 2);
	int WindowY = (GetSystemMetrics(SM_CYSCREEN) / 2) - (Height / 2);

	DWORD windowExtendedStyle;
	DWORD windowStyle;

	windowExtendedStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	windowStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

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
			OutputDebugString(TEXT("Can not create fullscreen window!"));
			DebugBreak();

			return ERROR_INVALID_FUNCTION;
		}

		windowExtendedStyle = WS_EX_APPWINDOW;
		windowStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	}

	WindowHandle = CreateWindowEx(windowExtendedStyle, windowClassName, Title, windowStyle, WindowX, WindowY, Width, Height, NULL, NULL, ApplicationInstance, NULL);

	ShowWindow(WindowHandle, ShowStyle);

	Renderer->Parameters.Width = Width;
	Renderer->Parameters.Height = Height;

	Renderer->BeforeStart(WindowDeviceContext, !isFullscreen);

	// Windows Message loop
	MSG Message = { 0 };
	
	while (IsMainLoopRunning)
	{
		while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
		{
			switch (Message.message)
			{
			case WM_QUIT:
				IsMainLoopRunning = false;
				break;
			case WM_MOUSEMOVE:
				POINTS points = MAKEPOINTS(Message.lParam);
				break;
			}

			TranslateMessage(&Message);
			DispatchMessageA(&Message);
		}

		try
		{
			unsigned __int64 time;
			QueryPerformanceCounter((LARGE_INTEGER*)&time);

			double deltaTime = (double)(time - TimeBase)*(1.0 / (double)TimeFrequency);

			Renderer->ClearWindow(deltaTime);

			Renderer->Update(deltaTime);
			Renderer->Render(deltaTime);
		}
		catch (std::exception* e)
		{
			OutputDebugStringA(e->what());
			IsMainLoopRunning = false;
			break;
		}
	}

	return 0;
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
		IsMainLoopRunning = false;
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

void Window::SetRenderer(Renderer::IWindowRenderer *NewRenderer)
{
	if(Renderer == nullptr)
	{
		delete Renderer;
	}

	Renderer = NewRenderer;
}

Window::~Window()
{
	delete Renderer;
}