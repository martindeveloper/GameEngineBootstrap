#if RENDERER_OPENGL4

#pragma once

#include "../../Common.h"
#include "OpenGL4Renderer.h"
#include "../IFrameBuffer.h"

namespace Renderer
{
	class OpenGL4Renderer;

	class OpenGL4FrameBuffer : IFrameBuffer
	{
	public:
		// TODO(martin.pernica): Use data struct for this pairs
		const char* PostProcessDefaultVertex = "Shaders/ScreenVertex.glsl";
		const char* PostProcessDefaultFragment = "Shaders/ScreenFragment.glsl";

		const char* PostProcessBlurVertex = "Shaders/ScreenVertex.glsl";
		const char* PostProcessBlurFragment = "Shaders/ScreenBlurFragment.glsl";

		OpenGL4FrameBuffer(Renderer::OpenGL4Renderer* renderer);
		virtual ~OpenGL4FrameBuffer();

		void Create() override;
		void Bind() override;
		void Unbind() override;
		void Draw() override;

		void SwitchProstProcessEffect(const char* vertexShaderName, const char* fragmentShaderName);

	private:
		void PrepareScreenQuad();
		void CreateBuffer();

		Renderer::OpenGL4Renderer* Renderer;

		OpenGL4Buffer FrameBuffer;
		OpenGL4Texture ColorBufferTexture;
		OpenGL4Buffer DepthStencilBuffer;

		OpenGL4Buffer ScreenVAO;
		OpenGL4Buffer ScreenVBO;
		OpenGL4ShaderProgram ScreenShaderProgramId;

		float ScreenQuadVerticies[24] = {
			-1.0f,  1.0f,  0.0f, 1.0f,
			1.0f,  1.0f,  1.0f, 1.0f,
			1.0f, -1.0f,  1.0f, 0.0f,

			1.0f, -1.0f,  1.0f, 0.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			-1.0f,  1.0f,  0.0f, 1.0f
		};
	};
}

#endif
