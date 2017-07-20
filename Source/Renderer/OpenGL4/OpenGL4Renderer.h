#if RENDERER_OPENGL4

#pragma once

#include "../../Common.h"

#include <glew/glew.h>
#include <glew/wglew.h>

#include "../../Image/Image.h"
#include "../../Image/ImageBMP.h"
#include "../../Graphic/Buffer/ConstantBuffer.h"
#include "../../Math/Math.h"

#include "OpenGL4FrameBuffer.h"
#include "OpenGL4Material.h"

#include "../../Graphic/Vertex.h"
#include "../../Core/GameEntity.h"
#include "../../Game/Entities/CubeEntity.h"

#include "../../Game/Components/RendererComponent.h"

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

		// Static geometry
		OpenGL4Buffer StaticGeometryBuffer;
		OpenGL4Buffer StaticGeometryVertexArrayObject;

		// Materials
		std::vector<Renderer::OpenGL4Material*> Materials;

		void LoadStaticEntities();
		void LoadDynamicEntities();

		void PrepareBuffers() override;

		template<typename T>
		void CreateConstantBuffer(T* targetBuffer);

		void CreateShaderForEntity(Core::GameEntity* entity) override;
		void CreateBuffersForEntity(Core::GameEntity* entity);
	};
}

#endif // RENDERER_OPENGL4
