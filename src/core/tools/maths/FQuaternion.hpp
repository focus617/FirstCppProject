#pragma once

#include "tools/maths/FVector3.hpp"
#include "tools/maths/FMatrix3.hpp"
#include "tools/maths/FMatrix4.hpp"

#include <string>

namespace xuzy::Maths
{

	/**
	* @brief Mathematic representation of a Quaternion with float precision
	*/
	struct FQuaternion
	{
	public:
		float x;
		float y;
		float z;
		float w;

		/**
		* @brief Return an identity quaternion
		*/
		static const FQuaternion Identity;

		/**
		* @brief Default Quaternion constructor (Create an identity quaternion with 1 as w)
		*/
		FQuaternion();

		/**
		* @brief Create an identity quaternion with a defined real value
		* @param p_real
		*/
		FQuaternion(float p_real);

		/**
		* @brief Create a quaternion from a set of 4 floats (x, y, z, w)
		* @param p_x Vector part of FQuaternion
		* @param p_y Vector part of FQuaternion
		* @param p_z Vector part of FQuaternion
		* @param p_w Real value of FQuaternion
		* @note In pure/applied Maths, we write W (or real), (Xi + Yj + Zk) (or Vector)
		*/
		FQuaternion(float p_x, float p_y, float p_z, float p_w);

		/**
		* @brief Copy Constructor
		* @param p_other
		*/
		FQuaternion(const FQuaternion& p_other);

		/**
		* @brief Construct from rotation matrix
		* @param p_rotationMatrix
		*/
		FQuaternion(const FMatrix3& p_rotationMatrix);

		/**
		* @brief Construct from rotation matrix
		* @param p_rotationMatrix
		*/
		FQuaternion(const FMatrix4& p_rotationMatrix);

		/**
		* @brief Construct from euler angles
		* @param p_euler
		*/
		FQuaternion(const FVector3& p_euler);

		/**
		* @brief Create a quaternion from a forward and up vector
		* @param p_forward
		* @param p_up
		*/
		static FQuaternion LookAt(const FVector3& p_forward, const FVector3& p_up);

		/**
		* @brief Check if the quaternion is Identity
		* if the quaternion has no rotation(meaning x,y,z axis values = 0), it's Identity
		* @param p_target
		*/
		static bool IsIdentity(const FQuaternion& p_target);

		/**
		* @brief Check if the quaternion is pure
		* If the quaternion has no real value(meaning real part = 0), it's pure
		* @param p_target
		*/
		static bool IsPure(const FQuaternion& p_target);

		/**
		* @brief Check if the quaternion is normalized
		* @param p_target
		*/
		static bool IsNormalized(const FQuaternion& p_target);

		/**
		* @brief Calculate the dot product between two quaternions
		* @param p_left
		* @param p_right
		*/
		static float DotProduct(const FQuaternion& p_left, const FQuaternion& p_right);	

		/**
		* @brief Calculate the normalized of a quaternion
		* @param p_target
		*/
		static FQuaternion Normalize(const FQuaternion& p_target);

		/**
		* @brief Calculate the length of a quaternion
		* @param p_target
		*/
		static float Length(const FQuaternion& p_target);

		/**
		* @brief Calculate the length square of a quaternion
		* @param p_target
		*/
		static float LengthSquare(const FQuaternion& p_target);

		/**
		* @brief Return the angle of a quaternion
		* @param p_target
		*/
		static float GetAngle(const FQuaternion& p_target);

		/**
		* @brief Return the rotation axis of the given quaternion
		* @param p_target
		*/
		static FVector3 GetRotationAxis(const FQuaternion& p_target);

		/**
		* @brief Calculate the inverse of a quaternion
		* @param p_target
		*/
		static FQuaternion Inverse(const FQuaternion& p_target);

		/**
		* @brief Calculate the conjugate of a quaternion
		* @param p_target
		*/
		static FQuaternion Conjugate(const FQuaternion& p_target);

		/**
		* @brief Calculate the square of a quaternion
		* @param p_target
		*/
		static FQuaternion Square(const FQuaternion& p_target);

		/**
		* @brief Get the axis and the angle from a quaternion
		* @param p_axis
		* @param p_angle
		*/
		static std::pair<FVector3, float> GetAxisAndAngle(const FQuaternion& p_target);

		/**
		* @brief Caculate the angle between two quaternions.
		* @param p_left
		* @param p_right
		*/
		static float AngularDistance(const FQuaternion& p_left, const FQuaternion& p_right);

		/**
		* @brief Lerp two quaternions
		* @param p_start
		* @param p_end
		* @param p_alpha
		*/
		static FQuaternion Lerp(const FQuaternion& p_start, const FQuaternion& p_end, float p_alpha);

		/**
		* @brief Slerp two quaternions
		* @param p_first
		* @param p_second
		* @param p_alpha
		*/
		static FQuaternion Slerp(const FQuaternion& p_start, const FQuaternion& p_end, float p_alpha);

		/**
		* @brief Nlerp two quaternions (= Lerp + normalization)
		* @param p_start
		* @param p_end
		* @param p_alpha
		*/
		static FQuaternion Nlerp(const FQuaternion& p_start, const FQuaternion& p_end, float p_alpha);

		/**
		* @brief Rotate a point using a rotation quaternion (qpq^-1)
		* @param p_point
		* @param p_quaternion
		*/
		static FVector3 RotatePoint(const FVector3& p_point, const FQuaternion& p_quaternion);

		/**
		* @brief Rotate a point around a pivot point using a rotation quaternion
		* @param p_point
		* @param p_pivot
		* @param p_quaternion
		*/
		static FVector3 RotatePoint(const FVector3& p_point, const FQuaternion& p_quaternion, const FVector3& p_pivot);

		/**
		* @brief Returning Euler axis angles (In degrees) for each axis.
		* @param p_target
		*/
		static FVector3 EulerAngles(const FQuaternion& p_target);

		/**
		* @brief Return a rotation matrix (3x3) out of the given quaternion
		* @param p_target
		*/
		static FMatrix3 ToMatrix3(const FQuaternion& p_target);

		/**
		* @brief Return a rotation matrix (4x4) out of the given quaternion
		* @param p_target
		*/
		static FMatrix4 ToMatrix4(const FQuaternion& p_target);

		FQuaternion& operator=(const FQuaternion&);
		bool operator==(const FQuaternion& p_otherQuat) const;
		bool operator!=(const FQuaternion& p_otherQuat) const;
		FQuaternion operator+(const FQuaternion& p_otherQuat) const;
		FQuaternion& operator+=(const FQuaternion& p_otherQuat);
		FQuaternion operator-(const FQuaternion& p_otherQuat) const;
		FQuaternion& operator-=(const FQuaternion& p_otherQuat);
		float operator|(const FQuaternion& p_otherQuat) const;
		FQuaternion& operator*=(const float p_scale);
		FQuaternion operator*(const float p_scale) const;
		FQuaternion operator*(const FQuaternion& p_otherQuat) const;
		FQuaternion& operator*=(const FQuaternion& p_otherQuat);
		FVector3 operator*(const FVector3& p_toMultiply) const;
		FMatrix3 operator*(const FMatrix3& p_multiply) const;
		FQuaternion& operator/=(const float p_scale);
		FQuaternion operator/(const float p_scale) const;
	};

} // namespace xuzy::Maths