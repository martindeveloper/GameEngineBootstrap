#pragma once

#include "../../Common.h"
#include "OpenGL4Renderer.h"

namespace Renderer
{
	class OpenGL4Renderer;

	class OpenGL4FrameBuffer
	{
	public:
		// TODO(martin.pernica): Use data struct for this pairs
		const char* PostProcessDefaultVertex = "ScreenVertex.glsl";
		const char* PostProcessDefaultFragment = "ScreenFragment.glsl";

		const char* PostProcessBlurVertex = "ScreenVertex.glsl";
		const char* PostProcessBlurFragment = "ScreenBlurFragment.glsl";

		OpenGL4FrameBuffer(Renderer::OpenGL4Renderer* renderer);
		virtual ~OpenGL4FrameBuffer();

		void Create();
		void Bind();
		void Unbind();
		void Draw();

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