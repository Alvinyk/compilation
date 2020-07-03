#ifndef COMPILATION_CRAFT_TOKEN_TYPE_H_
#define COMPILATION_CRAFT_TOKEN_TYPE_H_
#include "common.h"
#include <iostream>
#include <map>
#include <string>
using std::string;
using std::map;
using std::ostream;

namespace TOKEN {
enum TokenType {
  Plus,   // +
  Minus,  // -
  Star,   // *
  Slash,  // /

  GE,     // >=
  GT,     // >
  EQ,     // ==
  LE,     // <=
  LT,     // <

  SemiColon, // ;
  LeftParen, // (
  RightParen,// )

  Assignment,// =

  If,
  Else,

  Int,

  Identifier,     //标识符

  IntLiteral,     //整型字面量
  StringLiteral   //字符串字面量
};
map<TokenType, string> type_name{
    Enum2String(Plus),
    Enum2String(Minus),
    Enum2String(Star),
    Enum2String(Slash),
    Enum2String(GE),
    Enum2String(GT),
    Enum2String(EQ),
    Enum2String(LE),
    Enum2String(LT),
    Enum2String(SemiColon),
    Enum2String(LeftParen),
    Enum2String(RightParen),
    Enum2String(Assignment),
    Enum2String(If),
    Enum2String(Else),
    Enum2String(Int),
    Enum2String(Identifier),
    Enum2String(IntLiteral),
    Enum2String(StringLiteral)
};
}

using TokenType = TOKEN::TokenType;
ostream &operator<<(ostream &stream, TokenType type) {
  stream << TOKEN::type_name[type];
  return stream;
}
#endif //COMPILATION_CRAFT_TOKEN_TYPE_H_
