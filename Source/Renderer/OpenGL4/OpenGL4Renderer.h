#pragma once
#include "../../Common.h"

#include "../../../External/glew/inc/glew/glew.h"
#include "../../../External/glew/inc/glew/wglew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "../../Image/Image.h"
#include "../../Image/ImageBMP.h"
#include "../../Math/Math.h"

#include "OpenGL4Material.h"

#include "../../Graphic/Vertex.h"
#include "../../Core/GameEntity.h"
#include "../../Game/Entities/CubeEntity.h"

// Libraries
#pragma comment(lib, "glu32")
#if _WIN64
#pragma comment(lib, "../External/glew/lib/glew32_x64")
#else
#pragma comment(lib, "../External/glew/lib/glew32")
#endif
#pragma comment(lib, "opengl32")

namespace Renderer{
	class OpenGL4Renderer : public IWindowRenderer
	{
	public:
		OpenGL4Renderer();
		virtual ~OpenGL4Renderer();

		void BeforeStart(HDC WindowDeviceContext, const bool isWindowed) override;
		void ClearWindow(const double deltaTime) override;
		void Update(const double deltaTime) override;
		void Render(const double deltaTime) override;
		Renderer::Material* CreateMaterial() override;
		void UploadTexture(Core::GameEntity* entity, Image::Image* image) override;

	private:
		GLuint VBO;
		GLuint VAO;

		// Materials
		std::vector<Renderer::OpenGL4Material*> Materials;

		GLint CompileShader(const char* path, GLenum type);
		GLint CreateShaderProgram(GLint vertexShader, GLint fragmentShader);

		void PrepareBuffers() override;
		void CreateShaderForEntity(Core::GameEntity* entity) override;
	};
}