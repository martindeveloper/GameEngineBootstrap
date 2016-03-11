#pragma once
#include "../Common.h"

#include "../../External/glew/inc/glew/glew.h"
#include "../../External/glew/inc/glew/wglew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "../Image/Image.h"
#include "../Image/ImageBMP.h"
#include "../Math/Math.h"
#include "../Graphic/Primitive/Primitive.h"

// Libraries
#pragma comment(lib, "glu32")
#if _WIN64
#pragma comment(lib, "../External/glew/lib/glew32_x64")
#else
#pragma comment(lib, "../External/glew/lib/glew32")
#endif
#pragma comment(lib, "opengl32")

namespace Renderer{
	class OpenGLRenderer : public IWindowRenderer
	{
	public:
		OpenGLRenderer();
		virtual ~OpenGLRenderer();

		void BeforeStart(HDC WindowDeviceContext, const bool isWindowed) override;
		void ClearWindow(const double deltaTime) override;
		void Update(const double deltaTime) override;
		void Render(const double deltaTime) override;

		GLuint UploadTexture(Image::Image* image);

	private:
		GLuint VBO;
		GLuint VAO;
		GLuint ShaderProgramID;

		GLint CompileShader(const char* path, GLenum type);
		GLint CreateShaderProgram(GLint vertexShader, GLint fragmentShader);

		void PrepareBuffers();
		void CreateShaders();

		// TODO: Delete this temp stuff
		Graphic::Primitive::TrianglePrimitive* Triangle;
		Graphic::Primitive::CubePrimitive* Cube;
		GLuint TestTexture;
	};
}