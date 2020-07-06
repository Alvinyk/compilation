#ifndef COMPILATION_SRC_CRAFT_SIMPLE_PARSE_H_
#define COMPILATION_SRC_CRAFT_SIMPLE_PARSE_H_

#include "ast_node.h"
#include "simple_lexer.h"
#include <exception>
using std::exception;

/**
 * 一个简单的语法解析器。
 * 能够解析简单的表达式、变量声明和初始化语句、赋值语句。
 * 它支持的语法规则为：
 *
 * program -> intDeclare | expressionStatement | assignmentStatement
 * intDeclare -> 'int' Id ( = additive) ';'
 * expressionStatement -> additive ';'
 * additive -> multiplicative ( (+ | -) multiplicative)*
 * multiplicative -> primary ( (* | /) primary)*
 * primary -> IntLiteral | Id | (additive)
 */
class SimpleParse {
 public:

  /*
   * 解析脚本，返回抽象树节点
   */
  static ASTNodePtr ParseScript(const string &script) {
    return ParseRoot(SimpleLexer().Tokenize(script));
  }

 private:
  /*
   * AST的根节点，解析的入口
   * statement : intDeclaration | assignmentStatement | expressionStatement;
   */
  static ASTNodePtr ParseRoot(const TokenReaderPtr &reader) {
    auto node = make_shared<SimpleASTNode>(ASTNodeType::Program, "PWC");
    while (reader->Front() != nullptr) {
      auto child = IntDeclare(reader);

      if (child == nullptr) {
        child = Assignment(reader);
      }

      if (child == nullptr) {
        child = Expression(reader);
      }

      if (child != nullptr) {
        node->AddChild(child);
      } else {
        throw exception("unknown statement");
      }
    }
    return node;
  }

  /*
   * 解析变量声明语句
   * int a ;
   * int a = 10 * 2;
   */
  static ASTNodePtr IntDeclare(const TokenReaderPtr &reader) {
    ASTNodePtr node = nullptr;
    if (TokenType::Int == reader->Front()) {
      reader->Pop();

      if (TokenType::Identifier == reader->Front()) {
        node = make_shared<SimpleASTNode>(ASTNodeType::IntDeclaration, reader->Front()->GetText());
        reader->Pop();

        if (TokenType::Assignment == reader->Front()) {
          reader->Pop();

          auto child = Additive(reader);
          if (child != nullptr) {
            node->AddChild(child);
          } else {
            throw exception("invalid variable initialization, expecting an expression");
          }
        }
      } else {
        throw exception("variable name expected ");
      }

      if (node != nullptr) {
        CheckSemiColon(reader);
      }
    }
    return node;
  }

  /*
   * 解析赋值语句
   * a = 3 + 2;
   */
  static ASTNodePtr Assignment(const TokenReaderPtr &reader) {
    ASTNodePtr node = nullptr;
    if (TokenType::Identifier == reader->Front()) {
      node = make_shared<SimpleASTNode>(ASTNodeType::AssignmentStmt, reader->Front()->GetText());
      reader->Pop();

      if (TokenType::Assignment == reader->Front()) {
        reader->Pop();

        auto child = Additive(reader);
        if (child != nullptr) {
          node->AddChild(child);
          CheckSemiColon(reader);
        } else {
          throw exception("invalid assignment statement, expecting an expression");
        }
      } else {
        reader->UnRead();
        node = nullptr;
      }
    }
    return node;
  }

  /*
   * 解析表达式
   * 3 * 2 + 5;
   */
  static ASTNodePtr Expression(const TokenReaderPtr &reader) {
    ASTNodePtr node = Additive(reader);
    if (node != nullptr) {
      CheckSemiColon(reader);
    }
    return node;
  }

