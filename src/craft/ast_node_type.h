#ifndef COMPILATION_CRAFT_AST_NODE_TYPE_H_
#define COMPILATION_CRAFT_AST_NODE_TYPE_H_
#include <map>
#include <ostream>
#include <string>
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
  IntLiteral
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
    Enum2String(IntLiteral)
};
}

inline ostream &operator<<(ostream &stream, AST::ASTNodeType type) {
  stream << AST::type_name[type];
  return stream;
}
#endif //COMPILATION_CRAFT_AST_NODE_TYPE_H_
