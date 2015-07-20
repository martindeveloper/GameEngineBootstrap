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
	glDepthFunc(GL_LEQUAL);

	GLint majorVersion;
	GLint minorVersion;

	glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

	// Create VAO and VBO
	PrepareBuffers();

	// Triangle VAO
	TrianglePrimitive triangle;

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), &triangle, GL_STATIC_DRAW);

	// Create shaders
	CreateShaders();

	// Unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);
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

void OpenGLRenderer::Update(double deltaTime)
{

}

void OpenGLRenderer::Render(double deltaTime)
{
	glViewport(0, 0, 1024, 768);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glUseProgram(TriangleProgramID);

	GLint attributeVertexPosition = glGetAttribLocation(TriangleProgramID, "vertexPosition");
	GLint attributeVertexColor = glGetAttribLocation(TriangleProgramID, "vertexColor");

	const GLsizei stride = 7 * sizeof(GLfloat);

	glVertexAttribPointer(attributeVertexPosition, 3, GL_FLOAT, GL_FALSE, stride, 0); // X, Y, Z
	glVertexAttribPointer(attributeVertexColor, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(3 * sizeof(GLfloat))); // R, G, B, A

	glEnableVertexAttribArray(attributeVertexPosition);
	glEnableVertexAttribArray(attributeVertexColor);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(attributeVertexPosition);
	glDisableVertexAttribArray(attributeVertexColor);

	GLenum err = glGetError();

	if (err != GL_NO_ERROR)
	{
		OutputDebugStringW(L"My output string.");
	}
}

GLint RendererBootstrap::OpenGLRenderer::CompileShader(const char* path, GLenum type)
{
	// Create files helper class to read file
	FileSystem::File file(path);

	// Variables to debug of shader compilation
	GLint compilationResult = GL_FALSE;
	int compilationLogLength;

	// Create shader
	GLuint shaderID = glCreateShader(type);

	// Read file
	std::string shaderString = file.GetContent();
	const char* shaderCharPointer = shaderString.c_str();

	// Pass source code of shader and compile it
	glShaderSource(shaderID, 1, &shaderCharPointer, NULL);
	glCompileShader(shaderID);

	// Check compilation
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compilationResult);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &compilationLogLength);
	std::vector<char> shaderErrorMessage(compilationLogLength);
	glGetShaderInfoLog(shaderID, compilationLogLength, NULL, &shaderErrorMessage[0]);

	// Output debug message
	std::cout << "Shader #" << shaderID << " compilation result: " << &shaderErrorMessage[0] << std::endl;
	OutputDebugStringA(&shaderErrorMessage[0]);

	return shaderID;
}

GLint RendererBootstrap::OpenGLRenderer::CreateShaderProgram(GLint vertexShader, GLint fragmentShader)
{
	GLint isLinked = GL_FALSE;
	GLuint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int *)&isLinked);

	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, &infoLog[0]);

		// Output debug message
		std::cerr << "Shader program #" << shaderProgram << " link result: " << &infoLog[0] << std::endl;
		OutputDebugStringA(&infoLog[0]);

		glDeleteProgram(shaderProgram);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void RendererBootstrap::OpenGLRenderer::PrepareBuffers()
{
	// VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);
}

void RendererBootstrap::OpenGLRenderer::CreateShaders()
{
	// Shaders
	GLuint vertexShader = CompileShader("TriangleVertex.glsl", GL_VERTEX_SHADER);
	GLuint fragmentShader = CompileShader("TriangleFragment.glsl", GL_FRAGMENT_SHADER);

	TriangleProgramID = CreateShaderProgram(vertexShader, fragmentShader);
}
