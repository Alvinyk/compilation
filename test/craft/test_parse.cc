#include "craft/simple_parse.h"
#include <string>
using std::string;

void ParseScript(const string &script) {
  ASTNodePtr tree = nullptr;
  try {
    cout << "Parse: " << script << endl;
    tree = SimpleParse::ParseScript(script);
    tree->Dump();
  } catch (const exception &e) {
    cout << e.what() << endl;
  }

}

int main() {
  string scripts[] = {
      "int age = 45 + 2; age = 20; age + 10 * 2;",
      "2 + 3+ 4;",
      "2 + 3 + ;",
      "2 + 3 * ;",
      "2 + (-3);"
  };

  for (const string &script: scripts) {
    ParseScript(script);
    cout << endl << endl;
  }
}