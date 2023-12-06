#include "compiler.h"
#include "scanner.h"
#include "value.h"
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

void Compiler::errorAtCurrent(std::string_view message) {
  errorAt(this->parser.getCurrent(), message);
}

void Compiler::error(std::string_view message) {
  errorAt(this->parser.getPrevious(), message);
}

void Compiler::errorAt(Token token, std::string_view message) {
  std::cerr << "[line " << token.line << "] Error";

  if (token.type == TokenType::EoF) {
    std::cerr << " at end";
  } else if (token.type == TokenType::ERROR) {
    // nothing
  } else {
    std::cerr << " at '" << token.token << "'";
  }

  std::cerr << ": " << message << std::endl;
  throw std::logic_error(""); // panic and unwind to vm
}

// Assigns the current token to previous, and gets the next token and sets it
// to current.
void Compiler::advance() {
  this->parser.setPrevious(this->parser.getCurrent());

  for (;;) { this->parser.setCurrent(this->scanner.scanToken());
    if (parser.getCurrent().type != TokenType::ERROR) {
      break;
    }
    errorAtCurrent(this->parser.getCurrent().token);
  }
}

void Compiler::consume(TokenType type, std::string_view message) {
  if (this->parser.getCurrent().type == type) {
    advance();
    return;
  }

  errorAtCurrent(message);
}

void Compiler::emitByte(std::uint8_t byte) {
  this->chunk.writeChunk(byte, this->parser.getPrevious().line);
}

void Compiler::emitByte(std::uint8_t byte, std::uint8_t byte2) {
  this->chunk.writeChunk(byte, this->parser.getPrevious().line);
  this->chunk.writeChunk(byte2, this->parser.getPrevious().line);
}

void Compiler::emitReturn() { emitByte(OP_RETURN); }

// Returns the offset
uint8_t Compiler::makeConstant(Value value) {
  int offset = this->chunk.addConstant(value);
  if (offset > UINT8_MAX) {
    this->error("Too many constants in one chunk!");
    return 0;
  }
  return offset;
}

void Compiler::grouping() {
  this->expression();
  consume(TokenType::RIGHT_PAREN, "Expected ')' after expression.");
}

void Compiler::unary() {
  TokenType operatorType = this->parser.getPrevious().type;

  // compile the operand
  parsePrecedence(Precedence::UNARY);

  switch (operatorType) {
  case TokenType::MINUS:
    emitByte(OP_NEGATE);
    break;
  case TokenType::BANG:
    emitByte(OP_NOT);
    break;
  default:
    break;
  }
}

void Compiler::parsePrecedence(Precedence precedence) {
  this->advance();
  Token tmp = parser.getPrevious();
  Compiler::ParseFn prefixRule = getRule(tmp.type).prefix;
  if (prefixRule == NULL) {
    error("Expect expression");
    return;
  }

  std::invoke(prefixRule, this);

  while (precedence <= getRule(parser.getCurrent().type).precedence) {
    this->advance();
    Compiler::ParseFn infixRule = getRule(parser.getPrevious().type).infix;
    std::invoke(infixRule, this);
  }
}

Compiler::ParseRule Compiler::getRule(TokenType type) {
  Compiler::ParseRule p = *(&this->rules.at(static_cast<int>(type)));
  return p;
}

void Compiler::expression() { parsePrecedence(Precedence::ASSIGNMENT); }

// Adds constant opcode followed by offset into constant vector;
void Compiler::emitConstant(Value value) {
  auto cVecOffset = this->makeConstant(value);
  emitByte(OP_CONSTANT, cVecOffset);
}

void Compiler::number() {
  double value = std::stoi(this->parser.getPrevious().token.data());
  emitConstant(Value(value));
}

std::shared_ptr<std::string> Compiler::copyString(Token token) {
  std::shared_ptr<std::string> tmp = std::make_shared<std::string>(token.token);
  std::cout << "token: " << token.token << std::endl;
  return tmp;
}

