#include "tools/maths/FMatrix3.hpp"

#include <cmath>
#include <stdexcept>
#include <string>

namespace xuzy::Maths {

const FMatrix3 FMatrix3::Identity =
    FMatrix3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

FMatrix3::FMatrix3() { memcpy(data, Identity.data, 9 * sizeof(float)); }

FMatrix3::FMatrix3(float p_value) {
  for (float& element : data) element = p_value;
}

FMatrix3::FMatrix3(float p_element1, float p_element2, float p_element3,
                   float p_element4, float p_element5, float p_element6,
                   float p_element7, float p_element8, float p_element9) {
  data[0] = p_element1;
  data[1] = p_element2;
  data[2] = p_element3;
  data[3] = p_element4;
  data[4] = p_element5;
  data[5] = p_element6;
  data[6] = p_element7;
  data[7] = p_element8;
  data[8] = p_element9;
}

FMatrix3::FMatrix3(const FMatrix3& p_other) { *this = p_other; }

FMatrix3& FMatrix3::operator=(const FMatrix3& p_other) {
  memcpy(this->data, p_other.data, 9 * sizeof(float));
  return *this;
}

bool FMatrix3::operator==(const FMatrix3& p_other) {
  return AreEquals(*this, p_other);
}

FMatrix3 FMatrix3::operator+(const FMatrix3& p_other) const {
  return Add(*this, p_other);
}

FMatrix3& FMatrix3::operator+=(const FMatrix3& p_other) {
  *this = Add(*this, p_other);
  return *this;
}

FMatrix3 FMatrix3::operator-(const FMatrix3& p_other) const {
  return Subtract(*this, p_other);
}

FMatrix3& FMatrix3::operator-=(const FMatrix3& p_other) {
  *this = Subtract(*this, p_other);
  return *this;
}

FMatrix3 FMatrix3::operator*(float p_scalar) const {
  return Multiply(*this, p_scalar);
}

FMatrix3& FMatrix3::operator*=(float p_scalar) {
  *this = Multiply(*this, p_scalar);
  return *this;
}

FVector3 FMatrix3::operator*(const FVector3& p_vector) const {
  return Multiply(*this, p_vector);
}

FMatrix3 FMatrix3::operator*(const FMatrix3& p_other) const {
  return Multiply(*this, p_other);
}

FMatrix3& FMatrix3::operator*=(const FMatrix3& p_other) {
  *this = Multiply(*this, p_other);
  return *this;
}

FMatrix3 FMatrix3::operator/(float p_scalar) const {
  return Divide(*this, p_scalar);
}

FMatrix3& FMatrix3::operator/=(float p_scalar) {
  *this = Divide(*this, p_scalar);
  return *this;
}

FMatrix3 FMatrix3::operator/(const FMatrix3& p_other) const {
  return Divide(*this, p_other);
}

FMatrix3& FMatrix3::operator/=(const FMatrix3& p_other) {
  *this = Divide(*this, p_other);
  return *this;
}

float& FMatrix3::operator()(uint8_t p_row, uint8_t p_column) {
  if (p_row >= 3 || p_column >= 3)
    throw std::out_of_range("Invalid index : " + std::to_string(p_row) + "," +
                            std::to_string(p_column) + " is out of range");
  return data[3 * p_row + p_column];
}

bool FMatrix3::AreEquals(const FMatrix3& p_left, const FMatrix3& p_right) {
  return memcmp(&p_left, &p_right, 9 * sizeof(float)) == 0;
}

FMatrix3 FMatrix3::Add(const FMatrix3& p_left, float p_scalar) {
  FMatrix3 result(p_left);
  for (uint8_t i = 0; i < 9; ++i) result.data[i] += p_scalar;
  return result;
}

FMatrix3 FMatrix3::Add(const FMatrix3& p_left,
                                 const FMatrix3& p_right) {
  FMatrix3 result(p_left);
  for (uint8_t i = 0; i < 9; ++i) result.data[i] += p_right.data[i];
  return result;
}

FMatrix3 FMatrix3::Subtract(const FMatrix3& p_left, float p_scalar) {
  FMatrix3 result(p_left);
  for (float& element : result.data) {
    element -= p_scalar;
  }
  return result;
}

FMatrix3 FMatrix3::Subtract(const FMatrix3& p_left,
                                      const FMatrix3& p_right) {
  FMatrix3 result(p_left);
  for (uint8_t i = 0; i < 9; ++i) result.data[i] -= p_right.data[i];
  return result;
}

FMatrix3 FMatrix3::Multiply(const FMatrix3& p_left, float p_scalar) {
  FMatrix3 result(p_left);
  for (float& element : result.data) {
    element *= p_scalar;
  }
  return result;
}

FVector3 FMatrix3::Multiply(const FMatrix3& p_matrix,
                            const FVector3& p_vector) {
  FVector3 result;
  result.x =
      ((p_matrix.data[0] * p_vector.x) + (p_matrix.data[1] * p_vector.y) +
       (p_matrix.data[2] * p_vector.z));
  result.y =
      ((p_matrix.data[3] * p_vector.x) + (p_matrix.data[4] * p_vector.y) +
       (p_matrix.data[5] * p_vector.z));
  result.z =
      ((p_matrix.data[6] * p_vector.x) + (p_matrix.data[7] * p_vector.y) +
       (p_matrix.data[8] * p_vector.z));

  return result;
}

FMatrix3 FMatrix3::Multiply(const FMatrix3& p_left, const FMatrix3& p_right) {
  return FMatrix3(
      (p_left.data[0] * p_right.data[0]) + (p_left.data[1] * p_right.data[3]) +
          (p_left.data[2] * p_right.data[6]),
      (p_left.data[0] * p_right.data[1]) + (p_left.data[1] * p_right.data[4]) +
          (p_left.data[2] * p_right.data[7]),
      (p_left.data[0] * p_right.data[2]) + (p_left.data[1] * p_right.data[5]) +
          (p_left.data[2] * p_right.data[8]),

      (p_left.data[3] * p_right.data[0]) + (p_left.data[4] * p_right.data[3]) +
          (p_left.data[5] * p_right.data[6]),
      (p_left.data[3] * p_right.data[1]) + (p_left.data[4] * p_right.data[4]) +
          (p_left.data[5] * p_right.data[7]),
      (p_left.data[3] * p_right.data[2]) + (p_left.data[4] * p_right.data[5]) +
          (p_left.data[5] * p_right.data[8]),

      (p_left.data[6] * p_right.data[0]) + (p_left.data[7] * p_right.data[3]) +
          (p_left.data[8] * p_right.data[6]),
      (p_left.data[6] * p_right.data[1]) + (p_left.data[7] * p_right.data[4]) +
          (p_left.data[8] * p_right.data[7]),
      (p_left.data[6] * p_right.data[2]) + (p_left.data[7] * p_right.data[5]) +
          (p_left.data[8] * p_right.data[8]));
}

FMatrix3 FMatrix3::Divide(const FMatrix3& p_left, float p_scalar) {
  FMatrix3 result(p_left);
  for (float& element : result.data) {
    element /= p_scalar;
  }
  return result;
}

FMatrix3 FMatrix3::Divide(const FMatrix3& p_left,
                                    const FMatrix3& p_right) {
  return p_left * Inverse(p_right);
}

bool FMatrix3::IsIdentity(const FMatrix3& p_matrix) {
  return memcmp(Identity.data, p_matrix.data, 9 * sizeof(float)) == 0;
}

float FMatrix3::Determinant(const FMatrix3& p_matrix) {
  return p_matrix.data[0] * (p_matrix.data[4] * p_matrix.data[8] -
                             p_matrix.data[5] * p_matrix.data[7]) -
         p_matrix.data[3] * (p_matrix.data[1] * p_matrix.data[8] -
                             p_matrix.data[2] * p_matrix.data[7]) +
         p_matrix.data[6] * (p_matrix.data[1] * p_matrix.data[5] -
                             p_matrix.data[2] * p_matrix.data[4]);
}

FMatrix3 FMatrix3::Transpose(const FMatrix3& p_matrix) {
  FMatrix3 result;

  result.data[0] = p_matrix.data[0];
  result.data[1] = p_matrix.data[3];
  result.data[2] = p_matrix.data[6];

  result.data[3] = p_matrix.data[1];
  result.data[4] = p_matrix.data[4];
  result.data[5] = p_matrix.data[7];

  result.data[6] = p_matrix.data[2];
  result.data[7] = p_matrix.data[5];
  result.data[8] = p_matrix.data[8];

  return result;
}

FMatrix3 FMatrix3::Cofactor(const FMatrix3& p_matrix) {
  return FMatrix3(((p_matrix.data[4] * p_matrix.data[8]) -
                   (p_matrix.data[5] * p_matrix.data[7])),  // 0
                  -((p_matrix.data[3] * p_matrix.data[8]) -
                    (p_matrix.data[5] * p_matrix.data[6])),  // 1
                  ((p_matrix.data[3] * p_matrix.data[7]) -
                   (p_matrix.data[4] * p_matrix.data[6])),  // 2
                  -((p_matrix.data[1] * p_matrix.data[8]) -
                    (p_matrix.data[2] * p_matrix.data[7])),  // 3
                  ((p_matrix.data[0] * p_matrix.data[8]) -
                   (p_matrix.data[2] * p_matrix.data[6])),  // 4
                  -((p_matrix.data[0] * p_matrix.data[7]) -
                    (p_matrix.data[1] * p_matrix.data[6])),  // 5
                  ((p_matrix.data[1] * p_matrix.data[5]) -
                   (p_matrix.data[2] * p_matrix.data[4])),  // 6
                  -((p_matrix.data[0] * p_matrix.data[5]) -
                    (p_matrix.data[2] * p_matrix.data[3])),  // 7
                  ((p_matrix.data[0] * p_matrix.data[4]) -
                   (p_matrix.data[1] * p_matrix.data[3])));  // 8
}

FMatrix3 FMatrix3::Minor(const FMatrix3& p_matrix) {
  return FMatrix3(((p_matrix.data[4] * p_matrix.data[8]) -
                   (p_matrix.data[5] * p_matrix.data[7])),  // 0
                  ((p_matrix.data[3] * p_matrix.data[8]) -
                   (p_matrix.data[5] * p_matrix.data[6])),  // 1
                  ((p_matrix.data[3] * p_matrix.data[7]) -
                   (p_matrix.data[4] * p_matrix.data[6])),  // 2
                  ((p_matrix.data[1] * p_matrix.data[8]) -
                   (p_matrix.data[2] * p_matrix.data[7])),  // 3
                  ((p_matrix.data[0] * p_matrix.data[8]) -
                   (p_matrix.data[2] * p_matrix.data[6])),  // 4
                  ((p_matrix.data[0] * p_matrix.data[7]) -
                   (p_matrix.data[1] * p_matrix.data[6])),  // 5
                  ((p_matrix.data[1] * p_matrix.data[5]) -
                   (p_matrix.data[2] * p_matrix.data[4])),  // 6
                  ((p_matrix.data[0] * p_matrix.data[5]) -
                   (p_matrix.data[2] * p_matrix.data[3])),  // 7
                  ((p_matrix.data[0] * p_matrix.data[4]) -
                   (p_matrix.data[1] * p_matrix.data[3])));  // 8
}

FMatrix3 FMatrix3::Adjoint(const FMatrix3& p_other) {
  return Transpose(Cofactor(p_other));
}

FMatrix3 FMatrix3::Inverse(const FMatrix3& p_matrix) {
  const float determinant = Determinant(p_matrix);
  if (determinant == 0) throw std::logic_error("Division by 0");

  return Adjoint(p_matrix) / determinant;
}

FMatrix3 FMatrix3::Translation(const FVector2& p_translation) {
  return FMatrix3(1, 0, p_translation.x, 0, 1, p_translation.y, 0, 0, 1);
}

FMatrix3 FMatrix3::Translate(const FMatrix3& p_matrix,
                                       const FVector2& p_translation) {
  return p_matrix * Translation(p_translation);
}

FMatrix3 FMatrix3::Rotation(float p_rotation) {
  return FMatrix3(std::cos(p_rotation), -std::sin(p_rotation), 0,
                  std::sin(p_rotation), std::cos(p_rotation), 0, 0, 0, 1);
}

FMatrix3 FMatrix3::Rotate(const FMatrix3& p_matrix,
                                    float p_rotation) {
  return p_matrix * Rotation(p_rotation);
}

FMatrix3 FMatrix3::Scaling(const FVector2& p_scale) {
  return FMatrix3(p_scale.x, 0, 0, 0, p_scale.y, 0, 0, 0, 1);
}

FMatrix3 FMatrix3::Scale(const FMatrix3& p_matrix,
                                   const FVector2& p_scale) {
  return p_matrix * Scaling(p_scale);
}

FVector3 FMatrix3::GetRow(const FMatrix3& p_matrix, uint8_t p_row) {
  if (p_row >= 3)
    throw std::out_of_range("Invalid index : " + std::to_string(p_row) +
                            " is out of range");

  return FVector3(p_matrix.data[p_row * 3], p_matrix.data[p_row * 3 + 1],
                  p_matrix.data[p_row * 3 + 2]);
}

FVector3 FMatrix3::GetColumn(const FMatrix3& p_matrix, uint8_t p_column) {
  if (p_column >= 3)
    throw std::out_of_range("Invalid index : " + std::to_string(p_column) +
                            " is out of range");

  return FVector3(p_matrix.data[p_column + 6], p_matrix.data[p_column + 3],
                  p_matrix.data[p_column]);
}

}  // namespace xuzy::Maths