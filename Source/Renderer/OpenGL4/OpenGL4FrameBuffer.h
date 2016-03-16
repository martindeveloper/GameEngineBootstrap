#pragma once

#include "../../Common.h"
#include "OpenGL4Renderer.h"

namespace Renderer
{
	class OpenGL4Renderer;

	class OpenGL4FrameBuffer
	{
	public:
		OpenGL4FrameBuffer(Renderer::OpenGL4Renderer* renderer);
		virtual ~OpenGL4FrameBuffer();

		void Create();
		void Bind();
		void Unbind();
		void Draw();

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