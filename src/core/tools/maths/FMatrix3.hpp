#pragma once

#include <stdint.h>

#include "tools/maths/FVector2.hpp"
#include "tools/maths/FVector3.hpp"
#include "tools/maths/FVector4.hpp"

namespace xuzy::Maths
{
	/**
	* @brief Mathematic representation of a 3x3 Matrix of floats
	*/
	struct FMatrix3
	{
		float data[9];
		static const FMatrix3 Identity;

		/**
		* @brief @brief Default constructor
		*/
		FMatrix3();

		/**
		* @brief @brief Set all elements to value
		* @param p_value
		*/
		FMatrix3(float p_value);

		/**
		* @brief @brief Constructor
		* @param p_element1
		* @param p_element2
		* @param p_element3
		* @param p_element4
		* @param p_element5
		* @param p_element6
		* @param p_element7
		* @param p_element9
		* @param p_element8
		*/
		FMatrix3(float p_element1, float p_element2, float p_element3,
				 float p_element4, float p_element5, float p_element6, 
				 float p_element7, float p_element8, float p_element9);

		/**
		* @brief @brief Copy constructor
		* @param p_other
		*/
		FMatrix3(const FMatrix3& p_other);

		/**
		* @brief Copy assignment
		* @param p_other
		*/
		FMatrix3& operator=(const FMatrix3& p_other);

		/**
		* @brief Check if elements are equals
		* @param p_other
		*/
		bool operator==(const FMatrix3& p_other);

		/**
		* @brief Element-wise addition
		* @param p_other
		*/
		FMatrix3 operator+(const FMatrix3& p_other) const;

		/**
		* @brief Element-wise addition
		* @param p_other
		*/
		FMatrix3& operator+=(const FMatrix3& p_other);

		/**
		* @brief Element-wise subtraction
		* @param p_other
		*/
		FMatrix3 operator-(const FMatrix3& p_other) const;

		/**
		* @brief Element-wise subtraction
		* @param p_other
		*/
		FMatrix3& operator-=(const FMatrix3& p_other);

		/**
		* @brief Scalar Product
		* @param p_scalar
		*/
		FMatrix3 operator*(float p_scalar) const;

		/**
		* @brief Scalar Product
		* @param p_scalar
		*/
		FMatrix3& operator*=(float p_scalar);

		/**
		* @brief Vector Product
		* @param p_vector
		*/
		FVector3 operator*(const FVector3& p_vector) const;

		/**
		* @brief Matrix Product
		* @param p_other
		*/
		FMatrix3 operator*(const FMatrix3& p_other) const;

		/**
		* @brief Matrix Product
		* @param p_other
		*/
		FMatrix3& operator*=(const FMatrix3& p_other);

		/**
		* @brief Scalar Division
		* @param p_scalar
		*/
		FMatrix3 operator/(float p_scalar) const;

		/**
		* @brief Scalar Division
		* @param p_scalar
		*/
		FMatrix3& operator/=(float p_scalar);

		/**
		* @brief Matrix Division
		* @param p_other
		*/
		FMatrix3 operator/(const FMatrix3& p_other) const;

		/**
		* @brief Matrix Division
		* @param p_other
		*/
		FMatrix3& operator/=(const FMatrix3& p_other);

		/**
		* @brief Get element at index (row,column)
		* @param p_row
		* @param p_column
		*/
		float& operator()(uint8_t p_row, uint8_t p_column);
		
		/**
		* @brief Check if elements are equals
		* @param p_left
		* @param p_right
		*/
		static bool AreEquals(const FMatrix3& p_left, const FMatrix3& p_right);

		/**
		* @brief Element-wise addition
		* @param p_left
		* @param p_scalar
		*/
		static FMatrix3 Add(const FMatrix3& p_left, float p_scalar);

		/**
		* @brief Element-wise addition
		* @param p_left
		* @param p_right
		*/
		static FMatrix3 Add(const FMatrix3& p_left, const FMatrix3& p_right);

