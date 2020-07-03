#ifndef COMPILATION_CRAFT_TOKEN_READER_H_
#define COMPILATION_CRAFT_TOKEN_READER_H_
#include "token.h"
#include <vector>
#include <memory>
#include <iostream>
using std::cout;
using std::endl;
using std::shared_ptr;
using std::make_shared;
using std::vector;

class TokenReader;
using TokenReaderPtr = shared_ptr<TokenReader>;

class TokenReader {
 public:
  explicit TokenReader(vector<TokenPtr> &tokens) {
    tokens_ = tokens;
    pos_ = 0;
  }

  void Pop() {
    ++pos_;
  }

  TokenPtr Front() const {
    return pos_ < tokens_.size() ? tokens_[pos_] : nullptr;
  }

  void UnRead() {
    if (pos_ > 0) {
      --pos_;
    }
  }

  int GetPosition() const {
    return pos_;
  }

  void SetPosition(int pos) {
    if (pos >= 0 && pos < tokens_.size()) {
      pos_ = pos;
    }
  }

  void Dump() const {
    cout << "text\ttype" << endl;
    for (const auto &token : tokens_) {
      cout << token->GetText() << "\t" << token->GetType() << endl;
    }
  }
 private:
  vector<TokenPtr> tokens_;
  int pos_;
};

#endif //COMPILATION_CRAFT_TOKEN_READER_H_
