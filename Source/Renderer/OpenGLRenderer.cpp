#include "OpenGLRenderer.h"

using namespace Renderer;

OpenGLRenderer::OpenGLRenderer()
{
}

OpenGLRenderer::~OpenGLRenderer()
{
}

void OpenGLRenderer::BeforeStart(HDC WindowDeviceContext, const bool isWindowed)
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
	glDepthFunc(GL_ALWAYS);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);

	// Create VAO and VBO
	PrepareBuffers();

	// Triangle VAO
	Triangle = new Graphic::Primitive::TrianglePrimitive();

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*Triangle), Triangle, GL_DYNAMIC_DRAW);

	// Create shaders
	CreateShaders();

	// Unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);
}

void OpenGLRenderer::ClearWindow(const double deltaTime)
{
	const GLfloat color[] = { (float)sin(deltaTime) * 0.5f + 0.5f, (float)cos(deltaTime) * 0.5f + 0.5f, 0.0f, 1.0f };

	glClearBufferfv(GL_COLOR, 0, color);

	if (glGetError() != GL_NO_ERROR)
	{
		throw new std::exception("OpenGL renderer error!");
	}
}

void OpenGLRenderer::Update(const double deltaTime)
{

}

void OpenGLRenderer::Render(const double deltaTime)
{
	glViewport(0, 0, 1024, 768);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glUseProgram(ShaderProgramID);

	GLint attributeVertexPosition = glGetAttribLocation(ShaderProgramID, "vertexPosition");
	GLint attributeVertexColor = glGetAttribLocation(ShaderProgramID, "vertexColor");
	
	const GLsizei stride = 7 * sizeof(GLfloat);

	glVertexAttribPointer(attributeVertexPosition, 3, GL_FLOAT, GL_FALSE, stride, 0); // X, Y, Z
	glVertexAttribPointer(attributeVertexColor, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(3 * sizeof(GLfloat))); // R, G, B, A

	glEnableVertexAttribArray(attributeVertexPosition);
	glEnableVertexAttribArray(attributeVertexColor);

	// TODO: Remove this code below. Only for rotation testing
	// Frame counter uniform block
	{
		static unsigned __int32 frameCounter = 0;

		GLint frameCounterUniform = glGetUniformLocation(ShaderProgramID, "frameNumber");

		assert(frameCounterUniform >= 0);

		glUniform1f(frameCounterUniform, (float)frameCounter);

		frameCounter++;
	}
	// End of frame counter uniform

	// MVP block
	{
		GLint mvpUniform = glGetUniformLocation(ShaderProgramID, "ModelViewProjectionMatrix");

		assert(mvpUniform >= 0);

		Math::Matrix4x4 projectionMatrix(Math::Vector3<float>(.8f, .3f, 1.0f));
		Math::Matrix4x4 viewMatrix;
		Math::Matrix4x4 modelMatrix;

		const Math::Matrix4x4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

		glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, (const GLfloat*)&mvpMatrix);
	}
	// End of MVP block

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisableVertexAttribArray(attributeVertexPosition);
	glDisableVertexAttribArray(attributeVertexColor);

	GLenum err = glGetError();

	if (err != GL_NO_ERROR)
	{
		const char* message = (const char*)gluErrorString(err);
		OutputDebugStringA(message);
		DebugBreak();
	}
}

GLint OpenGLRenderer::CompileShader(const char* path, GLenum type)
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

	if (compilationResult != GL_TRUE)
	{
		std::vector<char> shaderErrorMessage(compilationLogLength);
		glGetShaderInfoLog(shaderID, compilationLogLength, NULL, &shaderErrorMessage[0]);

		// Output debug message
		std::cout << "Shader #" << shaderID << " compilation result: " << &shaderErrorMessage[0] << std::endl;
		OutputDebugStringA(&shaderErrorMessage[0]);
	}
	
	return shaderID;
}

GLint OpenGLRenderer::CreateShaderProgram(GLint vertexShader, GLint fragmentShader)
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

void OpenGLRenderer::PrepareBuffers()
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

void OpenGLRenderer::CreateShaders()
{
	// Shaders
	GLuint vertexShader = CompileShader("RotationVertex.glsl", GL_VERTEX_SHADER);
	GLuint fragmentShader = CompileShader("SolidColorFragment.glsl", GL_FRAGMENT_SHADER);

	ShaderProgramID = CreateShaderProgram(vertexShader, fragmentShader);
}