#pragma once
#include "Headers.h"

// Libraries
#pragma comment(lib, "glu32")
#pragma comment(lib, "glew32")
#pragma comment(lib, "opengl32")

namespace RendererBootstrap{
	class OpenGLRenderer : public IWindowRenderer
	{
	public:
		OpenGLRenderer();
		void BeforeStart(HDC WindowDeviceContext, bool isWindowed) override;
		void ClearWindow(double deltaTime) override;
		void Update(double deltaTime) override;
		void Render(double deltaTime) override;
		virtual ~OpenGLRenderer();

	private:
		GLuint VBO;
		GLuint VAO;
		GLuint TriangleProgramID;

		GLint CompileShader(const char* path, GLenum type);
		GLint CreateShaderProgram(GLint vertexShader, GLint fragmentShader);

		void PrepareBuffers();
		void CreateShaders();
	};
}