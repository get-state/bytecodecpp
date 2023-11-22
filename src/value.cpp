#include "value.h"
#include <iostream>

namespace value {
double _asNumber(Value value) {
  return std::get<static_cast<int>(ValueType::NUMBER)>(value);
}

bool asBool(Value value) {
  return std::get<static_cast<int>(ValueType::BOOL)>(value);
}

bool isNill(Value value) {
  return value.index() == static_cast<int>(ValueType::NIL);
}

bool isNumber(Value value) {
  return value.index() == static_cast<int>(ValueType::NUMBER);
}

bool isBool(Value value) {
  return value.index() == static_cast<int>(ValueType::BOOL);
}

bool isFalsey(Value value) {
  return isNill(value) || (isBool(value) && !asBool(value));
}

void printValue(Value value) {
  switch (value.index()) {
  case VALUECAST(ValueType::BOOL):
    std::cout << value::asBool(value);
    break;
  case VALUECAST(ValueType::NUMBER):
    std::cout << value::_asNumber(value);
    break;
  case VALUECAST(ValueType::NIL):
    std::cout << "nil";
    break;
  }
}
} // namespace value
