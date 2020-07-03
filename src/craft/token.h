#ifndef COMPILATION_CRAFT_TOKEN_H_
#define COMPILATION_CRAFT_TOKEN_H_
#include "token_type.h"
#include <string>
#include <memory>
using std::shared_ptr;
using std::string;
class Token;
using TokenPtr = shared_ptr<Token>;

class Token {
 public:
  TokenType GetType() const {
    return type_;
  }
  string GetText() const {
    return text_;
  }
  void SetType(TokenType type) {
    type_ = type;
  }
  void SetText(const string &text) {
    text_ = text;
  }
 private:
  TokenType type_;
  string text_;
};

inline bool operator==(TokenType type, const TokenPtr &token) {
  return token != nullptr && token->GetType() == type;
}

#endif //COMPILATION_CRAFT_TOKEN_H_
