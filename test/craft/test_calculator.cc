#include "craft/simple_calculator.h"

int main() {
  SimpleLexer lexer;
  string script = "int a = b + 3;";
  cout << "解析变量声明语句 : { " << script << " }" << endl;
  auto token_reader = lexer.Tokenize(script);
  auto node = SimpleCalculator::IntDeclare(token_reader);
  node->Dump();
  cout << endl << endl;

  script = "2+3*5";
  cout<<"计算："<<script<<", 看上去一切正常"<<endl;
  SimpleCalculator::Evaluate(script);
  cout << endl << endl;

  script = "2+3+4";
  cout<<"计算："<<script<<", 结合性出现错误。"<<endl;
  SimpleCalculator::Evaluate(script);
  cout<<endl<<endl;

  try{
    script = "2+";
    cout<<"计算："<<script<<", 应该有语法错误。"<<endl;
    SimpleCalculator::Evaluate(script);
    cout<<endl<<endl;
  } catch (const exception& e) {
    cout<<e.what()<<endl;
  }
}