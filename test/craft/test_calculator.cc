#include "craft/simple_calculator.h"

int main() {
  SimpleLexer lexer;
  string script = "int a = b + 3;";
  cout << "��������������� : { " << script << " }" << endl;
  auto token_reader = lexer.Tokenize(script);
  auto node = SimpleCalculator::IntDeclare(token_reader);
  node->Dump();
  cout << endl << endl;

  script = "2+3*5";
  cout<<"���㣺"<<script<<", ����ȥһ������"<<endl;
  SimpleCalculator::Evaluate(script);
  cout << endl << endl;

  script = "2+3+4";
  cout<<"���㣺"<<script<<", ����Գ��ִ���"<<endl;
  SimpleCalculator::Evaluate(script);
  cout<<endl<<endl;

  try{
    script = "2+";
    cout<<"���㣺"<<script<<", Ӧ�����﷨����"<<endl;
    SimpleCalculator::Evaluate(script);
    cout<<endl<<endl;
  } catch (const exception& e) {
    cout<<e.what()<<endl;
  }
}