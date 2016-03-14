#pragma once

#pragma pack(push, 1)
namespace Math
{
	template<typename T>
	struct Vector2
	{
		T X, Y;

		Vector2<T>()
		{
		};

		Vector2<T>(T x, T y)
		{
			X = x;
			Y = y;
		};

		Vector2<T> operator*(T scale)
		{
			Vector2<T> result(*this);

			result.X = this->X * scale;
			result.Y = this->Y * scale;

			return result;
		};
	};
};
#pragma pack(pop)