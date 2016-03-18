#include "OpenGL4Renderer.h"

using namespace Renderer;

OpenGL4Renderer::OpenGL4Renderer()
{
}

OpenGL4Renderer::~OpenGL4Renderer()
{
	// Clear materials
	for (auto material : Materials)
	{
		delete material;
	}

	Materials.clear();

	for (auto entity : GameEntitites)
	{
		// Call dtor of entity
		delete entity;
	}

	GameEntitites.clear();

	delete FrameBuffer;
}

void OpenGL4Renderer::BeforeStart(HDC windowDeviceContext, const bool isWindowed)
{
	this->WindowDeviceContext = windowDeviceContext;

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

	int WindowPixelFormat = ChoosePixelFormat(windowDeviceContext, &OpenGLPixelFormatDescriptor);

	SetPixelFormat(windowDeviceContext, WindowPixelFormat, &OpenGLPixelFormatDescriptor);

	// Create OpenGL context
	HGLRC FakeOpenGLRenderingContextHandle = wglCreateContext(windowDeviceContext);

	// Make OpenGL current
	if (!wglMakeCurrent(windowDeviceContext, FakeOpenGLRenderingContextHandle))
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

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
		0
	};

	HGLRC OpenGLRenderingContextHandle = wglCreateContextAttribsARB(windowDeviceContext, 0, attribs);
	wglMakeCurrent(NULL, NULL);
	BOOL isTempContextDeleted = wglDeleteContext(FakeOpenGLRenderingContextHandle);
	BOOL isContextActive = wglMakeCurrent(windowDeviceContext, OpenGLRenderingContextHandle);

	PrepareBuffers();

	FrameBuffer->Create();

	{
		// Cube entity
		Game::Entities::CubeEntity* cube = new Game::Entities::CubeEntity("Cube1");
		cube->StartingPosition = { -10.0f, 0, 0 };
		GameEntitites.push_back((Core::GameEntity*)cube);

		// 2nd
		Game::Entities::CubeEntity* cube2 = new Game::Entities::CubeEntity("Cube2");
		cube2->StartingPosition = { 0.0f, 0, 0 };
		GameEntitites.push_back((Core::GameEntity*)cube2);
	}

	{
		for (auto entity : GameEntitites)
		{
			entity->SetRenderer(this);
			entity->OnLoad();

			std::vector<Graphic::Vertex>* verticies = entity->GetVerticies();

			CreateBuffersForEntity(entity);

			Renderer::OpenGL4Material* material = static_cast<Renderer::OpenGL4Material*>(entity->GetMaterial());

			glBindVertexArray(material->VertexArrayObject);
			glBindBuffer(GL_ARRAY_BUFFER, material->VertexBuffer);

			glBufferData(GL_ARRAY_BUFFER, entity->GetVertexBufferWidth(), &verticies->at(0), GL_STATIC_DRAW);

			CreateShaderForEntity(entity);
		}
	}

	// Unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);
}

void OpenGL4Renderer::UploadTexture(Core::GameEntity* entity, Image::Image* image)
{
	Renderer::OpenGL4Material* material = static_cast<Renderer::OpenGL4Material*>(entity->GetMaterial());

	image->Load();

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->GetWidth(), image->GetHeight(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image->GetDataPointer());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	image->Free();

	material->DiffuseTextureId = textureID;
}

