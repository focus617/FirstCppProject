#ifndef BASE_HPP_
#define BASE_HPP_

class Base
{
public:
  virtual ~Base() {}
  virtual void saySomething() = 0;
};

class InvalidBase{};

#endif  // BASE_HPP_
