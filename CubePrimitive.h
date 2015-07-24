#pragma once

#include "Vertex.h"
namespace Graphic
{
	namespace Primitive
	{
		class CubePrimitive
		{
		private:
			Graphic::Vertex Verticies[36];

		public:
			CubePrimitive(Math::Vector3<float> positions[3], Graphic::ColorRGBA colors[3])
			{
				for (unsigned __int32 i = 0; i < 36; i++)
				{
					Verticies[i].Position = positions[i];
					Verticies[i].Color = colors[i];
				}
			};

			CubePrimitive(Graphic::ColorRGBA color)
			{
				static const float rawCubeVerticies[] = {
					-1.0f, -1.0f, -1.0f,
					-1.0f, -1.0f, 1.0f,
					-1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, -1.0f,
					-1.0f, -1.0f, -1.0f,
					-1.0f, 1.0f, -1.0f,
					1.0f, -1.0f, 1.0f,
					-1.0f, -1.0f, -1.0f,
					1.0f, -1.0f, -1.0f,
					1.0f, 1.0f, -1.0f,
					1.0f, -1.0f, -1.0f,
					-1.0f, -1.0f, -1.0f,
					-1.0f, -1.0f, -1.0f,
					-1.0f, 1.0f, 1.0f,
					-1.0f, 1.0f, -1.0f,
					1.0f, -1.0f, 1.0f,
					-1.0f, -1.0f, 1.0f,
					-1.0f, -1.0f, -1.0f,
					-1.0f, 1.0f, 1.0f,
					-1.0f, -1.0f, 1.0f,
					1.0f, -1.0f, 1.0f,
					1.0f, 1.0f, 1.0f,
					1.0f, -1.0f, -1.0f,
					1.0f, 1.0f, -1.0f,
					1.0f, -1.0f, -1.0f,
					1.0f, 1.0f, 1.0f,
					1.0f, -1.0f, 1.0f,
					1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, -1.0f,
					-1.0f, 1.0f, -1.0f,
					1.0f, 1.0f, 1.0f,
					-1.0f, 1.0f, -1.0f,
					-1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f,
					-1.0f, 1.0f, 1.0f,
					1.0f, -1.0f, 1.0f
				};

				for (unsigned __int32 i = 0; i < 36; i++)
				{
					Verticies[i].Position = (((Math::Vector3<float>*)rawCubeVerticies)[i]) * (float)0.5f;
					Verticies[i].Color = color;
				}
			};
		};
	};
};