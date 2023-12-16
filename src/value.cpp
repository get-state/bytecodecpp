#include "value.h"
#include <iostream>
#include <stdexcept>
#include <variant>

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

std::string asString(Value value) {
  return std::get<static_cast<int>(ValueType::STRING)>(value).getStr();
}

bool isNill(Value value) {
  return value.index() == static_cast<int>(ValueType::NIL);
}

bool isNumber(Value value) { return std::holds_alternative<double>(value); }

bool isString(Value value) {
  return std::holds_alternative<cloxString>(value);
}

bool isBool(Value value) {
  return value.index() == static_cast<int>(ValueType::BOOL);
}

bool isFalsey(Value value) {
  return isNill(value) || (isBool(value) && !asBool(value));
}

void printStringObj(Value value) {
  auto tmp =
      std::get<static_cast<int>(ValueType::STRING)>(value);
  std::cout << "string: " << '"' << tmp.getStr() << '"';
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
  case static_cast<int>(ValueType::STRING):
    return asString(a) == asString(b);
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
  case VALUECAST(ValueType::STRING):
    value::printStringObj(value);
    break;
  }
}
} // namespace value
