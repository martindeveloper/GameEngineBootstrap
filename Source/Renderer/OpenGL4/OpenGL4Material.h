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
			glDeleteBuffers(1, &DynamicVertexBuffer);
			glDeleteVertexArrays(1, &DynamicVertexArrayObject);
			glDeleteProgram(ShaderProgramId);
			glDeleteTextures(1, &DiffuseTextureId);
		}
		
		OpenGL4Buffer DynamicVertexArrayObject;
		OpenGL4Buffer DynamicVertexBuffer;
		OpenGL4Texture DiffuseTextureId = 0;
		OpenGL4ShaderProgram ShaderProgramId = 0;
	};
}
