#include "craft/simple_lexer.h"

int main() {
  vector<string> scripts{
      "int age = 45 ;",
      "inta age = 45 ;",
      "in age = 45 ;",
      "age > 45 ;",
      "age >= 45 ;",
      "age < 45 ;",
      "age <= 45 ;",
      "int a = b + 3 ;",
  };

  SimpleLexer lexer;
  for (const auto &script : scripts) {
    cout << "parse : { " << script << " }" << endl;
    auto token_reader = lexer.Tokenize(script);
    token_reader->Dump();
    cout << endl << endl;
  }
}