		/**
		* @brief Element-wise subtraction
		* @param p_left
		* @param p_scalar
		*/
		static FMatrix3 Subtract(const FMatrix3& p_left, float p_scalar);

		/**
		* @brief Element-wise subtractions
		* @param p_left
		* @param p_right
		*/
		static FMatrix3 Subtract(const FMatrix3& p_left, const FMatrix3& p_right);

		/**
		* @brief Scalar Product
		* @param p_left
		* @param p_scalar
		*/
		static FMatrix3 Multiply(const FMatrix3& p_left, float p_scalar);

		/**
		* @brief Vector Product
		* @param p_matrix
		* @param p_vector
		*/
		static FVector3 Multiply(const FMatrix3& p_matrix, const FVector3& p_vector);

		/**
		* @brief Matrix Product
		* @param p_left
		* @param p_right
		*/
		static FMatrix3 Multiply(const FMatrix3& p_left, const FMatrix3& p_right);

		/**
		* @brief Scalar Division
		* @param p_left
		* @param p_scalar
		*/
		static FMatrix3 Divide(const FMatrix3& p_left, float p_scalar);

		/**
		* @brief Matrix Division
		* @param p_left
		* @param p_right
		*/
		static FMatrix3 Divide(const FMatrix3& p_left, const FMatrix3& p_right);

		/**
		* @brief Compare to Identity matrix
		* @param p_matrix
		*/
		static bool IsIdentity(const FMatrix3& p_matrix);

		/**
		* @brief Compute matrix determinant
		* @param p_matrix
		*/
		static float Determinant(const FMatrix3& p_matrix);

		/**
		* @brief Return transposed matrix
		* @param p_matrix
		*/
		static FMatrix3 Transpose(const FMatrix3& p_matrix);

		/**
		* @brief Return Cofactor matrix
		* @param p_matrix
		*/
		static FMatrix3 Cofactor(const FMatrix3& p_matrix);

		/**
		* @brief Return Minor matrix
		* @param p_matrix
		*/
		static FMatrix3 Minor(const FMatrix3& p_matrix);

		/**
		* @brief Return Adjoint matrix
		* @param p_other
		*/
		static FMatrix3 Adjoint(const FMatrix3& p_other);

		/**
		* @brief Return inverse matrix
		* @param p_matrix
		*/
		static FMatrix3 Inverse(const FMatrix3& p_matrix);

		/**
		* @brief Return 2D translation matrix
		* @param p_translation
		*/
		static FMatrix3 Translation(const FVector2& p_translation);

		/**
		* @brief Translate matrix in 2D
		* @param p_matrix
		* @param p_translation
		*/
		static FMatrix3 Translate(const FMatrix3& p_matrix, const FVector2& p_translation);

		/**
		* @brief Return 2D rotation matrix
		* @param p_rotation angle in radians
		*/
		static FMatrix3 Rotation(float p_rotation);

		/**
		* @brief Rotate matrix in 2D
		* @param p_matrix
		* @param p_rotation angle in radians
		*/
		static FMatrix3 Rotate(const FMatrix3& p_matrix, float p_rotation);

		/**
		* @brief Return 2D scaling matrix
		* @param p_scale
		*/
		static FMatrix3 Scaling(const FVector2& p_scale);

		/**
		* @brief Scale matrix in 2D
		* @param p_matrix
		* @param p_scale
		*/
		static FMatrix3 Scale(const FMatrix3& p_matrix, const FVector2& p_scale);
	
		/**
		* @brief Get row
		* @param p_row
		*/
		static FVector3 GetRow(const FMatrix3& p_matrix, uint8_t p_row);

		/**
		* @brief Get Column
		* @param p_column
		*/
		static FVector3 GetColumn(const FMatrix3& p_matrix, uint8_t p_column);
	};

} // namespace xuzy::Maths