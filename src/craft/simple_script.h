#ifndef COMPILATION_SRC_CRAFT_SIMPLE_SCRIPT_H_
#define COMPILATION_SRC_CRAFT_SIMPLE_SCRIPT_H_
#include <iostream>
#include "simple_parse.h"
using std::cin;

/**
 * 一个简单的脚本解释器。
 * 所支持的语法，请参见SimpleParser.java
 *
 * 运行脚本：
 * 在命令行下，键入：java SimpleScript
 * 则进入一个REPL界面。你可以依次敲入命令。比如：
 * > 2+3;
 * > int age = 10;
 * > int b;
 * > b = 10*2;
 * > age = age + b;
 * > exit();  //退出REPL界面。
 *
 * 你还可以使用一个参数 -v，让每次执行脚本的时候，都输出AST和整个计算过程。
 *
 */
class SimpleScript {
 public:
  explicit SimpleScript(bool verbose = false) {
    verbose_ = verbose;
    cout << "verbose mode" << endl;
    cout << "Simple script language" << endl;
  }

  void Run() {
    cout << ">> ";
    string script;

    while (true) {
      try {
        script.clear();
        getline(cin, script);
        Trim(script);
        if(!script.empty()){
          if (script == "exit()") {
            cout << "good bye!" << endl;
            break;
          }

          if (script.back() == ';') {
            auto tree = SimpleParse::ParseScript(script);
            if (verbose_){
              tree->Dump();
            }
            Evaluate(tree);
          }else{
            cout<<"script must end with ; "<<endl;
          }
        }

        cout<<endl<<">>";

      } catch (const exception &e) {
        cout << e.what() << endl << endl << ">> ";
      }
    }

  }

 private:
  /*
   * 遍历AST节点，并计算值
   */
  int Evaluate(const ASTNodePtr &node, const string &indent = "") {
    int result = 0;
    if (verbose_) {
      cout << indent << "Calculating: " << node->GetType() << endl;
    }

    switch ( node->GetType()) {
      case ASTNodeType::Program:
        for (const auto &child : node->GetChildren()) {
          result = Evaluate(child, indent);
        }
        break;
      case ASTNodeType::Negative:
        result = -1 * Evaluate(node->GetChildren()[0], indent + '\t');
        break;
      case ASTNodeType::Additive:
      case ASTNodeType::Subtraction:
      case ASTNodeType::Multiplicative:
      case ASTNodeType::Division: {
        auto left = node->GetChildren()[0];
        auto right = node->GetChildren()[1];
        auto value_left = Evaluate(left, indent + '\t');
        auto value_right = Evaluate(right, indent + '\t');
        result = GetResult(value_left, value_right, node->GetType());
      }
        break;
      case ASTNodeType::IntLiteral:result = atoi(node->GetText().c_str());
        break;
      case ASTNodeType::Identifier:result = GetVariableValue(node->GetText());
        break;
      case ASTNodeType::AssignmentStmt:CheckVariableExist(node->GetText());
        //复用下面的设值代码
      case ASTNodeType::IntDeclaration: {
        if (!node->GetChildren().empty()) {
          result = Evaluate(node->GetChildren()[0],indent+'\t');
        }
        SetVariableValue(node->GetText(), result);
      }
        break;
      default:break;
    }

    if (verbose_) {
      cout << indent << " Result: " << result << endl;
    } else if (indent.empty()) {
      if (ASTNodeType::IntDeclaration == node->GetType() || ASTNodeType::AssignmentStmt == node->GetType()) {
        cout << node->GetText() << " : " << result << endl;
      } else if (node->GetType() != ASTNodeType::Program) {
        cout << result << endl;
      }
    }
    return result;
  }

  int GetVariableValue(const string &variable) {
    CheckVariableExist(variable);
    return variable_table_[variable];
  }

  void SetVariableValue(const string &variable, int value) {
    variable_table_[variable] = value;
  }

  void CheckVariableExist(const string &variable) {
    if (variable_table_.count(variable) < 0) {
      throw exception(("unknown variable: " + variable).c_str());
    }
  }

  static void Trim(string &s) {
    if (!s.empty()) {
      s.erase(0, s.find_first_not_of(' '));
      s.erase(s.find_last_not_of(' ') + 1);
    }
  }
 private:
  bool verbose_ = false;
  map<string, int> variable_table_;
};
#endif //COMPILATION_SRC_CRAFT_SIMPLE_SCRIPT_H_
