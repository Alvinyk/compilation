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
  //������ڣ����ڵ�
  Program,
  //���ͱ�������
  IntDeclaration,
  //���ʽ��䣬�����ʽ��������ֺ�
  ExpressionStmt,
  //��ֵ���
  AssignmentStmt,
  //�������ʽ
  Primary,
  //�˷����ʽ
  Multiplicative,
  //�������ʽ
  Division,
  //�������ʽ
  Subtraction,
  //�ӷ����ʽ
  Additive,
  //��ʶ��
  Identifier,
  //����������
  IntLiteral,
  //���������
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
 * ��token������ת��Ϊnode���ͣ���֧�ֲ��ֲ�����
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
 * ��ȡ˫Ŀ������ļ�����
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