void Compiler::string() {
  emitConstant(Value(this->copyString(this->parser.getPrevious())));
}

void Compiler::binary() {
  TokenType operatorType = this->parser.getPrevious().type;
  ParseRule rule = getRule(operatorType);
  this->parsePrecedence(static_cast<Precedence>(
      static_cast<int>(rule.precedence) + 1)); // gets next precedence

  switch (operatorType) {
  case TokenType::PLUS:
    emitByte(OP_ADD);
    break;
  case TokenType::MINUS:
    emitByte(OP_SUBTRACT);
    break;
  case TokenType::STAR:
    emitByte(OP_MULTIPLY);
    break;
  case TokenType::SLASH:
    emitByte(OP_DIVIDE);
    break;
  case TokenType::BANG_EQUAL:
    emitByte(OP_EQUAL, OP_NOT);
    break;
  case TokenType::EQUAL_EQUAL:
    emitByte(OP_EQUAL);
    break;
  case TokenType::LESS:
    emitByte(OP_LESS);
    break;
  case TokenType::GREATER:
    emitByte(OP_GREATER);
    break;
  case TokenType::LESS_EQUAL:
    emitByte(OP_GREATER, OP_NOT);
    break;
  case TokenType::GREATER_EQUAL:
    emitByte(OP_LESS, OP_NOT);
    break;

  default:
    return; // TODO: refactor to throw an error
  }
}

