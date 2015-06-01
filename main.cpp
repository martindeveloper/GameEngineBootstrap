#include "Headers.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	RendererBootstrap::IWindowRenderer *Renderer = new RendererBootstrap::D3D11Renderer();

	RendererBootstrap::Window *Window = new RendererBootstrap::Window(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
	Window->SetRenderer(Renderer);

	int result = Window->Create(1920, 1080, L"Cool OpenGL Window on Windows", true);

	delete Window;

	return result;
}