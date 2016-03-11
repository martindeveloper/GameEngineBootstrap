#pragma once

#include "../Vertex.h"
namespace Graphic
{
	namespace Primitive
	{
		class CubePrimitive
		{
		public:
			const unsigned __int16 DrawCount = 12 * 3;
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
				const float rawCubeVerticies[] = {
					-1.0f,-1.0f,-1.0f,
					-1.0f,-1.0f, 1.0f,
					-1.0f, 1.0f, 1.0f,
					1.0f, 1.0f,-1.0f,
					-1.0f,-1.0f,-1.0f,
					-1.0f, 1.0f,-1.0f,
					1.0f,-1.0f, 1.0f,
					-1.0f,-1.0f,-1.0f,
					1.0f,-1.0f,-1.0f,
					1.0f, 1.0f,-1.0f,
					1.0f,-1.0f,-1.0f,
					-1.0f,-1.0f,-1.0f,
					-1.0f,-1.0f,-1.0f,
					-1.0f, 1.0f, 1.0f,
					-1.0f, 1.0f,-1.0f,
					1.0f,-1.0f, 1.0f,
					-1.0f,-1.0f, 1.0f,
					-1.0f,-1.0f,-1.0f,
					-1.0f, 1.0f, 1.0f,
					-1.0f,-1.0f, 1.0f,
					1.0f,-1.0f, 1.0f,
					1.0f, 1.0f, 1.0f,
					1.0f,-1.0f,-1.0f,
					1.0f, 1.0f,-1.0f,
					1.0f,-1.0f,-1.0f,
					1.0f, 1.0f, 1.0f,
					1.0f,-1.0f, 1.0f,
					1.0f, 1.0f, 1.0f,
					1.0f, 1.0f,-1.0f,
					-1.0f, 1.0f,-1.0f,
					1.0f, 1.0f, 1.0f,
					-1.0f, 1.0f,-1.0f,
					-1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f,
					-1.0f, 1.0f, 1.0f,
					1.0f,-1.0f, 1.0f
				};

				const float rawCubeUV[] = {
					0.000059f, 1.0f - 0.000004f,
					0.000103f, 1.0f - 0.336048f,
					0.335973f, 1.0f - 0.335903f,
					1.000023f, 1.0f - 0.000013f,
					0.667979f, 1.0f - 0.335851f,
					0.999958f, 1.0f - 0.336064f,
					0.667979f, 1.0f - 0.335851f,
					0.336024f, 1.0f - 0.671877f,
					0.667969f, 1.0f - 0.671889f,
					1.000023f, 1.0f - 0.000013f,
					0.668104f, 1.0f - 0.000013f,
					0.667979f, 1.0f - 0.335851f,
					0.000059f, 1.0f - 0.000004f,
					0.335973f, 1.0f - 0.335903f,
					0.336098f, 1.0f - 0.000071f,
					0.667979f, 1.0f - 0.335851f,
					0.335973f, 1.0f - 0.335903f,
					0.336024f, 1.0f - 0.671877f,
					1.000004f, 1.0f - 0.671847f,
					0.999958f, 1.0f - 0.336064f,
					0.667979f, 1.0f - 0.335851f,
					0.668104f, 1.0f - 0.000013f,
					0.335973f, 1.0f - 0.335903f,
					0.667979f, 1.0f - 0.335851f,
					0.335973f, 1.0f - 0.335903f,
					0.668104f, 1.0f - 0.000013f,
					0.336098f, 1.0f - 0.000071f,
					0.000103f, 1.0f - 0.336048f,
					0.000004f, 1.0f - 0.671870f,
					0.336024f, 1.0f - 0.671877f,
					0.000103f, 1.0f - 0.336048f,
					0.336024f, 1.0f - 0.671877f,
					0.335973f, 1.0f - 0.335903f,
					0.667969f, 1.0f - 0.671889f,
					1.000004f, 1.0f - 0.671847f,
					0.667979f, 1.0f - 0.335851f
				};

				for (unsigned __int32 i = 0; i < DrawCount; i++)
				{
					Verticies[i].Position = (((Math::Vector3<float>*)rawCubeVerticies)[i]);
					Verticies[i].Color = color;
					Verticies[i].UV = ((UVCoordinates*)rawCubeUV)[i];
				}
			};
		};
	};
};