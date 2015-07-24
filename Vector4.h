#pragma once

#pragma pack(push, 1)
namespace Math
{
	template<typename T>
	struct Vector4
	{
		T X, Y, Z, W;

		Vector4<T> operator*(T scale)
		{
			Vector4<T> result(*this);

			result.X = this->X * scale;
			result.Y = this->Y * scale;
			result.Z = this->Z * scale;
			result.W = this->W * scale;

			return result;
		};

		Vector4<T> operator*(Vector4<T> vector)
		{
			Vector4<T> result(*this);

			result.X = this->X * vector.X;
			result.Y = this->Y * vector.Y;
			result.Z = this->Z * vector.Z;
			result.W = this->W * vector.W;

			return result;
		};

		Vector4<T> operator*(Vector3<T> vector)
		{
			Vector4<T> result(*this);

			result.X = this->X * vector.X;
			result.Y = this->Y * vector.Y;
			result.Z = this->Z * vector.Z;

			return result;
		};
	};
};
#pragma pack(pop)