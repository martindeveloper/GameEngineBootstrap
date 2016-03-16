#pragma once
#include "../../Common.h"

#include "../../../External/glew/inc/glew/glew.h"
#include "../../../External/glew/inc/glew/wglew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "../../Image/Image.h"
#include "../../Image/ImageBMP.h"
#include "../../Graphic/Buffer/ConstantBuffer.h"
#include "../../Math/Math.h"

#include "OpenGL4FrameBuffer.h"
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

namespace Renderer
{
	class OpenGL4FrameBuffer;

	class OpenGL4Renderer : public IWindowRenderer
	{
	public:
		OpenGL4Renderer();
		virtual ~OpenGL4Renderer();

		void BeforeStart(HDC windowDeviceContext, const bool isWindowed) override;
		void ClearWindow(const double deltaTime) override;
		void Update(const double deltaTime) override;
		void Render(const double deltaTime) override;
		Renderer::Material* CreateMaterial() override;
		void UploadTexture(Core::GameEntity* entity, Image::Image* image) override;

		GLint CompileShader(const char* path, GLenum type);
		GLint CreateShaderProgram(GLint vertexShader, GLint fragmentShader);

	private:
		HDC WindowDeviceContext;

		// Global buffers
		Renderer::OpenGL4FrameBuffer* FrameBuffer;
		Graphic::Buffer::ConstantBuffer UniformBuffer;

		// Materials
		std::vector<Renderer::OpenGL4Material*> Materials;

		void PrepareBuffers() override;

		template<typename T>
		void CreateConstantBuffer(T* targetBuffer);

		void CreateShaderForEntity(Core::GameEntity* entity) override;
		void CreateBuffersForEntity(Core::GameEntity* entity);
	};
}