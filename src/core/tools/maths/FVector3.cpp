#include <utility>
#include <stdexcept>
#include <cmath>

#include "tools/maths/FVector3.hpp"

namespace xuzy::Maths {

const FVector3 FVector3::One(1.0f, 1.0f, 1.0f);
const FVector3 FVector3::Zero(0.0f, 0.0f, 0.0f);
const FVector3 FVector3::Forward(0.0f, 0.0f, 1.0f);
const FVector3 FVector3::Right(1.0f, 0.0f, 0.0f);
const FVector3 FVector3::Up(0.0f, 1.0f, 0.0f);

FVector3::FVector3(float p_x, float p_y, float p_z) : x(p_x), y(p_y), z(p_z)
{
}

FVector3::FVector3(const FVector3& p_toCopy) : x(p_toCopy.x), y(p_toCopy.y), z(p_toCopy.z)
{
}

FVector3 FVector3::operator-() const
{
	return operator*(-1);
}

FVector3 FVector3::operator=(const FVector3& p_other)
{
	this->x = p_other.x;
	this->y = p_other.y;
	this->z = p_other.z;

	return *this;
}

FVector3 FVector3::operator+(const FVector3& p_other) const
{
	return Add(*this, p_other);
}

FVector3& FVector3::operator+=(const FVector3& p_other)
{
	*this = Add(*this, p_other);
	return *this;
}

FVector3 FVector3::operator-(const FVector3& p_other) const
{
	return Substract(*this, p_other);
}

FVector3& FVector3::operator-=(const FVector3& p_other)
{
	*this = Substract(*this, p_other);
	return *this;
}

FVector3 FVector3::operator*(float p_scalar) const
{
	return Multiply(*this, p_scalar);
}

FVector3& FVector3::operator*=(float p_scalar)
{
	*this = Multiply(*this, p_scalar);
	return *this;
}

FVector3 FVector3::operator/(float p_scalar) const
{
	return Divide(*this, p_scalar);
}

FVector3& FVector3::operator/=(float p_scalar)
{
	*this = Divide(*this, p_scalar);
	return *this;
}

bool FVector3::operator==(const FVector3 & p_other)
{
	return
		this->x == p_other.x &&
		this->y == p_other.y &&
		this->z == p_other.z;
}

bool FVector3::operator!=(const FVector3 & p_other)
{
	return !operator==(p_other);
}

FVector3 FVector3::Add(const FVector3& p_left, const FVector3& p_right)
{
	return FVector3
	(
		p_left.x + p_right.x,
		p_left.y + p_right.y,
		p_left.z + p_right.z
	);
}

FVector3 FVector3::Substract(const FVector3& p_left, const FVector3& p_right)
{
	return FVector3
	(
		p_left.x - p_right.x,
		p_left.y - p_right.y,
		p_left.z - p_right.z
	);
}

FVector3 FVector3::Multiply(const FVector3& p_target, float p_scalar)
{
	return FVector3
	(
		p_target.x * p_scalar,
		p_target.y * p_scalar,
		p_target.z * p_scalar
	);
}

FVector3 FVector3::Divide(const FVector3& p_left, float p_scalar)
{
	FVector3 result(p_left);

	if (p_scalar == 0)
		throw std::logic_error("Division by 0");

	result.x /= p_scalar;
	result.y /= p_scalar;
	result.z /= p_scalar;

	return result;
}

float FVector3::Length(const FVector3& p_target)
{
	return sqrtf(p_target.x * p_target.x + p_target.y * p_target.y + p_target.z * p_target.z);
}

float FVector3::Dot(const FVector3& p_left, const FVector3& p_right)
{
	return p_left.x * p_right.x + p_left.y * p_right.y + p_left.z * p_right.z;
}

float FVector3::Distance(const FVector3 & p_left, const FVector3 & p_right)
{
	return std::sqrt
	(
		(p_left.x - p_right.x) * (p_left.x - p_right.x) +
		(p_left.y - p_right.y) * (p_left.y - p_right.y) +
		(p_left.z - p_right.z) * (p_left.z - p_right.z)
	);
}

FVector3 FVector3::Cross(const FVector3 & p_left, const FVector3 & p_right)
{
	return FVector3
	(
		p_left.y * p_right.z - p_left.z * p_right.y,
		p_left.z * p_right.x - p_left.x * p_right.z,
		p_left.x * p_right.y - p_left.y * p_right.x
	);
}
FVector3 FVector3::Normalize(const FVector3& p_target)
{
	float length = Length(p_target);

	if (length > 0.0f)
	{
		float targetLength = 1.0f / length;

		return FVector3
		(
			p_target.x * targetLength,
			p_target.y * targetLength,
			p_target.z * targetLength
		);
	}
	else
	{
		return FVector3::Zero;
	}
}

FVector3 FVector3::Lerp(const FVector3& p_start, const FVector3& p_end, float p_alpha)
{
	return (p_start + (p_end - p_start) * p_alpha);
}

float FVector3::AngleBetween(const FVector3& p_from, const FVector3& p_to)
{
	float lengthProduct = Length(p_from) * Length(p_to);

	if (lengthProduct > 0.0f)
	{
		float fractionResult = Dot(p_from, p_to) / lengthProduct;

		if (fractionResult >= -1.0f && fractionResult <= 1.0f)
			return acosf(fractionResult);
	}

	return 0.0f;
}

} // namespace xuzy::Maths
