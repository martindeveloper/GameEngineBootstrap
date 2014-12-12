#include "Headers.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	OpenGLBootstrap::IWindowRenderer *Renderer = new OpenGLBootstrap::OpenGLRenderer();

	OpenGLBootstrap::Window *Window = new OpenGLBootstrap::Window(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
	Window->SetRenderer(Renderer);

	int result = Window->Create(1024, 768, L"Cool OpenGL Window on Windows");

	delete Window;

	return result;
}