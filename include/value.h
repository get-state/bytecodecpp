#ifndef clox_value_h
#define clox_value_h

#include <memory>
#include <string>
#include <variant>
#include <vector>

#define VALUECAST(x) static_cast<int>(x)

enum class ValueType {
  // primatives
  BOOL,
  NIL,
  NUMBER,
  //objects
  STRING,
};

// a variant holding the possible cLox values. Indexed by the ValueType enum.
typedef std::variant<bool, bool, double, std::shared_ptr<std::string>> Value;


// Some helper functions
namespace value {
double asNumber(Value value);

bool isNumber(Value value);

bool isBool(Value value);

bool isNill(Value value);

bool isString(Value value);

void printValue(Value value);

bool isFalsey(Value value);

bool valuesEqual(Value a, Value b);

} // namespace value

/* #define AS_NUMBER(value) \ */
/*   (Value(< std::in_place_index<static_cast<int>(ValueType::NUMBER)>, value))
 */
/* #define AS_BOOL(value) \ */
/*   (Value(< std::in_place_index<static_cast<int>(ValueType::BOOL)>, value)) */
/* #define AS_NIL(value) \ */
/*   (Value(< std::in_place_index<static_cast<int>(ValueType::NIL)>, value)) */

/* typedef double Value; */


class ValueArray {
public:
  [[nodiscard]] Value at(int index) { return this->values.at(index); }

  void push_back(Value v) { this->values.push_back(v); }

  [[nodiscard]] int size() { return this->values.size(); }

private:
  std::vector<Value> values = {};
};

#endif /* ifndef clox_value_h */
