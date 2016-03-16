#pragma once

#include "../Common.h"
#include "../Graphic/Buffer/TransformBuffer.h"

namespace Renderer
{
	class Material
	{
	public:
		virtual ~Material() {};

		Graphic::Buffer::TransformBuffer Transform;

		std::string VertexShader;
		std::string PixelShader;

		// Not used
		std::string GeometryShader;
		std::string HullShader;
		std::string DomainShader;
	};
}
