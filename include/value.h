#ifndef clox_value_h
#define clox_value_h

#include <vector>

typedef double Value;

void printValue(Value value);

class ValueArray {
public:
  Value at(int index) { return this->values[index]; }

  void push_back(Value v) { this->values.push_back(v); }

  int size() { return this->values.size(); }

private:
  std::vector<Value> values = {};
};

#endif /* ifndef clox_value_h */
