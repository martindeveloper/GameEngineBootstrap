#pragma once

#pragma pack(push, 1)
namespace Graphic
{
	struct UVCoordinates
	{
		float U, V;

	public:
		UVCoordinates() : U(0.0f), V(0.0f)
		{
		};

		UVCoordinates(float u, float v) : U(u), V(v)
		{
		};
	};
};
#pragma pack(pop)