#include "Headers.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	RendererBootstrap::IWindowRenderer *Renderer = new RendererBootstrap::OpenGLRenderer();

	RendererBootstrap::Window *Window = new RendererBootstrap::Window(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
	Window->SetRenderer(Renderer);

	int result = Window->Create(1024, 768, L"Cool OpenGL Window on Windows", false);

	delete Window;

	return result;
}