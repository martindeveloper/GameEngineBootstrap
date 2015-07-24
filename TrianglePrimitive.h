#pragma once

#include "Vertex.h"
namespace Graphic
{
	namespace Primitive
	{
		class TrianglePrimitive
		{
		private:
			Graphic::Vertex Verticies[3];

		public:
			TrianglePrimitive(Math::Vector3<float> positions[3], Graphic::ColorRGBA colors[3])
			{
				for (unsigned __int32 i = 0; i < 3; i++)
				{
					Verticies[i].Position = positions[i];
					Verticies[i].Color = colors[i];
				}
			};

			TrianglePrimitive()
			{
				Verticies[0].Position = { 0.0f, 0.5f, 0.5f };
				Verticies[1].Position = { 0.5f, -0.5f, 0.5f };
				Verticies[2].Position = { -0.5f, -0.5f, 0.5f };

				Verticies[0].Color = Graphic::ColorRGBA::Red(0.2f);
				Verticies[1].Color = Graphic::ColorRGBA::Green(0.6f);
				Verticies[2].Color = Graphic::ColorRGBA(0.5f, 0.5f, 0.5f, 0.5f);
			};
		};
	};
};