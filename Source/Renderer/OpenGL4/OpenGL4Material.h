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
			glDeleteProgram(ShaderProgramId);
			glDeleteTextures(1, &DiffuseTextureId);
		}

		uint32 DiffuseTextureId = 0;
		uint32 ShaderProgramId = 0;
	};
}
