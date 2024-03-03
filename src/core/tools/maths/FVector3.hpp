#pragma once

namespace xuzy::Maths
{
	/**
	* @brief Mathematic representation of a 2D vector of floats
	*/
	struct XUZY_API FVector3
	{
		static const FVector3 One;
		static const FVector3 Zero;
		static const FVector3 Forward;
		static const FVector3 Right;
		static const FVector3 Up;

		float x;
		float y;
		float z;

		/**
		* @brief Default constructor
		* @param p_x
		* @param p_y
		* @param p_z
		*/
		FVector3(float p_x = 0.0f, float p_y = 0.0f, float p_z = 0.0f);

		/**
		* @brief Copy constructor
		* @param p_toCopy
		*/
		FVector3(const FVector3& p_toCopy);

		/**
		* @brief Move constructor
		* @param p_toMove
		*/
		FVector3(FVector3&& p_toMove) noexcept = default;

		/**
		* @brief Negation
		*/
		FVector3 operator-() const;

		/**
		* @brief Copy assignment
		* @param p_other
		*/
		FVector3 operator=(const FVector3& p_other);

		/**
		* @brief Calculate the sum of two vectors
		* @param p_other
		*/
		FVector3 operator+(const FVector3& p_other) const;

		/**
		* @brief Add the right vector to the left one
		* @param p_other
		*/
		FVector3& operator+=(const FVector3& p_other);

		/**
		* @brief Calculate the substraction of two vectors
		* @param p_other
		*/
		FVector3 operator-(const FVector3& p_other) const;

		/**
		* @brief Remove the right vector from the left one
		* @param p_other
		*/
		FVector3& operator-=(const FVector3& p_other);

		/**
		* @brief Calcualte the multiplication of a vector with a scalar
		* @param p_scalar
		*/
		FVector3 operator*(float p_scalar) const;

		/**
		* @brief Multiply the vector by a scalar
		* @param p_scalar
		*/
		FVector3& operator*=(float p_scalar);

		/**
		* @brief Return the division of scalar and actual vector
		* @param p_scalar
		*/
		FVector3 operator/(float p_scalar) const;

		/**
		* @brief Divide scalar to the actual vector
		* @param p_scalar
		*/
		FVector3& operator/=(float p_scalar);

		/**
		* @brief Return true if the two vectors are equals
		* @param p_other
		*/
		bool operator==(const FVector3& p_other);

		/**
		* @brief Return true if the two vectors are different
		* @param p_other
		*/
		bool operator!=(const FVector3& p_other);

		/**
		* @brief Calculate the sum of two vectors
		* @param p_left (First vector)
		* @param p_right (Second vector)
		*/
		static FVector3 Add(const FVector3& p_left, const FVector3& p_right);

		/**
		* @brief Calculate the substraction of two vectors
		* @param p_left (First vector)
		* @param p_right (Second vector)
		*/
		static FVector3 Substract(const FVector3& p_left, const FVector3& p_right);

		/**
		* @brief Calculate the multiplication of a vector with a scalar
		* @param p_target
		* @param p_scalar
		*/
		static FVector3 Multiply(const FVector3& p_target, float p_scalar);

		/**
		* @brief Divide scalar to vector left
		* @param p_left
		* @param p_scalar
		*/
		static FVector3 Divide(const FVector3& p_left, float p_scalar);

		/**
		* @brief Return the length of a vector
		* @param p_target
		*/
		static float Length(const FVector3& p_target);

		/**
		* @brief Return the dot product of two vectors
		* @param p_left
		* @param p_right
		*/
		static float Dot(const FVector3& p_left, const FVector3& p_right);

		/**
		* Return the distance between two vectors
		* @param p_left
		* @param p_right
		*/
		static float Distance(const FVector3& p_left, const FVector3& p_right);

		/**
		* Return the cross product of two vectors
		* @param p_left
		* @param p_right
		*/
		static FVector3 Cross(const FVector3& p_left, const FVector3& p_right);

		/**
		* @brief Return the normalize of the given vector
		* @param p_target
		*/
		static FVector3 Normalize(const FVector3& p_target);

		/**
		* @brief Calculate the interpolation between two vectors
		* @param p_start
		* @param p_end
		* @param p_alpha
		*/
		static FVector3 Lerp(const FVector3& p_start, const FVector3& p_end, float p_alpha);

		/**
		* Calculate the angle between two vectors
		* @param p_from
		* @param p_to
		*/
		static float AngleBetween(const FVector3& p_from, const FVector3& p_to);
	};

} // namespace xuzy::Maths