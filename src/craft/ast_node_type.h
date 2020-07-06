#ifndef COMPILATION_CRAFT_AST_NODE_TYPE_H_
#define COMPILATION_CRAFT_AST_NODE_TYPE_H_
#include <map>
#include <ostream>
#include <string>
#include <exception>
#include "token_type.h"
using std::exception;
using std::map;
using std::ostream;
using std::string;
#define Enum2String(code) {code, #code}
namespace AST {
enum ASTNodeType {
  //程序入口，根节点
  Program,
  //整型变量声明
  IntDeclaration,
  //表达式语句，即表达式后面跟个分号
  ExpressionStmt,
  //赋值语句
  AssignmentStmt,
  //基础表达式
  Primary,
  //乘法表达式
  Multiplicative,
  //除法表达式
  Division,
  //减法表达式
  Subtraction,
  //加法表达式
  Additive,
  //标识符
  Identifier,
  //整型字面量
  IntLiteral,
  //负号运算符
  Negative
};

map<ASTNodeType, string> type_name{
    Enum2String(Program),
    Enum2String(IntDeclaration),
    Enum2String(ExpressionStmt),
    Enum2String(AssignmentStmt),
    Enum2String(Primary),
    Enum2String(Multiplicative),
    Enum2String(Additive),
    Enum2String(Identifier),
    Enum2String(IntLiteral),
    Enum2String(Negative),
};
}

inline ostream &operator<<(ostream &stream, AST::ASTNodeType type) {
  stream << AST::type_name[type];
  return stream;
}

/*
 * 将token的类型转换为node类型，仅支持部分操作符
 */
using ASTNodeType = AST::ASTNodeType;
static ASTNodeType ToASTNodeType(TokenType type) {
  switch ( type ) {
    case TokenType::Plus:return ASTNodeType::Additive;
    case TokenType::Minus:return ASTNodeType::Subtraction;
    case TokenType::Star:return ASTNodeType::Multiplicative;
    case TokenType::Slash:return ASTNodeType::Division;
    default:break;
  }
  throw exception("not support token type");
}

/*
 * 获取双目运算符的计算结果
 */
static int GetResult(int value_left, int value_right, ASTNodeType type) {
  switch ( type ) {
    case ASTNodeType::Additive:return value_left + value_right;
    case ASTNodeType::Subtraction:return value_left - value_right;
    case ASTNodeType::Multiplicative: return value_left * value_right;
    case ASTNodeType::Division: return value_left / value_right;
    default:break;
  }
  throw exception("not support node type");
}
#endif //COMPILATION_CRAFT_AST_NODE_TYPE_H_
