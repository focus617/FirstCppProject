#ifndef HELLOBASE_H
#define HELLOBASE_H

#include <string>

#include "visibility_control.hpp"

class XUZY_API Shape {
public:
  virtual ~Shape() {}
  virtual float Area() const =0;
  virtual std::string get_name() const =0;
};
#endif  // HELLOBASE_H
