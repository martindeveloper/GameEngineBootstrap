#pragma once

#include "../../Common.h"
#include "../Material.h"

namespace Renderer
{
	class OpenGL4Material : public Renderer::Material
	{
	public:
		virtual ~OpenGL4Material()
		{
			glDeleteBuffers(1, &VertexBuffer);
			glDeleteVertexArrays(1, &VertexArrayObject);
			glDeleteProgram(ShaderProgramId);
			glDeleteTextures(1, &DiffuseTextureId);
		}
		
		OpenGL4Buffer VertexArrayObject;
		OpenGL4Buffer VertexBuffer;
		OpenGL4Texture DiffuseTextureId = 0;
		OpenGL4ShaderProgram ShaderProgramId = 0;
	};
}
