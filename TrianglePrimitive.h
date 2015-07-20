#pragma once

#include "Vertex.h"

class TrianglePrimitive
{
private:
	Vertex Verticies[3];

public:
	TrianglePrimitive()
	{
		Verticies[0].Position = { 0.0f, 0.5f, -1.0f };
		Verticies[1].Position = { 0.5f, -0.5f, -1.0f };
		Verticies[2].Position = { -0.5f, -0.5f, -1.0f };

		Verticies[0].Color = { 1.0f, 0.0f, 0.0f, 1.0f };
		Verticies[1].Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Verticies[2].Color = { 0.0f, 0.0f, 0.0f, 1.0f };
	};
};