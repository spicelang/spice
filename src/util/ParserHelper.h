// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <ast/ASTNodes.h>
#include <exception/ParserError.h>
#include <util/CodeLoc.h>

#include "antlr4-runtime.h"

namespace spice::compiler {

// Constants
#define ERROR_MESSAGE_CONTEXT 20
const char *const RESERVED_KEYWORDS[] = {"new", "switch", "case", "yield", "stash", "pick", "sync", "class"};

// Type defs
template <typename T> using Ptr = std::unique_ptr<T>;

class ParserHelper {
public:
  // Constructors
  ParserHelper() = default;

  // Public methods
  template <typename T> Ptr<T> createNode(const antlr4::ParserRuleContext *ctx) {
    ASTNode *currentParent = nullptr;
    if constexpr (!std::is_same_v<T, EntryNode>)
      currentParent = parentStack.top();
    Ptr<T> node = std::make_unique<T>(currentParent, getCodeLoc(ctx));
    node->reserveChildren(ctx->children.size());
    if constexpr (!std::is_same_v<T, EntryNode>)
      currentParent->addChild(node.get());
    pushParent(node.get());
    return node;
  }
  template <typename T> void concludeNode(const antlr4::ParserRuleContext *ctx, const Ptr<T> &node) {
    // constexpr (!std::is_same_v<T, EntryNode>)
    //  saveErrorMessage(node.get(), ctx);
    popParent(node.get());
  }
  void pushParent(ASTNode *node) { parentStack.push(node); }
  void popParent(ASTNode *node) {
    assert(parentStack.top() == node);
    parentStack.pop();
  }
  static CodeLoc getCodeLoc(const antlr4::ParserRuleContext *ctx) {
    return CodeLoc(ctx->start, ctx->start->getInputStream()->getSourceName());
  }
  int32_t parseInt(ConstantNode *constantNode, antlr4::tree::TerminalNode *terminal);
  int16_t parseShort(ConstantNode *constantNode, antlr4::tree::TerminalNode *terminal);
  int64_t parseLong(ConstantNode *constantNode, antlr4::tree::TerminalNode *terminal);
  static int8_t parseChar(ConstantNode *constantNode, antlr4::tree::TerminalNode *terminal);
  static std::string parseString(std::string input);
  template <typename T>
  T parseNumeric(ConstantNode *constantNode, antlr4::tree::TerminalNode *terminal, std::function<T(const std::string &, int)> cb);
  static void saveErrorMessage(ASTNode *node, const antlr4::ParserRuleContext *ctx);
  static void replaceEscapeChars(std::string &string);
  static std::string getIdentifier(antlr4::tree::TerminalNode *terminal);

private:
  // Private members
  std::stack<ASTNode *> parentStack;
};

} // namespace spice::compiler