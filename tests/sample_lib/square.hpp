#ifndef SQUARE_H
#define SQUARE_H

#include "shape.hpp"

///
/// \brief TODO
///
class XUZY_API Square : public Shape
{
public:
  Square(float a);
  float Area() const override;
  std::string get_name() const override;
private:
  float a_;
};


#endif //SQUARE_H
