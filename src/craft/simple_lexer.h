#ifndef COMPILATION_CRAFT_SIMPLE_LEXER_H_
#define COMPILATION_CRAFT_SIMPLE_LEXER_H_

#include "token.h"
#include "token_reader.h"
#include "dfa_state.h"
using DfaState = DFA::DfaState;
using TokenType = TOKEN::TokenType;

class SimpleLexer {
 public:
  TokenReaderPtr Tokenize(const string &code) {
    tokens_.clear();
    token_text_.clear();
    temp_token_ = make_shared<Token>();
    DfaState state = DfaState::Initial;
    for (const char &ch : code) {
      switch ( state ) {
        case DfaState::Initial:state = InitToken(ch);
          break;
        case DfaState::Id:
          if (isalpha(ch) || isalnum(ch)) {
            token_text_ += ch;
          } else {
            state = InitToken(ch);
          }
          break;
        case DfaState::Id_int1:
          if (ch == 'n') {
            state = DfaState::Id_int2;
            token_text_ += ch;
          } else if (isalpha(ch) || isalnum(ch)) {
            state = DfaState::Id;
            token_text_ += ch;
          } else {
            state = InitToken(ch);
          }
          break;
        case DfaState::Id_int2:
          if (ch == 't') {
            state = DfaState::Id_int3;
            token_text_ += ch;
          } else if (isalpha(ch) || isalnum(ch)) {
            state = DfaState::Id;
            token_text_ += ch;
          } else {
            state = InitToken(ch);
          }
          break;
        case DfaState::Id_int3:
          if (isblank(ch)) {
            temp_token_->SetType(TokenType::Int);
            state = InitToken(ch);
          } else {
            state = DfaState::Id;
            token_text_ += ch;
          }
          break;
        case DfaState::IntLiteral:
          if (isalnum(ch)) {
            token_text_ += ch;
          } else {
            state = InitToken(ch);
          }
          break;
        case DfaState::GT:
          if (ch == '=') {
            temp_token_->SetType(TokenType::GE);
            state = DfaState::GE;
            token_text_ += ch;
          } else {
            state = InitToken(ch);
          }
          break;
        case DfaState::LT:
          if (ch == '=') {
            temp_token_->SetType(TokenType::LE);
            state = DfaState::LE;
            token_text_ += ch;
          } else {
            state = InitToken(ch);
          }
          break;
        case DfaState::GE:
        case DfaState::LE:
        case DfaState::Assignment:
        case DfaState::Plus:
        case DfaState::Minus:
        case DfaState::Star:
        case DfaState::Slash:
        case DfaState::SemiColon:
        case DfaState::LeftParen:
        case DfaState::RightParen:state = InitToken(ch);
          break;
        default:break;
      }
    }

    if (!token_text_.empty()) {
      InitToken(' ');
    }
    return make_shared<TokenReader>(tokens_);
  }

 private:
  DfaState InitToken(char ch) {
    if (!token_text_.empty()) {
      temp_token_->SetText(token_text_);
      tokens_.emplace_back(temp_token_);

      token_text_.clear();
      temp_token_ = make_shared<Token>();
    }
    DfaState new_state;

    token_text_ += ch;
    if (isalpha(ch)) {
      new_state = ch == 'i' ? DfaState::Id_int1 : DfaState::Id;
      temp_token_->SetType(TokenType::Identifier);
    } else if (isalnum(ch)) {
      new_state = DfaState::IntLiteral;
      temp_token_->SetType(TokenType::IntLiteral);
    } else {
      switch ( ch ) {
        case '>':new_state = DfaState::GT;
          temp_token_->SetType(TokenType::GT);
          break;
        case '<':new_state = DfaState::LT;
          temp_token_->SetType(TokenType::LT);
          break;
        case '=':new_state = DfaState::Assignment;
          temp_token_->SetType(TokenType::Assignment);
          break;
        case '(':new_state = DfaState::LeftParen;
          temp_token_->SetType(TokenType::LeftParen);
          break;
        case ')':new_state = DfaState::RightParen;
          temp_token_->SetType(TokenType::RightParen);
          break;
        case ';':new_state = DfaState::SemiColon;
          temp_token_->SetType(TokenType::SemiColon);
          break;
        case '+':new_state = DfaState::Plus;
          temp_token_->SetType(TokenType::Plus);
          break;
        case '-':
          new_state = DfaState::Minus;
          temp_token_->SetType(TokenType::Minus);
          break;
        case '*':new_state = DfaState::Star;
          temp_token_->SetType(TokenType::Star);
          break;
        case '/':new_state = DfaState::Slash;
          temp_token_->SetType(TokenType::Slash);
          break;
        default:token_text_.erase(token_text_.end() - 1);
          new_state = DfaState::Initial;
          break;
      }
    }

    return new_state;
  }

 private:
  string token_text_;
  vector<TokenPtr> tokens_;
  TokenPtr temp_token_;

};

#endif //COMPILATION_CRAFT_SIMPLE_LEXER_H_
