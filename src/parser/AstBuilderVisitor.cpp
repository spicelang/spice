// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "AstBuilderVisitor.h"

#include <ast/AstNodes.h>

std::any AstBuilderVisitor::visitEntry(SpiceParser::EntryContext *ctx) {
  auto entryNode = static_cast<EntryNode *>(currentNode);
  for (auto subTree : ctx->children) {
    // Create child for the current node
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::MainFunctionDefContext *>(subTree); rule != nullptr) // MainFctDef
      currentNode = entryNode->createChild<MainFctDefNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::FunctionDefContext *>(subTree); rule != nullptr) // FctDef
      currentNode = entryNode->createChild<FctDefNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::ProcedureDefContext *>(subTree); rule != nullptr) // ProcDef
      currentNode = entryNode->createChild<ProcDefNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::StructDefContext *>(subTree); rule != nullptr) // StructDef
      currentNode = entryNode->createChild<StructDefNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::GenericTypeDefContext *>(subTree); rule != nullptr) // GenericTypeDef
      currentNode = entryNode->createChild<GenericTypeDefNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::GlobalVarDefContext *>(subTree); rule != nullptr) // GlobalVarDef
      currentNode = entryNode->createChild<GlobalVarDefNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::ImportStmtContext *>(subTree); rule != nullptr) // ImportStmt
      currentNode = entryNode->createChild<ImportStmtNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::ExtDeclContext *>(subTree); rule != nullptr) // ExtDecl
      currentNode = entryNode->createChild<ExtDeclNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != entryNode) {
      visit(rule);
      currentNode = entryNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitMainFunctionDef(SpiceParser::MainFunctionDefContext *ctx) {
  auto mainFctDefNode = static_cast<MainFctDefNode *>(currentNode);
  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::ArgLstDefContext *>(subTree); rule != nullptr) // ArgLstDef
      currentNode = mainFctDefNode->createChild<ArgLstDefNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = mainFctDefNode->createChild<StmtLstNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != mainFctDefNode) {
      visit(rule);
      currentNode = mainFctDefNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitFunctionDef(SpiceParser::FunctionDefContext *ctx) {
  auto fctDefNode = static_cast<FctDefNode *>(currentNode);

  // Extract function name
  fctDefNode->functionName = ctx->IDENTIFIER().front()->getText();
  for (int i = 1; i < ctx->IDENTIFIER().size(); i++)
    fctDefNode->functionName += "." + ctx->IDENTIFIER()[i]->getText();

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DeclSpecifiersContext *>(subTree); rule != nullptr) // DeclSpecifiers
      currentNode = fctDefNode->createChild<DeclSpecifiersNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) // DataType
      currentNode = fctDefNode->createChild<DataTypeNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) // TypeLst
      currentNode = fctDefNode->createChild<TypeLstNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::ArgLstDefContext *>(subTree); rule != nullptr) // ArgLstDef
      currentNode = fctDefNode->createChild<ArgLstDefNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = fctDefNode->createChild<StmtLstNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != fctDefNode) {
      visit(rule);
      currentNode = fctDefNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) {
  auto procDefNode = static_cast<ProcDefNode *>(currentNode);

  // Extract procedure name
  procDefNode->procedureName = ctx->IDENTIFIER().front()->getText();
  for (int i = 1; i < ctx->IDENTIFIER().size(); i++)
    procDefNode->procedureName += "." + ctx->IDENTIFIER()[i]->getText();

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DeclSpecifiersContext *>(subTree); rule != nullptr) // DeclSpecifiers
      currentNode = procDefNode->createChild<DeclSpecifiersNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) // TypeLst
      currentNode = procDefNode->createChild<TypeLstNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::ArgLstDefContext *>(subTree); rule != nullptr) // ArgLstDef
      currentNode = procDefNode->createChild<ArgLstDefNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = procDefNode->createChild<StmtLstNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != procDefNode) {
      visit(rule);
      currentNode = procDefNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitStructDef(SpiceParser::StructDefContext *ctx) {
  auto structDefNode = static_cast<StructDefNode *>(currentNode);

  // Extract struct name
  structDefNode->structName = ctx->IDENTIFIER()->getText();

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DeclSpecifiersContext *>(subTree); rule != nullptr) // DeclSpecifiers
      currentNode = structDefNode->createChild<DeclSpecifiersNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) // TypeLst
      currentNode = structDefNode->createChild<TypeLstNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::FieldContext *>(subTree); rule != nullptr) // Field
      currentNode = structDefNode->createChild<FieldNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != structDefNode) {
      visit(rule);
      currentNode = structDefNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitGenericTypeDef(SpiceParser::GenericTypeDefContext *ctx) {
  auto genericTypeDefNode = static_cast<GenericTypeDefNode *>(currentNode);

  // Extract type name
  genericTypeDefNode->typeName = ctx->IDENTIFIER()->getText();

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DeclSpecifiersContext *>(subTree); rule != nullptr) // DeclSpecifiers
      currentNode = genericTypeDefNode->createChild<DeclSpecifiersNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::TypeAltsLstContext *>(subTree); rule != nullptr) // TypeAltsLst
      currentNode = genericTypeDefNode->createChild<TypeAltsLstNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != genericTypeDefNode) {
      visit(rule);
      currentNode = genericTypeDefNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitGlobalVarDef(SpiceParser::GlobalVarDefContext *ctx) {
  auto globalVarDefNode = static_cast<GlobalVarDefNode *>(currentNode);

  // Extract function name
  globalVarDefNode->varName = ctx->IDENTIFIER()->getText();

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DeclSpecifiersContext *>(subTree); rule != nullptr) // DeclSpecifiers
      currentNode = globalVarDefNode->createChild<DeclSpecifiersNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) // DataType
      currentNode = globalVarDefNode->createChild<DataTypeNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::ValueContext *>(subTree); rule != nullptr) // Value
      currentNode = globalVarDefNode->createChild<ValueNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != globalVarDefNode) {
      visit(rule);
      currentNode = globalVarDefNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitExtDecl(SpiceParser::ExtDeclContext *ctx) {
  auto extDeclNode = static_cast<ExtDeclNode *>(currentNode);

  // Extract function name
  extDeclNode->extFunctionName = ctx->IDENTIFIER()->getText();

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) // DataType
      currentNode = extDeclNode->createChild<DataTypeNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) // TypeLst
      currentNode = extDeclNode->createChild<TypeLstNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != extDeclNode) {
      visit(rule);
      currentNode = extDeclNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitThreadDef(SpiceParser::ThreadDefContext *ctx) {
  auto threadDefNode = static_cast<ThreadDefNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = threadDefNode->createChild<StmtLstNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != threadDefNode) {
      visit(rule);
      currentNode = threadDefNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitUnsafeBlockDef(SpiceParser::UnsafeBlockDefContext *ctx) {
  auto unsafeBlockDefNode = static_cast<UnsafeBlockDefNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = unsafeBlockDefNode->createChild<StmtLstNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != unsafeBlockDefNode) {
      visit(rule);
      currentNode = unsafeBlockDefNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitForLoop(SpiceParser::ForLoopContext *ctx) {
  auto forLoopNode = static_cast<ForLoopNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::ForHeadContext *>(subTree); rule != nullptr) { // ForHead
      // Noop
    } else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = forLoopNode->createChild<StmtLstNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != forLoopNode) {
      visit(rule);
      currentNode = forLoopNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitForHead(SpiceParser::ForHeadContext *ctx) {
  auto forLoopNode = static_cast<ForLoopNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DeclStmtContext *>(subTree); rule != nullptr) // DeclStmt
      currentNode = forLoopNode->createChild<DeclStmtNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = forLoopNode->createChild<AssignExprNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != forLoopNode) {
      visit(rule);
      currentNode = forLoopNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitForeachLoop(SpiceParser::ForeachLoopContext *ctx) {
  auto foreachLoopNode = static_cast<ForeachLoopNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::ForeachHeadContext *>(subTree); rule != nullptr) { // ForeachHead
      // Noop
    } else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = foreachLoopNode->createChild<StmtLstNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != foreachLoopNode) {
      visit(rule);
      currentNode = foreachLoopNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitForeachHead(SpiceParser::ForeachHeadContext *ctx) {
  auto foreachLoopNode = static_cast<ForeachLoopNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DeclStmtContext *>(subTree); rule != nullptr) // DeclStmt
      currentNode = foreachLoopNode->createChild<DeclStmtNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = foreachLoopNode->createChild<AssignExprNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != foreachLoopNode) {
      visit(rule);
      currentNode = foreachLoopNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitWhileLoop(SpiceParser::WhileLoopContext *ctx) {
  auto whileLoopNode = static_cast<WhileLoopNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = whileLoopNode->createChild<AssignExprNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = whileLoopNode->createChild<StmtLstNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != whileLoopNode) {
      visit(rule);
      currentNode = whileLoopNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitIfStmt(SpiceParser::IfStmtContext *ctx) {
  auto ifStmtNode = static_cast<IfStmtNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = ifStmtNode->createChild<AssignExprNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = ifStmtNode->createChild<StmtLstNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // ElseStmt
      currentNode = ifStmtNode->createChild<ElseStmtNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != ifStmtNode) {
      visit(rule);
      currentNode = ifStmtNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitElseStmt(SpiceParser::ElseStmtContext *ctx) {
  auto elseStmtNode = static_cast<ElseStmtNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::IfStmtContext *>(subTree); rule != nullptr) // IfStmt
      currentNode = elseStmtNode->createChild<IfStmtNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = elseStmtNode->createChild<StmtLstNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != elseStmtNode) {
      visit(rule);
      currentNode = elseStmtNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitAssertStmt(SpiceParser::AssertStmtContext *ctx) {
  auto assertStmtNode = static_cast<AssertStmtNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = assertStmtNode->createChild<AssignExprNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != assertStmtNode) {
      visit(rule);
      currentNode = assertStmtNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitStmtLst(SpiceParser::StmtLstContext *ctx) {
  auto stmtLstNode = static_cast<StmtLstNode *>(currentNode);
  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::StmtContext *>(subTree); rule != nullptr) // Stmt
      currentNode = stmtLstNode->createChild<StmtNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::ForLoopContext *>(subTree); rule != nullptr) // ForLoop
      currentNode = stmtLstNode->createChild<ForLoopNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::ForeachLoopContext *>(subTree); rule != nullptr) // ForeachLoop
      currentNode = stmtLstNode->createChild<ForeachLoopNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::WhileLoopContext *>(subTree); rule != nullptr) // WhileLoop
      currentNode = stmtLstNode->createChild<WhileLoopNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::IfStmtContext *>(subTree); rule != nullptr) // IfStmt
      currentNode = stmtLstNode->createChild<IfStmtNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::AssertStmtContext *>(subTree); rule != nullptr) // AssertStmt
      currentNode = stmtLstNode->createChild<AssertStmtNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::ThreadDefContext *>(subTree); rule != nullptr) // ThreadDef
      currentNode = stmtLstNode->createChild<ThreadDefNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::UnsafeBlockDefContext *>(subTree); rule != nullptr) // UnsafeBlockDef
      currentNode = stmtLstNode->createChild<UnsafeBlockDefNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != stmtLstNode) {
      visit(rule);
      currentNode = stmtLstNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitTypeLst(SpiceParser::TypeLstContext *ctx) {
  auto typeLstNode = static_cast<TypeLstNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) // DataType
      currentNode = typeLstNode->createChild<DataTypeNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != typeLstNode) {
      visit(rule);
      currentNode = typeLstNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitTypeAltsLst(SpiceParser::TypeAltsLstContext *ctx) {
  auto typeAltsLstNode = static_cast<TypeAltsLstNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) // DataType
      currentNode = typeAltsLstNode->createChild<DataTypeNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != typeAltsLstNode) {
      visit(rule);
      currentNode = typeAltsLstNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitArgLstDef(SpiceParser::ArgLstDefContext *ctx) {
  auto argLstDefNode = static_cast<ArgLstDefNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DeclStmtContext *>(subTree); rule != nullptr) // DeclStmt
      currentNode = argLstDefNode->createChild<DeclStmtNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != argLstDefNode) {
      visit(rule);
      currentNode = argLstDefNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitArgLst(SpiceParser::ArgLstContext *ctx) {
  auto argLstNode = static_cast<ArgLstNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignStmt
      currentNode = argLstNode->createChild<AssignExprNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != argLstNode) {
      visit(rule);
      currentNode = argLstNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitField(SpiceParser::FieldContext *ctx) {
  auto fieldNode = static_cast<ArgLstNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DeclSpecifiersContext *>(subTree); rule != nullptr) // DeclSpecifiers
      currentNode = fieldNode->createChild<DeclSpecifiersNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) // DataType
      currentNode = fieldNode->createChild<DataTypeNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != fieldNode) {
      visit(rule);
      currentNode = fieldNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitStmt(SpiceParser::StmtContext *ctx) {
  auto stmtNode = static_cast<StmtNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DeclStmtContext *>(subTree); rule != nullptr) // DeclStmt
      currentNode = stmtNode->createChild<DeclStmtNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = stmtNode->createChild<AssignExprNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::ReturnStmtContext *>(subTree); rule != nullptr) // ReturnStmt
      currentNode = stmtNode->createChild<ReturnStmtNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::BreakStmtContext *>(subTree); rule != nullptr) // BreakStmt
      currentNode = stmtNode->createChild<BreakStmtNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::ContinueStmtContext *>(subTree); rule != nullptr) // ContinueStmt
      currentNode = stmtNode->createChild<ContinueStmtNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != stmtNode) {
      visit(rule);
      currentNode = stmtNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitDeclStmt(SpiceParser::DeclStmtContext *ctx) {
  auto declStmtNode = static_cast<DeclStmtNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DeclSpecifiersContext *>(subTree); rule != nullptr) // DeclSpecifiers
      currentNode = declStmtNode->createChild<DeclSpecifiersNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) // DataType
      currentNode = declStmtNode->createChild<DataTypeNode>(CodeLoc(rule->start));
    else if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = declStmtNode->createChild<AssignExprNode>(CodeLoc(rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != declStmtNode) {
      visit(rule);
      currentNode = declStmtNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitDeclSpecifiers(SpiceParser::DeclSpecifiersContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitDeclSpecifier(SpiceParser::DeclSpecifierContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitImportStmt(SpiceParser::ImportStmtContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitBreakStmt(SpiceParser::BreakStmtContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitContinueStmt(SpiceParser::ContinueStmtContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitBuiltinCall(SpiceParser::BuiltinCallContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitPrintfCall(SpiceParser::PrintfCallContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitSizeOfCall(SpiceParser::SizeOfCallContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitLenCall(SpiceParser::LenCallContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitTidCall(SpiceParser::TidCallContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitJoinCall(SpiceParser::JoinCallContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitAssignExpr(SpiceParser::AssignExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitTernaryExpr(SpiceParser::TernaryExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitBitwiseXorExpr(SpiceParser::BitwiseXorExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitEqualityExpr(SpiceParser::EqualityExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitShiftExpr(SpiceParser::ShiftExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitCastExpr(SpiceParser::CastExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitValue(SpiceParser::ValueContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitFunctionCall(SpiceParser::FunctionCallContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitArrayInitialization(SpiceParser::ArrayInitializationContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitStructInstantiation(SpiceParser::StructInstantiationContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitPrimitiveValue(SpiceParser::PrimitiveValueContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitDataType(SpiceParser::DataTypeContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitBaseDataType(SpiceParser::BaseDataTypeContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitCustomDataType(SpiceParser::CustomDataTypeContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitAssignOp(SpiceParser::AssignOpContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitPrefixUnaryOp(SpiceParser::PrefixUnaryOpContext *ctx) { return nullptr; }
