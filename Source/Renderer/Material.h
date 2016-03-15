#pragma once

#include "../Common.h"

namespace Renderer
{
	class Material
	{
	public:
		virtual ~Material() {};

		std::string VertexShader;
		std::string PixelShader;

		// Not used
		std::string GeometryShader;
		std::string HullShader;
		std::string DomainShader;
	};
}
