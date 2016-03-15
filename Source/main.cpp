#include "Common.h"

#include "Renderer/OpenGL4/OpenGL4Renderer.h"
#include "Renderer/D3D11/D3D11Renderer.h"

struct WindowProperties
{
	// Win32
	HINSTANCE hInstance;
	HINSTANCE hPrevInstance;
	LPSTR lpCmdLine;
	int nShowCmd;

	// Custom
	unsigned __int32 Width;
	unsigned __int32 Height;
	LPCSTR Title;
	bool IsFullscreen;
	Renderer::IWindowRenderer* Renderer;
};

// Prototypes
int CreateWindowWithRenderer(WindowProperties properties, Renderer::IWindowRenderer* renderer);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	WindowProperties windowProperties;
	ZeroMemory(&windowProperties, sizeof(windowProperties));

	windowProperties.hInstance = hInstance;
	windowProperties.hPrevInstance = hPrevInstance;
	windowProperties.lpCmdLine = lpCmdLine;
	windowProperties.nShowCmd = nShowCmd;

	windowProperties.Width = 1024;
	windowProperties.Height = 768;
	windowProperties.Title = "OpenGL Window";
	windowProperties.IsFullscreen = false;
	windowProperties.Renderer = new Renderer::D3D11Renderer();

	int result = CreateWindowWithRenderer(windowProperties, windowProperties.Renderer);

	return result;
}

int CreateWindowWithRenderer(WindowProperties properties, Renderer::IWindowRenderer* renderer)
{
	WindowBootstrap::Window* Window = new WindowBootstrap::Window(properties.hInstance, properties.hPrevInstance, properties.lpCmdLine, properties.nShowCmd);
	Window->SetRenderer(renderer);

	int result = Window->Create(properties.Width, properties.Height, properties.Title, properties.IsFullscreen);

	delete Window;

	return result;
}