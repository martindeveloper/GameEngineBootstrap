#pragma once

#include "ColorRGBA.h"
#include "Vector3.h"

#pragma pack(push, 1)
struct Vertex
{
	Vector3 Position;
	ColorRGBA Color;
};
#pragma pack(pop)