void Compiler::literal() {
  switch (this->parser.getPrevious().type) {
  case TokenType::FALSE:
    emitByte(OP_FALSE);
    break;
  case TokenType::TRUE:
    emitByte(OP_TRUE);
    break;
  case TokenType::NIL:
    emitByte(OP_NIL);
    break;
  default:
    throw std::logic_error(
        "Invalid opCode passed to Compiler::literal()"); // unreachable
  }
}
// Creates the parse table rules
std::array<Compiler::ParseRule, TokenTypeCardinality()>
Compiler::buildParseTable() {
  std::array<Compiler::ParseRule, TokenTypeCardinality()> tmp{};

  /* Compiling Expressions rules < Calls and Functions infix-left-paren
        [TOKEN_LEFT_PAREN]    = {grouping, NULL,   PREC_NONE},
      */
  //> Calls and Functions infix-left-paren
  tmp.at(static_cast<int>(TokenType::LEFT_PAREN)) = {&Compiler::grouping, NULL,
                                                     Precedence::NONE};
  //< Calls and Functions infix-left-paren
  tmp.at(static_cast<int>(TokenType::RIGHT_PAREN)) =
      ParseRule{.prefix = NULL, .infix = NULL, .precedence = Precedence::NONE};
  tmp.at(static_cast<int>(TokenType::LEFT_BRACE)) = {NULL, NULL,
                                                     Precedence::NONE}; // [big]
  tmp.at(static_cast<int>(TokenType::RIGHT_BRACE)) = {NULL, NULL,
                                                      Precedence::NONE};
  tmp.at(static_cast<int>(TokenType::COMMA)) = {NULL, NULL, Precedence::NONE};
  /* Compiling Expressions rules < Classes and Instances table-dot
    [TOKEN_DOT]           = {NULL,     NULL,   PREC_NONE},
  */
  //> Classes and Instances table-dot
  /* [static_cast<int>(TokenType::DOT)] = {NULL, &Compiler::dot, */
  /*                                       Precedence::CALL}, */
  //< Classes and Instances table-dot
  tmp.at(static_cast<int>(TokenType::MINUS)) = {
      &Compiler::unary, &Compiler::binary, Precedence::TERM};
  tmp.at(static_cast<int>(TokenType::PLUS)) = {NULL, &Compiler::binary,
                                               Precedence::TERM};
  tmp.at(static_cast<int>(TokenType::SEMICOLON)) = {NULL, NULL,
                                                    Precedence::NONE};
  tmp.at(static_cast<int>(TokenType::SLASH)) = {NULL, &Compiler::binary,
                                                Precedence::FACTOR};
  tmp.at(static_cast<int>(TokenType::STAR)) = {NULL, &Compiler::binary,
                                               Precedence::FACTOR};
  /* Compiling Expressions rules < Types of Values table-not
    [static_cast<int>(TokenType::BANG)]          = {NULL,     NULL,
    Precedence::NONE},
  */
  //> Types of Values table-not
  tmp.at(static_cast<int>(TokenType::BANG)) = {
      &Compiler::unary, NULL, Precedence::NONE}; //< Types of Values table-not
  /* Compiling Expressions rules < Types of Values table-equal
    [static_cast<int>(TokenType::BANG_EQUAL)]    = {NULL,     NULL,
    Precedence::NONE},
  */
  //> Types of Values table-equal
  tmp.at(static_cast<int>(TokenType::BANG_EQUAL)) = {NULL, &Compiler::binary,
                                                     Precedence::EQUALITY};
  //< Types of Values table-equal
  tmp.at(static_cast<int>(TokenType::EQUAL)) = {NULL, NULL, Precedence::NONE};
  /* Compiling Expressions rules < Types of Values table-comparisons
    [static_cast<int>(TokenType::EQUAL_EQUAL)]   = {NULL,     NULL,
    Precedence::NONE}, [static_cast<int>(TokenType::GREATER)]       =
    {NULL, NULL,   Precedence::NONE},
    [static_cast<int>(TokenType::GREATER_EQUAL)] = {NULL,     NULL,
    Precedence::NONE}, [static_cast<int>(TokenType::LESS)] = {NULL, NULL,
    Precedence::NONE}, [static_cast<int>(TokenType::LESS_EQUAL)]    =
    {NULL, NULL, Precedence::NONE},
  */
  //> Types of Values table-comparisons
  tmp.at(static_cast<int>(TokenType::EQUAL_EQUAL)) = {NULL, &Compiler::binary,
                                                      Precedence::EQUALITY};
  tmp.at(static_cast<int>(TokenType::GREATER)) = {NULL, &Compiler::binary,
                                                  Precedence::COMPARISON};
  tmp.at(static_cast<int>(TokenType::GREATER_EQUAL)) = {NULL, &Compiler::binary,
                                                        Precedence::COMPARISON};
  tmp.at(static_cast<int>(TokenType::LESS)) = {NULL, &Compiler::binary,
                                               Precedence::COMPARISON};
  tmp.at(static_cast<int>(TokenType::LESS_EQUAL)) = {NULL, &Compiler::binary,
                                                     Precedence::COMPARISON};
  //< Types of Values table-comparisons
  /* Compiling Expressions rules < Global Variables table-identifier
    [static_cast<int>(TokenType::IDENTIFIER)]    = {NULL,     NULL,
    Precedence::NONE},
  */
  //> Global Variables table-identifier
  /* [static_cast<int>(TokenType::IDENTIFIER)] = {&Compiler::variable,
     NULL, */
  /*                                              Precedence::NONE}, */
  //< Global Variables table-identifier
  /* Compiling Expressions rules < Strings table-string
    [static_cast<int>(TokenType::STRING)]        = {NULL,     NULL,
    Precedence::NONE},
  */
  //> Strings table-string
  tmp.at(static_cast<int>(TokenType::STRING)) = {&Compiler::string, NULL,
                                                 Precedence::NONE};
  //< Strings table-string
  tmp.at(static_cast<int>(TokenType::NUMBER)) = {&Compiler::number, NULL,
                                                 Precedence::NONE};
  /* Compiling Expressions rules < Jumping Back and Forth table-and
    [static_cast<int>(TokenType::AND)]           = {NULL,     NULL,
    Precedence::NONE},
  */
  //> Jumping Back and Forth table-and
  /* [static_cast<int>(TokenType::AND)] = {NULL, &Compiler::and_, */
  /*                                       Precedence::AND}, */
  //< Jumping Back and Forth table-and
  tmp.at(static_cast<int>(TokenType::CLASS)) = {NULL, NULL, Precedence::NONE};
  tmp.at(static_cast<int>(TokenType::ELSE)) = {NULL, NULL, Precedence::NONE};
  /* Compiling Expressions rules < Types of Values table-false
    [static_cast<int>(TokenType::FALSE)]         = {NULL,     NULL,
    Precedence::NONE},
  */
  //> Types of Values table-false
  tmp.at(static_cast<int>(TokenType::FALSE)) = {&Compiler::literal, NULL,
                                                Precedence::NONE};
  //< Types of Values table-false
  tmp.at(static_cast<int>(TokenType::FOR)) = {NULL, NULL, Precedence::NONE};
  tmp.at(static_cast<int>(TokenType::FUN)) = {NULL, NULL, Precedence::NONE};
  tmp.at(static_cast<int>(TokenType::IF)) = {NULL, NULL, Precedence::NONE};
  /* Compiling Expressions rules < Types of Values table-nil
    [static_cast<int>(TokenType::NIL)]           = {NULL,     NULL,
    Precedence::NONE},
  */
  //> Types of Values table-nil
  tmp.at(static_cast<int>(TokenType::NIL)) = {&Compiler::literal, NULL,
                                              Precedence::NONE};
  //< Types of Values table-nil
  /* Compiling Expressions rules < Jumping Back and Forth table-or
    [static_cast<int>(TokenType::OR)]            = {NULL,     NULL,
    Precedence::NONE},
  */
  //> Jumping Back and Forth table-or
  /* [static_cast<int>(TokenType::OR)] = {NULL, &Compiler::or_, */
  /*                                      Precedence::OR}, */
  //< Jumping Back and Forth table-or
  tmp.at(static_cast<int>(TokenType::PRINT)) = {NULL, NULL, Precedence::NONE};
  tmp.at(static_cast<int>(TokenType::RETURN)) = {NULL, NULL, Precedence::NONE};
  /* Compiling Expressions rules < Superclasses table-super
    [static_cast<int>(TokenType::SUPER)]         = {NULL,     NULL,
    Precedence::NONE},
  */
  //> Superclasses table-super
  /* [static_cast<int>(TokenType::SUPER)] = {&Compiler::super_, NULL, */
  /*                                         Precedence::NONE}, */
  //< Superclasses table-super
  /* Compiling Expressions rules < Methods and Initializers table-this
    [static_cast<int>(TokenType::THIS)]          = {NULL,     NULL,
    Precedence::NONE},
  */
  //> Methods and Initializers table-this
  /* [static_cast<int>(TokenType::THIS)] = {&Compiler::this_, NULL, */
  /*                                        Precedence::NONE}, */
  //< Methods and Initializers table-this
  /* Compiling Expressions rules < Types of Values table-true
    [static_cast<int>(TokenType::TRUE)]          = {NULL,     NULL,
    Precedence::NONE},
  */
  //> Types of Values table-true
  tmp.at(static_cast<int>(TokenType::TRUE)) = {&Compiler::literal, NULL,
                                               Precedence::NONE};
  //< Types of Values table-true
  tmp.at(static_cast<int>(TokenType::VAR)) = {NULL, NULL, Precedence::NONE};
  tmp.at(static_cast<int>(TokenType::WHILE)) = {NULL, NULL, Precedence::NONE};
  tmp.at(static_cast<int>(TokenType::ERROR)) = {NULL, NULL, Precedence::NONE};
  tmp.at(static_cast<int>(TokenType::EoF)) = {NULL, NULL, Precedence::NONE};
  return tmp;
}

Compiler::Compiler(std::string const &source, Chunk &chunk)
    : parser{Parser()}, scanner{source}, chunk{chunk} {
  this->rules = this->buildParseTable();
  this->advance(); // initial setup parser.
  this->expression();
  consume(TokenType::EoF, "Expected end of expression.");
  this->emitReturn();
#ifdef DEBUG_PRINT_CODE
  disassembleChunk(this->chunk, "code");
#endif
}
