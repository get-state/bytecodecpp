#ifndef clox_value_h
#define clox_value_h

#include <vector>

typedef double Value;

void printValue(Value value);

class ValueArray {
public:
  [[nodiscard]] Value at(int index) { return this->values.at(index); }

  void push_back(Value v) { this->values.push_back(v); }

  [[nodiscard]] int size() { return this->values.size(); }

private:
  std::vector<Value> values = {};
};

#endif /* ifndef clox_value_h */
