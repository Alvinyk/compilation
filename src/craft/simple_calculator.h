#ifndef COMPILATION_SRC_CRAFT_SIMPLE_CALCULATOR_H_
#define COMPILATION_SRC_CRAFT_SIMPLE_CALCULATOR_H_
#include "ast_node_type.h"
#include "ast_node.h"
#include "token_reader.h"
#include "simple_lexer.h"
#include <exception>
#include <iostream>
using std::atoi;
using std::exception;

class SimpleCalculator {
 public:

  static void Evaluate(const string &script) {
    auto tree = ParseScript(script);
    tree->Dump();
    Evaluate(tree);
  }

  static int Evaluate(const ASTNodePtr &node, const string &indent = "") {
    int result = 0;
    cout << indent << "Calculating: " << node->GetType() << endl;
    switch ( node->GetType()) {
      case ASTNodeType::Program:
        for (const auto &child : node->GetChildren()) {
          result = Evaluate(child, indent + "\t");
        }
        break;
      case ASTNodeType::Additive:
      case ASTNodeType::Subtraction:
      case ASTNodeType::Multiplicative:
      case ASTNodeType::Division: {
        auto child_left = node->GetChildren()[0];
        auto child_right = node->GetChildren()[1];
        int value_left = Evaluate(child_left, indent + "\t");
        int value_right = Evaluate(child_right, indent + "\t");
        result = GetCalculatorResult(value_left, value_right, node->GetType());
      }
        break;
      case ASTNodeType::IntLiteral:result = atoi(node->GetText().c_str());
        break;
      default:break;
    }
    cout << indent << "Result: " << result << endl;
    return result;
  }

  static int GetCalculatorResult(int value_left, int value_right, ASTNodeType type) {
    switch ( type ) {
      case ASTNodeType::Additive:return value_left + value_right;
      case ASTNodeType::Subtraction:return value_left - value_right;
      case ASTNodeType::Multiplicative: return value_left * value_right;
      case ASTNodeType::Division: return value_left / value_right;
      default:break;
    }
    throw exception("not support node type");
  }

  /*
   * 解析脚本，并返回根节点
   */
  static ASTNodePtr ParseScript(const string &script) {
    return ParseRoot(SimpleLexer().Tokenize(script));
  }

  /*
   * 语法解析：解析根节点
   */
  static ASTNodePtr ParseRoot(const TokenReaderPtr &reader) {
    SimpleASTNodePtr node = make_shared<SimpleASTNode>(ASTNodeType::Program, "Calculator");
    SimpleASTNodePtr child = Additive(reader);
    if (child != nullptr) {
      node->AddChild(child);
    }

    return node;
  }
/*
   * 解析整形变量声明语句
   * 目前不支持一行内定义多个变量 如：int a, b;
   * int a;
   * int b = 2 * 3;
   */
  static ASTNodePtr IntDeclare(const TokenReaderPtr &reader) {
    SimpleASTNodePtr node = nullptr;
    auto token = reader->Front();
    if (TokenType::Int == token) {
      reader->Pop();

      token = reader->Front();
      if (TokenType::Identifier == token) {
        reader->Pop();
        node = make_shared<SimpleASTNode>(ASTNodeType::IntDeclaration, token->GetText());

        token = reader->Front();
        if (TokenType::Assignment == token) {
          reader->Pop();

          //匹配表达式
          auto child = Additive(reader);
          if (child != nullptr) {
            node->AddChild(child);
          } else {
            throw exception("invalid variable initialization, expecting an expression");
          }
        }
      } else {
        throw exception(" expecting an variable name");
      }

      //匹配语句结束符
      if (node != nullptr) {
        token = reader->Front();
        if (TokenType::SemiColon == token) {
          reader->Pop();
        } else {
          throw exception("invalid statement, excepting semicolon");
        }
      }
    }
    return node;
  }

  /*
   * 语法解析：解析加法表达式
   */
  static SimpleASTNodePtr Additive(const TokenReaderPtr &reader) {
    SimpleASTNodePtr node = nullptr;
    SimpleASTNodePtr child_left = Multiplicative(reader);
    node = child_left;

    auto token = reader->Front();
    if (TokenType::Plus == token || TokenType::Minus == token) {
      reader->Pop();
      SimpleASTNodePtr child_right = Additive(reader);
      if (child_right != nullptr) {
        ASTNodeType node_type = ToNodeType(token->GetType());
        node = make_shared<SimpleASTNode>(node_type, token->GetText());
        node->AddChild(child_left);
        node->AddChild(child_right);
      } else {
        throw exception("invalid additive expression, expecting the right part");
      }
    }

    return node;
  }

  /*
   * 语法解析：解析乘法表达式
   */
  static SimpleASTNodePtr Multiplicative(const TokenReaderPtr &reader) {
    SimpleASTNodePtr node = nullptr;
    SimpleASTNodePtr child_left = Primary(reader);
    node = child_left;

    auto token = reader->Front();
    if (TokenType::Star == token || TokenType::Slash == token) {
      reader->Pop();
      auto child_right = Multiplicative(reader);
      if (child_right != nullptr) {
        ASTNodeType node_type = ToNodeType(token->GetType());
        node = make_shared<SimpleASTNode>(node_type, token->GetText());
        node->AddChild(child_left);
        node->AddChild(child_right);
      } else {
        throw exception("invalid multiplication expression, expecting the right part");
      }
    }
    return node;
  }

  /*
   * 语法解析：解析基础表达式
   */
  static SimpleASTNodePtr Primary(const TokenReaderPtr &reader) {
    SimpleASTNodePtr node = nullptr;
    auto token = reader->Front();
    if (TokenType::IntLiteral == token) {
      node = make_shared<SimpleASTNode>(ASTNodeType::IntLiteral, token->GetText());
      reader->Pop();
    } else if (TokenType::Identifier == token) {
      node = make_shared<SimpleASTNode>(ASTNodeType::Identifier, token->GetText());
      reader->Pop();
    } else if (TokenType::LeftParen == token) {
      return ParseParen(reader);
    }
    return node;
  }

  /*
   * 语法解析：解析括号内的表达式
   */
  static SimpleASTNodePtr ParseParen(const TokenReaderPtr &reader) {
    SimpleASTNodePtr node = nullptr;
    auto token = reader->Front();
    if (TokenType::LeftParen == token) {
      reader->Pop();
      node = Additive(reader);
      if (node != nullptr) {
        token = reader->Front();
        if (TokenType::RightParen == token) {
          reader->Pop();
        } else {
          throw exception("excepting right parenthesis");
        }
      } else {
        throw exception("expecting an additive expression inside parenthesis");
      }
    }
    return node;
  }

  /*
   * 将token的类型转换为node类型，仅支持部分操作符
   */
  static ASTNodeType ToNodeType(TokenType type) {
    switch ( type ) {
      case TokenType::Plus:return ASTNodeType::Additive;
      case TokenType::Minus:return ASTNodeType::Subtraction;
      case TokenType::Star:return ASTNodeType::Multiplicative;
      case TokenType::Slash:return ASTNodeType::Division;
      default:break;
    }
    throw exception("not support token type");
  }
};
#endif //COMPILATION_SRC_CRAFT_SIMPLE_CALCULATOR_H_
