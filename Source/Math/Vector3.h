#pragma once

#pragma pack(push, 1)
namespace Math
{
	template<typename T>
	struct Vector3
	{
		T X, Y, Z;

		Vector3<T>()
		{
		};

		Vector3<T>(T x, T y, T z)
		{
			X = x;
			Y = y;
			Z = z;
		};

		Vector3<T> operator*(T scale)
		{
			Vector3<T> result(*this);

			result.X = this->X * scale;
			result.Y = this->Y * scale;
			result.Z = this->Z * scale;

			return result;
		};
	};
};
#pragma pack(pop)