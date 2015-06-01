#include "OpenGLRenderer.h"

using namespace RendererBootstrap;

OpenGLRenderer::OpenGLRenderer()
{
}

OpenGLRenderer::~OpenGLRenderer()
{
}

void OpenGLRenderer::BeforeStart(HDC WindowDeviceContext, bool isWindowed)
{
	// Set pixel format
	PIXELFORMATDESCRIPTOR OpenGLPixelFormatDescriptor;
	ZeroMemory(&OpenGLPixelFormatDescriptor, sizeof(PIXELFORMATDESCRIPTOR));

	OpenGLPixelFormatDescriptor.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	OpenGLPixelFormatDescriptor.nVersion = 1;
	OpenGLPixelFormatDescriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	OpenGLPixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
	OpenGLPixelFormatDescriptor.cColorBits = 32;
	OpenGLPixelFormatDescriptor.cDepthBits = 24;
	OpenGLPixelFormatDescriptor.cStencilBits = 8;
	OpenGLPixelFormatDescriptor.iLayerType = PFD_MAIN_PLANE;

	int WindowPixelFormat = ChoosePixelFormat(WindowDeviceContext, &OpenGLPixelFormatDescriptor);

	SetPixelFormat(WindowDeviceContext, WindowPixelFormat, &OpenGLPixelFormatDescriptor);

	// Create OpenGL context
	HGLRC OpenGLRenderingContextHandle = wglCreateContext(WindowDeviceContext);

	// Make OpenGL current
	if (!wglMakeCurrent(WindowDeviceContext, OpenGLRenderingContextHandle))
	{
		DWORD error = GetLastError();
		OutputDebugStringA("Can not make current OpenGL context");
		DebugBreak();
		exit(1);
	}

	// Init GLEW
	static const int glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();

	if (GLEW_OK != glewError)
	{
		OutputDebugStringA((char*)glewGetErrorString(glewError));
		DebugBreak();

		exit(ERROR_INVALID_FUNCTION);
	}

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
}

void OpenGLRenderer::ClearWindow(double deltaTime)
{
	const GLfloat color[] = { sin(deltaTime) * 0.5 + 0.5, cos(deltaTime) * 0.5 + 0.5, 0.0, 1.0 };

	glClearBufferfv(GL_COLOR, 0, color);

	if (glGetError() != GL_NO_ERROR)
	{
		throw new std::exception("OpenGL renderer error!");
	}
}

void OpenGLRenderer::Render(double deltaTime)
{
}