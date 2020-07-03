#ifndef COMPILATION_CRAFT_AST_NODE_H_
#define COMPILATION_CRAFT_AST_NODE_H_
#include "ast_node_type.h"
#include <string>
#include <vector>
#include <memory>
#include <iostream>
using std::cout;
using std::endl;
using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::string;

class ASTNode;
class SimpleASTNode;

using SimpleASTNodePtr = shared_ptr<SimpleASTNode>;
using ASTNodePtr = shared_ptr<ASTNode>;
using ASTNodeType = AST::ASTNodeType;

class ASTNode {
 public:
  virtual ASTNodePtr GetParent() const = 0;
  virtual vector<ASTNodePtr> GetChildren() const = 0;
  virtual ASTNodeType GetType() const = 0;
  virtual string GetText() const = 0;
  virtual void Dump(string indent="") const = 0;
};

class SimpleASTNode : public ASTNode, public std::enable_shared_from_this<SimpleASTNode> {
 public:
  SimpleASTNode(ASTNodeType type, const string &text) {
    node_type_ = type;
    text_ = text;
  }

  ASTNodePtr GetParent() const override {
    return parent_;
  }

  vector<ASTNodePtr> GetChildren() const override {
    return children_;
  }

  ASTNodeType GetType() const override {
    return node_type_;
  }

  string GetText() const override {
    return text_;
  }

  void Dump(string indent = "") const override {
    cout << indent << node_type_ << " " << text_ << endl;
    for (const auto& child : children_) {
      child->Dump(indent + "\t");
    }
  }

  void AddChild(const SimpleASTNodePtr &node) {
    children_.emplace_back(node);
    parent_ = shared_from_this();
  }
 private:
  ASTNodePtr parent_{};
  vector<ASTNodePtr> children_;
  ASTNodeType node_type_;
  string text_;
};

#endif //COMPILATION_CRAFT_AST_NODE_H_
