#pragma once

#include "ColorRGBA.h"
#include "Vector3.h"

#pragma pack(push, 1)
namespace Graphic
{
	struct Vertex
	{
		Math::Vector3<float> Position;
		Graphic::ColorRGBA Color;
	};
};
#pragma pack(pop)