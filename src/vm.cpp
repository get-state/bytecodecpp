#include "vm.h"
#include "chunk.h"
#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "value.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

void VM::concatenate() {
  auto a = std::get<VALUECAST(ValueType::STRING)>(this->pop());
  auto b = std::get<VALUECAST(ValueType::STRING)>(this->pop());
  *a = *b + *a;
  this->push(Value(a));
}

// TODO implement runtimeerror, to show where the error is.
//
// Runs the bytecode stored in the stack. Will throw std::runtime_error in case
// the bytecode interpreter runs into a runtime error.
InterpretResult VM::run() {

#define READ_CONSTANT()                                                        \
  (this->chunk.getConstantValue(                                               \
      this->chunk.getAtOffset(this->ip++))) // reads one byte ahead.
                                            //
#define BINARY_OP(vType, op)                                                   \
  do {                                                                         \
    if (!value::isNumber(this->peek(0)) || !value::isNumber(this->peek(1))) {  \
      throw std::runtime_error("Operands must be numbers.");                   \
    }                                                                          \
    double b = value::asNumber(this->pop());                                   \
    double a = value::asNumber(this->pop());                                   \
    this->push(Value(std::in_place_index<static_cast<int>(vType)>, a op b));   \
  } while (false)

  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    std::cout << "\t";
    for (auto byteCode : this->stack) {
      std::cout << "[ ";
      value::printValue(byteCode);
      std::cout << " ]";
    }
    std::cout << std::endl;
    disassembleInstruction(this->chunk, static_cast<size_t>(this->ip));
#endif
    uint8_t instruction;
    switch (instruction = this->chunk.getAtOffset(this->ip++)) {
    case OP_CONSTANT: {
      Value constant = READ_CONSTANT();
      this->push(constant);
      break;
    }
    case OP_RETURN: {
      value::printValue(this->pop());
      std::cout << std::endl;
      return INTERPRET_OK;
    }
    case OP_NEGATE: {
      if (!value::isNumber(this->peek(0))) {
        throw(std::runtime_error("Operand must be a number."));
      }
      this->push(Value(-value::asNumber(this->pop())));
      break;
    }
    case OP_ADD:
      if (value::isString(this->peek(0)) && value::isString(this->peek(1))) {
        concatenate();
      } else if (value::isNumber(this->peek(0)) &&
                 value::isNumber(this->peek(1))) {
        double b = value::asNumber(this->pop());
        double a = value::asNumber(this->pop());
        this->push(Value(
            std::in_place_index<static_cast<int>(ValueType::NUMBER)>, a + b));
      } else {
        throw(std::runtime_error("Operand must be a number."));
      }
      break;
    case OP_SUBTRACT:
      BINARY_OP(ValueType::NUMBER, -);
      break;
    case OP_DIVIDE:
      BINARY_OP(ValueType::NUMBER, /);
      break;
    case OP_MULTIPLY:
      BINARY_OP(ValueType::NUMBER, *);
      break;
    case OP_NIL:
      this->push(Value(std::in_place_index<VALUECAST(ValueType::NIL)>, 0));
      break;
    case OP_TRUE:
      this->push(Value(std::in_place_index<VALUECAST(ValueType::BOOL)>, true));
      break;
    case OP_GREATER: {
      BINARY_OP(ValueType::BOOL, >);
      break;
    }
    case OP_LESS: {
      BINARY_OP(ValueType::BOOL, <);
      break;
    }
    case OP_EQUAL: {
      Value a = this->pop();
      Value b = this->pop();
      this->push(Value(std::in_place_index<VALUECAST(ValueType::BOOL)>,
                       value::valuesEqual(a, b)));
      break;
    }
    case OP_FALSE:
      this->push(Value(std::in_place_index<VALUECAST(ValueType::BOOL)>, false));
      break;
    case OP_NOT:
      this->push(Value(std::in_place_index<VALUECAST(ValueType::BOOL)>,
                       value::isFalsey(this->pop())));
      break;
    }
  }
#undef READ_CONSTANT
#undef BINARY_OP
}

InterpretResult VM::interpret(std::string const &source) {
  try {
    Compiler compile(source, this->chunk);
  } catch (std::logic_error const &) { // TODO: make own error class. We do not
    return INTERPRET_COMPILE_ERROR;
  }
  this->ip = 0;
  try {
    run();
  } catch (std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    return INTERPRET_RUNTIME_ERROR;
  }
  return INTERPRET_OK;
}
