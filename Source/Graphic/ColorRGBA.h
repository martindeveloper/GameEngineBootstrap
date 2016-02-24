#pragma once

#pragma pack(push, 1)
namespace Graphic
{
	struct ColorRGBA
	{
		float R, G, B, A;

	public:
		ColorRGBA() : R(0.0f), G(0.0f), B(0.0f), A(1.0f)
		{
		};

		ColorRGBA(float red, float green, float blue, float alpha = 1.0f) : R(red), G(green), B(blue), A(alpha)
		{
		};

		static ColorRGBA Red(float alpha = 1.0f)
		{
			return{ 1.0f, 0.0f, 0.0f, alpha };
		};

		static ColorRGBA Green(float alpha = 1.0f)
		{
			return{ 0.0f, 1.0f, 0.0f, alpha };
		};

		static ColorRGBA Blue(float alpha = 1.0f)
		{
			return{ 0.0f, 0.0f, 1.0f, alpha };
		};
	};
};
#pragma pack(pop)