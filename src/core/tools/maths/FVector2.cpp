#include <utility>
#include <stdexcept>
#include <cmath>

#include "tools/maths/FVector2.hpp"

namespace xuzy::Maths {

const FVector2 FVector2::One(1.0f, 1.0f);
const FVector2 FVector2::Zero(0.0f, 0.0f);

FVector2::FVector2(float p_x, float p_y) :
	x(p_x), y(p_y)
{
}

FVector2::FVector2(const FVector2& p_toCopy) :
	x(p_toCopy.x), y(p_toCopy.y)
{
}

FVector2 FVector2::operator-() const
{
	return operator*(-1);
}

FVector2 FVector2::operator=(const FVector2& p_other)
{
	this->x = p_other.x;
	this->y = p_other.y;

	return *this;
}

FVector2 FVector2::operator+(const FVector2& p_other) const
{
	return Add(*this, p_other);
}

FVector2& FVector2::operator+=(const FVector2& p_other)
{
	*this = Add(*this, p_other);
	return *this;
}

FVector2 FVector2::operator-(const FVector2& p_other) const
{
	return Substract(*this, p_other);
}

FVector2& FVector2::operator-=(const FVector2& p_other)
{
	*this = Substract(*this, p_other);
	return *this;
}

FVector2 FVector2::operator*(float p_scalar) const
{
	return Multiply(*this, p_scalar);
}

FVector2& FVector2::operator*=(float p_scalar)
{
	*this = Multiply(*this, p_scalar);
	return *this;
}

FVector2 FVector2::operator/(float p_scalar) const
{
	return Divide(*this, p_scalar);
}

FVector2& FVector2::operator/=(float p_scalar)
{
	*this = Divide(*this, p_scalar);
	return *this;
}

bool FVector2::operator==(const FVector2 & p_other)
{
	return
		this->x == p_other.x &&
		this->y == p_other.y;
}

bool FVector2::operator!=(const FVector2 & p_other)
{
	return !operator==(p_other);
}

FVector2 FVector2::Add(const FVector2 & p_left, const FVector2 & p_right)
{
	return FVector2
	(
		p_left.x + p_right.x,
		p_left.y + p_right.y
	);
}

FVector2 FVector2::Substract(const FVector2 & p_left, const FVector2 & p_right)
{
	return FVector2
	(
		p_left.x - p_right.x,
		p_left.y - p_right.y
	);
}

FVector2 FVector2::Multiply(const FVector2& p_target, float p_scalar)
{
	return FVector2
	(
		p_target.x * p_scalar,
		p_target.y * p_scalar
	);
}

FVector2 FVector2::Divide(const FVector2& p_left, float p_scalar)
{
	FVector2 result(p_left);

	if (p_scalar == 0)
		throw std::logic_error("Division by 0");

	result.x /= p_scalar;
	result.y /= p_scalar;

	return result;
}

float FVector2::Length(const FVector2& p_target)
{
	return sqrtf(p_target.x * p_target.x + p_target.y * p_target.y);
}

float FVector2::Dot(const FVector2& p_left, const FVector2& p_right)
{
	return p_left.x * p_right.x + p_left.y * p_right.y;
}

FVector2 FVector2::Normalize(const FVector2& p_target)
{
	float length = Length(p_target);

	if (length > 0.0f)
	{
		float targetLength = 1.0f / length;

		return FVector2
		(
			p_target.x * targetLength,
			p_target.y * targetLength
		);
	}
	else
	{
		return FVector2::Zero;
	}
}

FVector2 FVector2::Lerp(const FVector2 & p_start, const FVector2 & p_end, float p_alpha)
{
	return (p_start + (p_end - p_start) * p_alpha);
}

float FVector2::AngleBetween(const FVector2 & p_from, const FVector2 & p_to)
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
