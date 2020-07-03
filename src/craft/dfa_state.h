#ifndef COMPILATION_SRC_CRAFT_DFS_STATE_H_
#define COMPILATION_SRC_CRAFT_DFS_STATE_H_
namespace DFA{
enum DfaState{
  Initial,
  If, Id_if1, Id_if2,
  Else, Id_else1, Id_else2, Id_else3, Id_else4,
  Int, Id_int1, Id_int2, Id_int3,

  Id,

  GT, GE, EQ, LE, LT,

  Assignment,

  Plus, Minus, Star, Slash,

  SemiColon,
  LeftParen,
  RightParen,

  IntLiteral
};

}

#endif //COMPILATION_SRC_CRAFT_DFS_STATE_H_
