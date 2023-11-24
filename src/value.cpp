#include "value.h"
#include <iostream>
#include <stdexcept>

namespace value {
double asNumber(Value value) {
  switch (value.index()) {
  case VALUECAST(ValueType::NUMBER):
    return std::get<static_cast<int>(ValueType::NUMBER)>(value);
  case VALUECAST(ValueType::NIL):
    return std::get<static_cast<bool>(ValueType::NIL)>(value);
  case VALUECAST(ValueType::BOOL):
    return std::get<static_cast<bool>(ValueType::BOOL)>(value);
  default:
    throw std::logic_error("cannot cast non 'nil'/'integer/bool' as number");
  }
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

bool valuesEqual(Value a, Value b) {
  if (a.index() != b.index()) {
    return false;
  }
  switch (a.index()) {
  case static_cast<int>(ValueType::BOOL):
    return asBool(a) == asBool(b);
  case static_cast<int>(ValueType::NUMBER):
    return asNumber(a) == asNumber(b);
  case static_cast<int>(ValueType::NIL):
    return true;
  default:
    throw std::logic_error("Invalid value type");
  }
}

void printValue(Value value) {
  switch (value.index()) {
  case VALUECAST(ValueType::BOOL):
    std::cout << value::asBool(value);
    break;
  case VALUECAST(ValueType::NUMBER):
    std::cout << value::asNumber(value);
    break;
  case VALUECAST(ValueType::NIL):
    std::cout << "nil";
    break;
  }
}
} // namespace value