  /*
   * 解析加法表达式
   * add -> mul | add '+' mul
   * add -> mul ('+' mul)*
   */
  static ASTNodePtr Additive(const TokenReaderPtr &reader) {
    auto left = Multiplicative(reader);
    auto node = left;
    if (left != nullptr) {
      while (TokenType::Plus == reader->Front() || TokenType::Minus == reader->Front()) {
        auto token = reader->Front();
        reader->Pop();

        auto right = Multiplicative(reader);
        if (right != nullptr) {
          auto type = ToASTNodeType(token->GetType());
          node = make_shared<SimpleASTNode>(type, token->GetText());
          node->AddChild(left);
          node->AddChild(right);
          left = node;
        } else {
          throw exception("invalid additive expression, expecting the right part.");
        }
      }
    }

    return node;
  }

  /*
   * 解析乘法表达式
   * mul -> pri | mul '*' pri
   * mul -> pri ('*' pri)*
   */
  static ASTNodePtr Multiplicative(const TokenReaderPtr &reader) {
    auto left = Primary(reader);
    auto node = left;
    if (left != nullptr) {
      while (TokenType::Star == reader->Front() || TokenType::Slash == reader->Front()) {
        auto token = reader->Front();
        reader->Pop();

        auto right = Primary(reader);
        if (right != nullptr) {
          auto type = ToASTNodeType(token->GetType());
          node = make_shared<SimpleASTNode>(type, token->GetText());
          node->AddChild(left);
          node->AddChild(right);
          left = node;
        } else {
          throw exception("invalid multiplicative expression, expecting the right part.");
        }
      }
    }

    return node;
  }

  /*
   * 解析基础语法
   * pri -> Id | num | (add)
   */
  static ASTNodePtr Primary(const TokenReaderPtr &reader) {
    ASTNodePtr node = nullptr;
    auto token = reader->Front();
    if (TokenType::Minus == reader->Front()) {
      node = ParseNegative(reader);
    } else if (TokenType::Identifier == reader->Front()) {
      reader->Pop();
      node = make_shared<SimpleASTNode>(ASTNodeType::Identifier, token->GetText());
    } else if (TokenType::IntLiteral == reader->Front()) {
      reader->Pop();
      node = make_shared<SimpleASTNode>(ASTNodeType::IntLiteral, token->GetText());
    } else if (TokenType::LeftParen == reader->Front()) {
      node = ParseParen(reader);
    }

    return node;
  }

  /*
   * 解析负号表达式
   * -10， --10, ---10,
   * -a, --a, ---a,
   */
  static ASTNodePtr ParseNegative(const TokenReaderPtr &reader) {
    ASTNodePtr node = nullptr;
    if(TokenType::Minus == reader->Front()){
      node = make_shared<SimpleASTNode>(ASTNodeType::Negative,reader->Front()->GetText());
      reader->Pop();

      auto child = Primary(reader);
      if(child != nullptr){
        node->AddChild(child);
      }else{
        throw exception("invalid negative expression, expecting right part.");
      }
    }

    return node;
  }

  /*
   * 解析括号内的表达式
   * (add)
   */
  static ASTNodePtr ParseParen(const TokenReaderPtr &reader) {
    ASTNodePtr node = nullptr;
    if (TokenType::LeftParen == reader->Front()) {
      reader->Pop();

      node = Additive(reader);
      if (node != nullptr) {
        if (TokenType::RightParen == reader->Front()) {
          reader->Pop();
        } else {
          throw exception("expecting right parenthesis.");
        }
      } else {
        throw exception("expecting an additive expression inside parenthesis");
      }
    }
    return node;
  }

  /*
   * 校验语句最后的分号，如果不匹配则抛出异常
   */
  static void CheckSemiColon(const TokenReaderPtr &reader) {
    if (TokenType::SemiColon == reader->Front()) {
      reader->Pop();
    } else {
      throw exception("invalid statement, expecting semicolon");
    }
  }
};

#endif //COMPILATION_SRC_CRAFT_SIMPLE_PARSE_H_
