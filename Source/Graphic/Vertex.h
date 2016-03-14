#pragma once

#include "ColorRGBA.h"
#include "../Math/Vector3.h"
#include "UVCoordinates.h"

#pragma pack(push, 1)
namespace Graphic
{
	struct Vertex
	{
		Math::Vector3<float> Position;
		Graphic::ColorRGBA Color;
#ifdef VERTEX_HAVE_UV
		UVCoordinates UV;
#endif
#ifdef VERTEX_HAVE_NORMAL
		Math::Vector3<float> Normal;
#endif
	};
};
#pragma pack(pop)