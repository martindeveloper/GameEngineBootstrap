#include "Window.h"

using namespace OpenGLBootstrap;

static Window *WindowInstanceSingleton = nullptr;

Window::Window(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WindowInstanceSingleton = this;

	ApplicationInstance = hInstance;
	ShowStyle = nShowCmd;
}

int Window::Create(const int Width, const int Height, wchar_t const *Title)
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

	WindowHandle = CreateWindowEx(NULL, L"WindowsOpenGL", Title, WS_OVERLAPPEDWINDOW, WindowX, WindowY, Width, Height, NULL, NULL, ApplicationInstance, NULL);

	ShowWindow(WindowHandle, ShowStyle);

	Renderer->BeforeStart(WindowDeviceContext);

	// Windows Message loop
	MSG Message;

	while (IsRenderingLoopRunning)
	{
		while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessageA(&Message);
		}

		try
		{
			Renderer->ClearWindow();
			Renderer->Render();

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