void OpenGL4Renderer::ClearWindow(const double deltaTime)
{
	FrameBuffer->Bind();

	glClearColor((float)sin(deltaTime) * 0.5f + 0.5f, (float)cos(deltaTime) * 0.5f + 0.5f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	GLenum result = glGetError();

	if (result != GL_NO_ERROR)
	{
		DebugBreak();
		throw new std::exception("OpenGL renderer error!");
	}
}

void OpenGL4Renderer::Update(const double deltaTime)
{
	// Modify constant values
	{
		static uint32 frameCounter = 0;

		const glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)Parameters.Width / (float)Parameters.Height, 0.1f, 100.0f);
		const glm::mat4 viewMatrix = glm::lookAt(glm::vec3(4, 3, -3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		const glm::mat4 modelMatrix = glm::mat4(1.0f);

		const glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

		UniformBuffer.ModelViewProjectionMatrix = Math::Matrix4x4((const float*)&mvpMatrix);
		UniformBuffer.FrameNumber = frameCounter;

		frameCounter++;
	}

	for (auto entity : GameEntitites)
	{
		entity->OnUpdate(deltaTime);
	}
}

void OpenGL4Renderer::Render(const double deltaTime)
{
	glViewport(0, 0, Parameters.Width, Parameters.Height);

	FrameBuffer->Bind();

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLsizei stride = 0;

	for (auto entity : GameEntitites)
	{
		Renderer::OpenGL4Material* entityMaterial = static_cast<Renderer::OpenGL4Material*>(entity->GetMaterial());

		glBindVertexArray(entityMaterial->VertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, entityMaterial->VertexBuffer);

		const uint32 shaderProgram = entityMaterial->ShaderProgramId;

		glUseProgram(shaderProgram);

		GLint attributeVertexPosition = glGetAttribLocation(shaderProgram, "vertexPosition");
		GLint attributeVertexColor = glGetAttribLocation(shaderProgram, "vertexColor");
		GLint attributeVertexUV = glGetAttribLocation(shaderProgram, "vertexUV");
		GLint uniformTextureSampler = glGetUniformLocation(shaderProgram, "mainTextureSampler");

		assert(attributeVertexPosition >= 0);
		assert(attributeVertexColor >= 0);
		assert(attributeVertexUV >= 0);
		assert(uniformTextureSampler >= 0);

		stride = entity->GetVertexBufferStride();

		glVertexAttribPointer(attributeVertexPosition, 3, GL_FLOAT, GL_FALSE, stride, 0); // X, Y, Z
		glVertexAttribPointer(attributeVertexColor, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(3 * sizeof(GLfloat))); // R, G, B, A
		glVertexAttribPointer(attributeVertexUV, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(7 * sizeof(GLfloat))); // U, V

		glEnableVertexAttribArray(attributeVertexPosition);
		glEnableVertexAttribArray(attributeVertexColor);
		glEnableVertexAttribArray(attributeVertexUV);

		// Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, entityMaterial->DiffuseTextureId);
		glUniform1i(uniformTextureSampler, 0);

		// Constant buffer
		GLint mvpUniform = glGetUniformLocation(shaderProgram, "UniformBuffer.ModelViewProjectionMatrix");
		GLint frameCounterUniform = glGetUniformLocation(shaderProgram, "UniformBuffer.FrameNumber");

		glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, (const GLfloat*)&UniformBuffer.ModelViewProjectionMatrix);
		glUniform1ui(frameCounterUniform, UniformBuffer.FrameNumber);

		// Transform buffer
		GLint positionUniform = glGetUniformLocation(shaderProgram, "Transform.Position");
		GLint scaleUniform = glGetUniformLocation(shaderProgram, "Transform.Scale");

		glUniform3fv(positionUniform, 1, (const GLfloat*)&entityMaterial->Transform.Position);
		glUniform3fv(scaleUniform, 1, (const GLfloat*)&entityMaterial->Transform.Scale);

		glDrawArrays(GL_TRIANGLES, 0, entity->GetVerticies()->size());

		glDisableVertexAttribArray(attributeVertexPosition);
		glDisableVertexAttribArray(attributeVertexColor);
		glDisableVertexAttribArray(attributeVertexUV);

		GLenum err = glGetError();

		if (err != GL_NO_ERROR)
		{
			const char* message = (const char*)gluErrorString(err);
			OutputDebugStringA(message);
			DebugBreak();
		}
	}

	FrameBuffer->Draw();

	//SwapBuffers(WindowDeviceContext);
}

Renderer::Material* OpenGL4Renderer::CreateMaterial()
{
	Renderer::OpenGL4Material* material = new Renderer::OpenGL4Material();

	Materials.push_back(material);

	return material;
}

GLint OpenGL4Renderer::CompileShader(const char* path, GLenum type)
{
	// Variables to debug of shader compilation
	GLint compilationResult = GL_FALSE;
	int compilationLogLength;

	// Create shader
	GLuint shaderID = glCreateShader(type);

	// Create files helper class to read file
	FileSystem::File file(path);

	if (!file.IsExists())
	{
		const char* error = "Can not load shader file";
		std::cout << error << std::endl;
		OutputDebugStringA(error);

		return shaderID;
	}

	file.Load();

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

GLint OpenGL4Renderer::CreateShaderProgram(GLint vertexShader, GLint fragmentShader)
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

void OpenGL4Renderer::PrepareBuffers()
{
	// Framebuffer
	FrameBuffer = new Renderer::OpenGL4FrameBuffer(this);

	// Constant buffer
	CreateConstantBuffer<Graphic::Buffer::ConstantBuffer>(&UniformBuffer);

	// Unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);
}

template<typename T>
void OpenGL4Renderer::CreateConstantBuffer(T* targetBuffer)
{
	SecureZeroMemory(targetBuffer, sizeof(T));
}

void OpenGL4Renderer::CreateBuffersForEntity(Core::GameEntity* entity)
{
	Renderer::OpenGL4Material* material = static_cast<Renderer::OpenGL4Material*>(entity->GetMaterial());

	glGenVertexArrays(1, &material->VertexArrayObject);
	glGenBuffers(1, &material->VertexBuffer);

	assert(glGetError() == GL_NO_ERROR);
}

void OpenGL4Renderer::CreateShaderForEntity(Core::GameEntity* entity)
{
	Renderer::OpenGL4Material* material = static_cast<Renderer::OpenGL4Material*>(entity->GetMaterial());

	std::string vertexShaderName = material->VertexShader + ".glsl";
	std::string fragmentShaderName = material->PixelShader + ".glsl";

	// Shaders
	GLuint vertexShader = CompileShader(vertexShaderName.c_str(), GL_VERTEX_SHADER);
	GLuint fragmentShader = CompileShader(fragmentShaderName.c_str(), GL_FRAGMENT_SHADER);

	material->ShaderProgramId = CreateShaderProgram(vertexShader, fragmentShader);
}