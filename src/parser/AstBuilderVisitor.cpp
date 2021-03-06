// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "AstBuilderVisitor.h"

#include <regex>

#include <ast/AstNodes.h>
#include <util/CommonUtil.h>

std::any AstBuilderVisitor::visitEntry(SpiceParser::EntryContext *ctx) {
  auto entryNode = dynamic_cast<EntryNode *>(currentNode);
  for (auto subTree : ctx->children) {
    // Create child for the current node
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::MainFunctionDefContext *>(subTree); rule != nullptr) // MainFctDef
      currentNode = entryNode->createChild<MainFctDefNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::FunctionDefContext *>(subTree); rule != nullptr) // FctDef
      currentNode = entryNode->createChild<FctDefNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::ProcedureDefContext *>(subTree); rule != nullptr) // ProcDef
      currentNode = entryNode->createChild<ProcDefNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::StructDefContext *>(subTree); rule != nullptr) // StructDef
      currentNode = entryNode->createChild<StructDefNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::GenericTypeDefContext *>(subTree); rule != nullptr) // GenericTypeDef
      currentNode = entryNode->createChild<GenericTypeDefNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::GlobalVarDefContext *>(subTree); rule != nullptr) // GlobalVarDef
      currentNode = entryNode->createChild<GlobalVarDefNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::ImportStmtContext *>(subTree); rule != nullptr) // ImportStmt
      currentNode = entryNode->createChild<ImportStmtNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::ExtDeclContext *>(subTree); rule != nullptr) // ExtDecl
      currentNode = entryNode->createChild<ExtDeclNode>(CodeLoc(fileName, rule->start));
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
  auto mainFctDefNode = dynamic_cast<MainFctDefNode *>(currentNode);
  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::ParamLstContext *>(subTree); rule != nullptr) { // ArgLstDef
      currentNode = mainFctDefNode->createChild<ParamLstNode>(CodeLoc(fileName, rule->start));
      mainFctDefNode->hasArgs = true;
    } else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = mainFctDefNode->createChild<StmtLstNode>(CodeLoc(fileName, rule->start));
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
  auto fctDefNode = dynamic_cast<FctDefNode *>(currentNode);

  // Extract function name
  fctDefNode->functionName = ctx->IDENTIFIER().back()->getText();
  if (ctx->IDENTIFIER().size() > 1) {
    fctDefNode->structName = ctx->IDENTIFIER().front()->getText();
    fctDefNode->fqFunctionName = fctDefNode->structName + "." + fctDefNode->functionName;
    fctDefNode->isMethod = true;
  }

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierLstContext *>(subTree); rule != nullptr) // DeclSpecifiers
      currentNode = fctDefNode->createChild<SpecifierLstNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) // DataType
      currentNode = fctDefNode->createChild<DataTypeNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) { // TypeLst
      currentNode = fctDefNode->createChild<TypeLstNode>(CodeLoc(fileName, rule->start));
      fctDefNode->isGeneric = true;
    } else if (rule = dynamic_cast<SpiceParser::ParamLstContext *>(subTree); rule != nullptr) { // ParamLst
      currentNode = fctDefNode->createChild<ParamLstNode>(CodeLoc(fileName, rule->start));
      fctDefNode->hasParams = true;
    } else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = fctDefNode->createChild<StmtLstNode>(CodeLoc(fileName, rule->start));
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
  auto procDefNode = dynamic_cast<ProcDefNode *>(currentNode);

  // Extract procedure name
  procDefNode->procedureName = ctx->IDENTIFIER().back()->getText();
  if (ctx->IDENTIFIER().size() > 1) {
    procDefNode->structName = ctx->IDENTIFIER().front()->getText();
    procDefNode->fqProcedureName = procDefNode->structName + "." + procDefNode->procedureName;
    procDefNode->isMethod = true;
  }

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierLstContext *>(subTree); rule != nullptr) // DeclSpecifiers
      currentNode = procDefNode->createChild<SpecifierLstNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) { // TypeLst
      currentNode = procDefNode->createChild<TypeLstNode>(CodeLoc(fileName, rule->start));
      procDefNode->isGeneric = true;
    } else if (rule = dynamic_cast<SpiceParser::ParamLstContext *>(subTree); rule != nullptr) { // ParamLst
      currentNode = procDefNode->createChild<ParamLstNode>(CodeLoc(fileName, rule->start));
      procDefNode->hasParams = true;
    } else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = procDefNode->createChild<StmtLstNode>(CodeLoc(fileName, rule->start));
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
  auto structDefNode = dynamic_cast<StructDefNode *>(currentNode);

  // Extract struct name
  structDefNode->structName = ctx->IDENTIFIER()->getText();

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierLstContext *>(subTree); rule != nullptr) // DeclSpecifiers
      currentNode = structDefNode->createChild<SpecifierLstNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) { // TypeLst
      currentNode = structDefNode->createChild<TypeLstNode>(CodeLoc(fileName, rule->start));
      structDefNode->isGeneric = true;
    } else if (rule = dynamic_cast<SpiceParser::FieldContext *>(subTree); rule != nullptr) // Field
      currentNode = structDefNode->createChild<FieldNode>(CodeLoc(fileName, rule->start));
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
  auto genericTypeDefNode = dynamic_cast<GenericTypeDefNode *>(currentNode);

  // Extract type name
  genericTypeDefNode->typeName = ctx->IDENTIFIER()->getText();

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierLstContext *>(subTree); rule != nullptr) // DeclSpecifiers
      currentNode = genericTypeDefNode->createChild<SpecifierLstNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::TypeAltsLstContext *>(subTree); rule != nullptr) // TypeAltsLst
      currentNode = genericTypeDefNode->createChild<TypeAltsLstNode>(CodeLoc(fileName, rule->start));
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
  auto globalVarDefNode = dynamic_cast<GlobalVarDefNode *>(currentNode);

  // Extract function name
  globalVarDefNode->varName = ctx->IDENTIFIER()->getText();

  // Extract negative
  globalVarDefNode->negative = ctx->MINUS();

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierLstContext *>(subTree); rule != nullptr) // DeclSpecifiers
      currentNode = globalVarDefNode->createChild<SpecifierLstNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) // DataType
      currentNode = globalVarDefNode->createChild<DataTypeNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::ValueContext *>(subTree); rule != nullptr) // Value
      currentNode = globalVarDefNode->createChild<ValueNode>(CodeLoc(fileName, rule->start));
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
  auto extDeclNode = dynamic_cast<ExtDeclNode *>(currentNode);

  // Extract function name
  extDeclNode->extFunctionName = ctx->IDENTIFIER()->getText();

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) // DataType
      currentNode = extDeclNode->createChild<DataTypeNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) { // TypeLst
      currentNode = extDeclNode->createChild<TypeLstNode>(CodeLoc(fileName, rule->start));
      extDeclNode->hasArgs = true;

    } else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != extDeclNode) {
      visit(rule);
      currentNode = extDeclNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitThreadDef(SpiceParser::ThreadDefContext *ctx) {
  auto threadDefNode = dynamic_cast<ThreadDefNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = threadDefNode->createChild<StmtLstNode>(CodeLoc(fileName, rule->start));
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
  auto unsafeBlockDefNode = dynamic_cast<UnsafeBlockDefNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = unsafeBlockDefNode->createChild<StmtLstNode>(CodeLoc(fileName, rule->start));
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
  auto forLoopNode = dynamic_cast<ForLoopNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::ForHeadContext *>(subTree); rule != nullptr) // ForHead
      visit(rule);
    else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = forLoopNode->createChild<StmtLstNode>(CodeLoc(fileName, rule->start));
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
  auto forLoopNode = dynamic_cast<ForLoopNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DeclStmtContext *>(subTree); rule != nullptr) // DeclStmt
      currentNode = forLoopNode->createChild<DeclStmtNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = forLoopNode->createChild<AssignExprNode>(CodeLoc(fileName, rule->start));
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
  auto foreachLoopNode = dynamic_cast<ForeachLoopNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::ForeachHeadContext *>(subTree); rule != nullptr) // ForeachHead
      visit(rule);
    else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = foreachLoopNode->createChild<StmtLstNode>(CodeLoc(fileName, rule->start));
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
  auto foreachLoopNode = dynamic_cast<ForeachLoopNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DeclStmtContext *>(subTree); rule != nullptr) // DeclStmt
      currentNode = foreachLoopNode->createChild<DeclStmtNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = foreachLoopNode->createChild<AssignExprNode>(CodeLoc(fileName, rule->start));
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
  auto whileLoopNode = dynamic_cast<WhileLoopNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = whileLoopNode->createChild<AssignExprNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = whileLoopNode->createChild<StmtLstNode>(CodeLoc(fileName, rule->start));
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
  auto ifStmtNode = dynamic_cast<IfStmtNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = ifStmtNode->createChild<AssignExprNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = ifStmtNode->createChild<StmtLstNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::ElseStmtContext *>(subTree); rule != nullptr) // ElseStmt
      currentNode = ifStmtNode->createChild<ElseStmtNode>(CodeLoc(fileName, rule->start));
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
  auto elseStmtNode = dynamic_cast<ElseStmtNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::IfStmtContext *>(subTree); rule != nullptr) { // IfStmt
      currentNode = elseStmtNode->createChild<IfStmtNode>(CodeLoc(fileName, rule->start));
      elseStmtNode->isElseIf = true;
    } else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = elseStmtNode->createChild<StmtLstNode>(CodeLoc(fileName, rule->start));
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
  auto assertStmtNode = dynamic_cast<AssertStmtNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) { // AssignExpr
      currentNode = assertStmtNode->createChild<AssignExprNode>(CodeLoc(fileName, rule->start));
      assertStmtNode->expressionString = rule->getText();
    } else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != assertStmtNode) {
      visit(rule);
      currentNode = assertStmtNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitStmtLst(SpiceParser::StmtLstContext *ctx) {
  auto stmtLstNode = dynamic_cast<StmtLstNode *>(currentNode);
  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::StmtContext *>(subTree); rule != nullptr) // Stmt
      currentNode = stmtLstNode->createChild<StmtNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::ForLoopContext *>(subTree); rule != nullptr) // ForLoop
      currentNode = stmtLstNode->createChild<ForLoopNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::ForeachLoopContext *>(subTree); rule != nullptr) // ForeachLoop
      currentNode = stmtLstNode->createChild<ForeachLoopNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::WhileLoopContext *>(subTree); rule != nullptr) // WhileLoop
      currentNode = stmtLstNode->createChild<WhileLoopNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::IfStmtContext *>(subTree); rule != nullptr) // IfStmt
      currentNode = stmtLstNode->createChild<IfStmtNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::AssertStmtContext *>(subTree); rule != nullptr) // AssertStmt
      currentNode = stmtLstNode->createChild<AssertStmtNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::ThreadDefContext *>(subTree); rule != nullptr) // ThreadDef
      currentNode = stmtLstNode->createChild<ThreadDefNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::UnsafeBlockDefContext *>(subTree); rule != nullptr) // UnsafeBlockDef
      currentNode = stmtLstNode->createChild<UnsafeBlockDefNode>(CodeLoc(fileName, rule->start));
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
  auto typeLstNode = dynamic_cast<TypeLstNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) { // DataType
      currentNode = typeLstNode->createChild<DataTypeNode>(CodeLoc(fileName, rule->start));
      typeLstNode->numberOfTypes++;
    } else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != typeLstNode) {
      visit(rule);
      currentNode = typeLstNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitTypeAltsLst(SpiceParser::TypeAltsLstContext *ctx) {
  auto typeAltsLstNode = dynamic_cast<TypeAltsLstNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) { // DataType
      currentNode = typeAltsLstNode->createChild<DataTypeNode>(CodeLoc(fileName, rule->start));
      typeAltsLstNode->numberOfAlts++;
    } else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != typeAltsLstNode) {
      visit(rule);
      currentNode = typeAltsLstNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitParamLst(SpiceParser::ParamLstContext *ctx) {
  auto argLstDefNode = dynamic_cast<ParamLstNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DeclStmtContext *>(subTree); rule != nullptr) // DeclStmt
      currentNode = argLstDefNode->createChild<DeclStmtNode>(CodeLoc(fileName, rule->start));
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
  auto argLstNode = dynamic_cast<ArgLstNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignStmt
      currentNode = argLstNode->createChild<AssignExprNode>(CodeLoc(fileName, rule->start));
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
  auto fieldNode = dynamic_cast<FieldNode *>(currentNode);

  // Extract field name
  fieldNode->name = ctx->IDENTIFIER()->getText();

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierLstContext *>(subTree); rule != nullptr) // DeclSpecifiers
      currentNode = fieldNode->createChild<SpecifierLstNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) // DataType
      currentNode = fieldNode->createChild<DataTypeNode>(CodeLoc(fileName, rule->start));
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
  auto stmtNode = dynamic_cast<StmtNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DeclStmtContext *>(subTree); rule != nullptr) // DeclStmt
      currentNode = stmtNode->createChild<DeclStmtNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = stmtNode->createChild<AssignExprNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::ReturnStmtContext *>(subTree); rule != nullptr) // ReturnStmt
      currentNode = stmtNode->createChild<ReturnStmtNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::BreakStmtContext *>(subTree); rule != nullptr) // BreakStmt
      currentNode = stmtNode->createChild<BreakStmtNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::ContinueStmtContext *>(subTree); rule != nullptr) // ContinueStmt
      currentNode = stmtNode->createChild<ContinueStmtNode>(CodeLoc(fileName, rule->start));
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
  auto declStmtNode = dynamic_cast<DeclStmtNode *>(currentNode);

  // Extract var name
  declStmtNode->varName = ctx->IDENTIFIER()->getText();

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierLstContext *>(subTree); rule != nullptr) // DeclSpecifiers
      currentNode = declStmtNode->createChild<SpecifierLstNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) // DataType
      currentNode = declStmtNode->createChild<DataTypeNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) { // AssignExpr
      currentNode = declStmtNode->createChild<AssignExprNode>(CodeLoc(fileName, rule->start));
      declStmtNode->hasAssignment = true;
    } else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != declStmtNode) {
      visit(rule);
      currentNode = declStmtNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitSpecifierLst(SpiceParser::SpecifierLstContext *ctx) {
  auto specifierLstNode = dynamic_cast<SpecifierLstNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierContext *>(subTree); rule != nullptr) // DeclSpecifier
      currentNode = specifierLstNode->createChild<SpecifierNode>(CodeLoc(fileName, rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != specifierLstNode) {
      visit(rule);
      currentNode = specifierLstNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitSpecifier(SpiceParser::SpecifierContext *ctx) {
  auto specifierNode = dynamic_cast<SpecifierNode *>(currentNode);

  for (auto subTree : ctx->children) {
    auto token = dynamic_cast<antlr4::tree::TerminalNode *>(subTree);
    assert(token != nullptr);
    size_t symbolType = token->getSymbol()->getType();
    if (symbolType == SpiceParser::CONST)
      specifierNode->type = SpecifierNode::TY_CONST;
    else if (symbolType == SpiceParser::SIGNED)
      specifierNode->type = SpecifierNode::TY_SIGNED;
    else if (symbolType == SpiceParser::UNSIGNED)
      specifierNode->type = SpecifierNode::TY_UNSIGNED;
    else if (symbolType == SpiceParser::INLINE)
      specifierNode->type = SpecifierNode::TY_INLINE;
    else if (symbolType == SpiceParser::PUBLIC)
      specifierNode->type = SpecifierNode::TY_PUBLIC;
    else
      assert(false);
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitImportStmt(SpiceParser::ImportStmtContext *ctx) {
  auto importStmtNode = dynamic_cast<ImportStmtNode *>(currentNode);

  // Extract path
  std::string pathStr = ctx->STRING_LIT()->getText();
  importStmtNode->importPath = pathStr.substr(1, pathStr.size() - 2);

  // Extract name
  importStmtNode->importName = ctx->IDENTIFIER()->getText();

  return nullptr;
}

std::any AstBuilderVisitor::visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) {
  auto returnStmtNode = dynamic_cast<ReturnStmtNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) { // AssignExpr
      currentNode = returnStmtNode->createChild<AssignExprNode>(CodeLoc(fileName, rule->start));
      returnStmtNode->hasReturnValue = true;
    } else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != returnStmtNode) {
      visit(rule);
      currentNode = returnStmtNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitBreakStmt(SpiceParser::BreakStmtContext *ctx) {
  auto breakStmtNode = dynamic_cast<BreakStmtNode *>(currentNode);

  // Extract number of breaks
  breakStmtNode->breakTimes = std::stoi(ctx->INT_LIT()->toString());

  return nullptr;
}

std::any AstBuilderVisitor::visitContinueStmt(SpiceParser::ContinueStmtContext *ctx) {
  auto continueStmtNode = dynamic_cast<ContinueStmtNode *>(currentNode);

  // Extract number of continues
  continueStmtNode->continueTimes = std::stoi(ctx->INT_LIT()->toString());

  return nullptr;
}

std::any AstBuilderVisitor::visitBuiltinCall(SpiceParser::BuiltinCallContext *ctx) {
  auto atomicExprNode = dynamic_cast<AtomicExprNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::PrintfCallContext *>(subTree); rule != nullptr) // PrintfCall
      currentNode = atomicExprNode->createChild<PrintfCallNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::SizeOfCallContext *>(subTree); rule != nullptr) // SizeofCall
      currentNode = atomicExprNode->createChild<SizeofCallNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::LenCallContext *>(subTree); rule != nullptr) // LenCall
      currentNode = atomicExprNode->createChild<LenCallNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::TidCallContext *>(subTree); rule != nullptr) // TidCall
      currentNode = atomicExprNode->createChild<TidCallNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::JoinCallContext *>(subTree); rule != nullptr) // JoinCall
      currentNode = atomicExprNode->createChild<JoinCallNode>(CodeLoc(fileName, rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != atomicExprNode) {
      visit(rule);
      currentNode = atomicExprNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitPrintfCall(SpiceParser::PrintfCallContext *ctx) {
  auto printfCallNode = dynamic_cast<PrintfCallNode *>(currentNode);

  // Extract templated string
  std::string templatedString = ctx->STRING_LIT()->getText();
  templatedString = templatedString.substr(1, templatedString.size() - 2);
  replaceEscapeChars(templatedString);
  printfCallNode->templatedString = templatedString;

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = printfCallNode->createChild<AssignExprNode>(CodeLoc(fileName, rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != printfCallNode) {
      visit(rule);
      currentNode = printfCallNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitSizeOfCall(SpiceParser::SizeOfCallContext *ctx) {
  auto sizeofCallNode = dynamic_cast<SizeofCallNode *>(currentNode);

  // Check if type or value
  sizeofCallNode->isType = ctx->TYPE();

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = sizeofCallNode->createChild<AssignExprNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) // DataType
      currentNode = sizeofCallNode->createChild<DataTypeNode>(CodeLoc(fileName, rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != sizeofCallNode) {
      visit(rule);
      currentNode = sizeofCallNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitLenCall(SpiceParser::LenCallContext *ctx) {
  auto lenCallNode = dynamic_cast<LenCallNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = lenCallNode->createChild<AssignExprNode>(CodeLoc(fileName, rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != lenCallNode) {
      visit(rule);
      currentNode = lenCallNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitTidCall(SpiceParser::TidCallContext *ctx) { return nullptr; }

std::any AstBuilderVisitor::visitJoinCall(SpiceParser::JoinCallContext *ctx) {
  auto joinCallNode = dynamic_cast<JoinCallNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = joinCallNode->createChild<AssignExprNode>(CodeLoc(fileName, rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != joinCallNode) {
      visit(rule);
      currentNode = joinCallNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitAssignExpr(SpiceParser::AssignExprContext *ctx) {
  auto assignExprNode = dynamic_cast<AssignExprNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::PrefixUnaryExprContext *>(subTree); rule != nullptr) // PrefixUnaryExpr
      currentNode = assignExprNode->createChild<PrefixUnaryExprNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::AssignOpContext *>(subTree); rule != nullptr) // AssignOp
      visit(rule);
    else if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = assignExprNode->createChild<AssignExprNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::TernaryExprContext *>(subTree); rule != nullptr) // TernaryExpr
      currentNode = assignExprNode->createChild<TernaryExprNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::ThreadDefContext *>(subTree); rule != nullptr) // ThreadDef
      currentNode = assignExprNode->createChild<ThreadDefNode>(CodeLoc(fileName, rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != assignExprNode) {
      visit(rule);
      currentNode = assignExprNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitTernaryExpr(SpiceParser::TernaryExprContext *ctx) {
  auto ternaryExprNode = dynamic_cast<TernaryExprNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::LogicalOrExprContext *>(subTree); rule != nullptr) // LogicalOrExpr
      currentNode = ternaryExprNode->createChild<LogicalOrExprNode>(CodeLoc(fileName, rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != ternaryExprNode) {
      visit(rule);
      currentNode = ternaryExprNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) {
  auto logicalOrExprNode = dynamic_cast<LogicalOrExprNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::LogicalAndExprContext *>(subTree); rule != nullptr) // LogicalAndExpr
      currentNode = logicalOrExprNode->createChild<LogicalAndExprNode>(CodeLoc(fileName, rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != logicalOrExprNode) {
      visit(rule);
      currentNode = logicalOrExprNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) {
  auto logicalAndExprNode = dynamic_cast<LogicalAndExprNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::BitwiseOrExprContext *>(subTree); rule != nullptr) // BitwiseOr
      currentNode = logicalAndExprNode->createChild<BitwiseOrExprNode>(CodeLoc(fileName, rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != logicalAndExprNode) {
      visit(rule);
      currentNode = logicalAndExprNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) {
  auto bitwiseOrExprNode = dynamic_cast<BitwiseOrExprNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::BitwiseXorExprContext *>(subTree); rule != nullptr) // BitwiseXor
      currentNode = bitwiseOrExprNode->createChild<BitwiseXorExprNode>(CodeLoc(fileName, rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != bitwiseOrExprNode) {
      visit(rule);
      currentNode = bitwiseOrExprNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitBitwiseXorExpr(SpiceParser::BitwiseXorExprContext *ctx) {
  auto bitwiseXorExprNode = dynamic_cast<BitwiseXorExprNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::BitwiseAndExprContext *>(subTree); rule != nullptr) // BitwiseAnd
      currentNode = bitwiseXorExprNode->createChild<BitwiseAndExprNode>(CodeLoc(fileName, rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != bitwiseXorExprNode) {
      visit(rule);
      currentNode = bitwiseXorExprNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) {
  auto bitwiseAndExprNode = dynamic_cast<BitwiseAndExprNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::EqualityExprContext *>(subTree); rule != nullptr) // EqualityExpr
      currentNode = bitwiseAndExprNode->createChild<EqualityExprNode>(CodeLoc(fileName, rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != bitwiseAndExprNode) {
      visit(rule);
      currentNode = bitwiseAndExprNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitEqualityExpr(SpiceParser::EqualityExprContext *ctx) {
  auto equalityExprNode = dynamic_cast<EqualityExprNode *>(currentNode);

  // Extract operator
  if (ctx->EQUAL())
    equalityExprNode->op = EqualityExprNode::OP_EQUAL;
  else if (ctx->NOT_EQUAL())
    equalityExprNode->op = EqualityExprNode::OP_NOT_EQUAL;

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::RelationalExprContext *>(subTree); rule != nullptr) // RelationalExpr
      currentNode = equalityExprNode->createChild<RelationalExprNode>(CodeLoc(fileName, rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != equalityExprNode) {
      visit(rule);
      currentNode = equalityExprNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) {
  auto relationalExprNode = dynamic_cast<RelationalExprNode *>(currentNode);

  // Extract operator
  if (ctx->LESS())
    relationalExprNode->op = RelationalExprNode::OP_LESS;
  else if (ctx->GREATER())
    relationalExprNode->op = RelationalExprNode::OP_GREATER;
  else if (ctx->LESS_EQUAL())
    relationalExprNode->op = RelationalExprNode::OP_LESS_EQUAL;
  else if (ctx->GREATER_EQUAL())
    relationalExprNode->op = RelationalExprNode::OP_GREATER_EQUAL;

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::ShiftExprContext *>(subTree); rule != nullptr) // ShiftExpr
      currentNode = relationalExprNode->createChild<ShiftExprNode>(CodeLoc(fileName, rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != relationalExprNode) {
      visit(rule);
      currentNode = relationalExprNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitShiftExpr(SpiceParser::ShiftExprContext *ctx) {
  auto shiftExprNode = dynamic_cast<ShiftExprNode *>(currentNode);

  // Extract operator
  if (!ctx->LESS().empty())
    shiftExprNode->op = ShiftExprNode::OP_SHIFT_LEFT;
  else if (!ctx->GREATER().empty())
    shiftExprNode->op = ShiftExprNode::OP_SHIFT_RIGHT;

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AdditiveExprContext *>(subTree); rule != nullptr) // AdditiveExpr
      currentNode = shiftExprNode->createChild<AdditiveExprNode>(CodeLoc(fileName, rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != shiftExprNode) {
      visit(rule);
      currentNode = shiftExprNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) {
  auto additiveExprNode = dynamic_cast<AdditiveExprNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::MultiplicativeExprContext *>(subTree); rule != nullptr) // MultiplicativeExpr
      currentNode = additiveExprNode->createChild<MultiplicativeExprNode>(CodeLoc(fileName, rule->start));
    else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::PLUS)
      additiveExprNode->opQueue.push(AdditiveExprNode::OP_PLUS);
    else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::MINUS)
      additiveExprNode->opQueue.push(AdditiveExprNode::OP_MINUS);
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != additiveExprNode) {
      visit(rule);
      currentNode = additiveExprNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) {
  auto multiplicativeExprNode = dynamic_cast<MultiplicativeExprNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::CastExprContext *>(subTree); rule != nullptr) // CastExpr
      currentNode = multiplicativeExprNode->createChild<CastExprNode>(CodeLoc(fileName, rule->start));
    else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::MUL)
      multiplicativeExprNode->opQueue.push(MultiplicativeExprNode::OP_MUL);
    else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::DIV)
      multiplicativeExprNode->opQueue.push(MultiplicativeExprNode::OP_DIV);
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != multiplicativeExprNode) {
      visit(rule);
      currentNode = multiplicativeExprNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitCastExpr(SpiceParser::CastExprContext *ctx) {
  auto castExprNode = dynamic_cast<CastExprNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::PrefixUnaryExprContext *>(subTree); rule != nullptr) // PrefixUnaryExpr
      currentNode = castExprNode->createChild<PrefixUnaryExprNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) { // DataType
      currentNode = castExprNode->createChild<DataTypeNode>(CodeLoc(fileName, rule->start));
      castExprNode->isCasted = true;
    } else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != castExprNode) {
      visit(rule);
      currentNode = castExprNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext *ctx) {
  auto prefixUnaryExprNode = dynamic_cast<PrefixUnaryExprNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::PrefixUnaryOpContext *>(subTree); rule != nullptr) // PrefixUnaryOp
      visit(rule);
    else if (rule = dynamic_cast<SpiceParser::PostfixUnaryExprContext *>(subTree); rule != nullptr) // PostfixUnaryExpr
      currentNode = prefixUnaryExprNode->createChild<PostfixUnaryExprNode>(CodeLoc(fileName, rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != prefixUnaryExprNode) {
      visit(rule);
      currentNode = prefixUnaryExprNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext *ctx) {
  auto postfixUnaryExprNode = dynamic_cast<PostfixUnaryExprNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AtomicExprContext *>(subTree); rule != nullptr) // AtomicExpr
      currentNode = postfixUnaryExprNode->createChild<AtomicExprNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = postfixUnaryExprNode->createChild<AssignExprNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::PostfixUnaryExprContext *>(subTree); rule != nullptr) // PostfixUnaryExpr
      currentNode = postfixUnaryExprNode->createChild<PostfixUnaryExprNode>(CodeLoc(fileName, rule->start));
    else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::LBRACKET)
      postfixUnaryExprNode->opQueue.push(PostfixUnaryExprNode::OP_SUBSCRIPT);
    else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::DOT)
      postfixUnaryExprNode->opQueue.push(PostfixUnaryExprNode::OP_MEMBER_ACCESS);
    else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::PLUS_PLUS)
      postfixUnaryExprNode->opQueue.push(PostfixUnaryExprNode::OP_PLUS_PLUS);
    else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::MINUS_MINUS)
      postfixUnaryExprNode->opQueue.push(PostfixUnaryExprNode::OP_MINUS_MINUS);
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != postfixUnaryExprNode) {
      visit(rule);
      currentNode = postfixUnaryExprNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) {
  auto atomicExprNode = dynamic_cast<AtomicExprNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::ValueContext *>(subTree); rule != nullptr) // Value
      currentNode = atomicExprNode->createChild<ValueNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::BuiltinCallContext *>(subTree); rule != nullptr) // BuiltinCall
      visit(rule);
    else if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = atomicExprNode->createChild<AssignExprNode>(CodeLoc(fileName, rule->start));
    else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::IDENTIFIER)
      atomicExprNode->identifier = t->getText();
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != atomicExprNode) {
      visit(rule);
      currentNode = atomicExprNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitValue(SpiceParser::ValueContext *ctx) {
  auto valueNode = dynamic_cast<ValueNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::PrimitiveValueContext *>(subTree); rule != nullptr) // PrimitiveValue
      currentNode = valueNode->createChild<PrimitiveValueNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::FunctionCallContext *>(subTree); rule != nullptr) // FunctionCall
      currentNode = valueNode->createChild<FunctionCallNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::ArrayInitializationContext *>(subTree); rule != nullptr) // ArrayInitialization
      currentNode = valueNode->createChild<ArrayInitializationNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::StructInstantiationContext *>(subTree); rule != nullptr) // StructInstantiation
      currentNode = valueNode->createChild<StructInstantiationNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) { // DataType
      currentNode = valueNode->createChild<DataTypeNode>(CodeLoc(fileName, rule->start));
      valueNode->isNil = true;
    } else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != valueNode) {
      visit(rule);
      currentNode = valueNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitPrimitiveValue(SpiceParser::PrimitiveValueContext *ctx) {
  auto primitiveValueNode = dynamic_cast<PrimitiveValueNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::DOUBLE_LIT) {
      primitiveValueNode->type = PrimitiveValueNode::TY_DOUBLE;
      primitiveValueNode->data.doubleValue = std::stod(t->toString());
    } else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::INT_LIT) {
      primitiveValueNode->type = PrimitiveValueNode::TY_INT;
      primitiveValueNode->data.intValue = std::stoi(t->toString());
    } else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree);
               t->getSymbol()->getType() == SpiceParser::SHORT_LIT) {
      primitiveValueNode->type = PrimitiveValueNode::TY_SHORT;
      primitiveValueNode->data.shortValue = (short)std::stoi(t->toString());
    } else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::LONG_LIT) {
      primitiveValueNode->type = PrimitiveValueNode::TY_LONG;
      primitiveValueNode->data.longValue = std::stoll(t->toString());
    } else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::CHAR_LIT) {
      primitiveValueNode->type = PrimitiveValueNode::TY_CHAR;
      primitiveValueNode->data.charValue = ctx->CHAR_LIT()->toString()[1];
    } else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree);
               t->getSymbol()->getType() == SpiceParser::STRING_LIT) {
      primitiveValueNode->type = PrimitiveValueNode::TY_STRING;
      std::string strValue = ctx->STRING_LIT()->toString();
      strValue = strValue.substr(1, strValue.size() - 2);
      replaceEscapeChars(strValue);
      primitiveValueNode->data.stringValue = strValue;
    } else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::TRUE) {
      primitiveValueNode->type = PrimitiveValueNode::TY_BOOL;
      primitiveValueNode->data.boolValue = true;
    } else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::FALSE) {
      primitiveValueNode->type = PrimitiveValueNode::TY_BOOL;
      primitiveValueNode->data.boolValue = false;
    } else {
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal
    }

    if (currentNode != primitiveValueNode) {
      visit(rule);
      currentNode = primitiveValueNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitFunctionCall(SpiceParser::FunctionCallContext *ctx) {
  auto fctCallNode = dynamic_cast<FunctionCallNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) { // TypeLst
      currentNode = fctCallNode->createChild<TypeLstNode>(CodeLoc(fileName, rule->start));
      fctCallNode->isGeneric = true;
    } else if (rule = dynamic_cast<SpiceParser::ArgLstContext *>(subTree); rule != nullptr) { // ArgLst
      currentNode = fctCallNode->createChild<ArgLstNode>(CodeLoc(fileName, rule->start));
      fctCallNode->hasArgs = true;
    } else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree);
               t->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
      std::string fragment = t->toString();
      fctCallNode->functionNameFragments.push_back(fragment);
      if (!fctCallNode->fqFunctionName.empty())
        fctCallNode->fqFunctionName += ".";
      fctCallNode->fqFunctionName += fragment;
    } else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != fctCallNode) {
      visit(rule);
      currentNode = fctCallNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitArrayInitialization(SpiceParser::ArrayInitializationContext *ctx) {
  auto arrayInitializationNode = dynamic_cast<ArrayInitializationNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::ArgLstContext *>(subTree); rule != nullptr) // ArgLst
      currentNode = arrayInitializationNode->createChild<ArgLstNode>(CodeLoc(fileName, rule->start));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != arrayInitializationNode) {
      visit(rule);
      currentNode = arrayInitializationNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitStructInstantiation(SpiceParser::StructInstantiationContext *ctx) {
  auto structInstantiationNode = dynamic_cast<StructInstantiationNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) // TypeLst
      currentNode = structInstantiationNode->createChild<TypeLstNode>(CodeLoc(fileName, rule->start));
    else if (rule = dynamic_cast<SpiceParser::ArgLstContext *>(subTree); rule != nullptr) // ArgLst
      currentNode = structInstantiationNode->createChild<ArgLstNode>(CodeLoc(fileName, rule->start));
    else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
      std::string fragment = t->toString();
      structInstantiationNode->structNameFragments.push_back(fragment);
      if (!structInstantiationNode->fqStructName.empty())
        structInstantiationNode->fqStructName += ".";
      structInstantiationNode->fqStructName += fragment;
    } else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != structInstantiationNode) {
      visit(rule);
      currentNode = structInstantiationNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitDataType(SpiceParser::DataTypeContext *ctx) {
  auto dataTypeNode = dynamic_cast<DataTypeNode *>(currentNode);

  for (int i = 0; i < ctx->children.size(); i++) {
    auto subTree = ctx->children[i];
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::BaseDataTypeContext *>(subTree); rule != nullptr) // BaseDataType
      currentNode = dataTypeNode->createChild<BaseDataTypeNode>(CodeLoc(fileName, rule->start));
    else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::MUL)
      dataTypeNode->tmQueue.push({DataTypeNode::TY_POINTER, false, false, 0});
    else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::LBRACKET) {
      i++; // Consume LBRACKET
      subTree = ctx->children[i];
      bool hasSize = false;
      bool isHardcoded = false;
      int hardCodedSize = 0;
      if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) { // AssignExpr
        hasSize = true;
        hardCodedSize = -1;
        currentNode = dataTypeNode->createChild<AssignExprNode>(CodeLoc(fileName, rule->start));
        i++; // Consume INTEGER
      } else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree);
                 t->getSymbol()->getType() == SpiceParser::INT_LIT) {
        hasSize = true;
        isHardcoded = true;
        hardCodedSize = std::stoi(t->getSymbol()->getText());
        i++; // Consume INTEGER
      }
      dataTypeNode->tmQueue.push({DataTypeNode::TY_ARRAY, hasSize, isHardcoded, hardCodedSize});
    } else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != dataTypeNode) {
      visit(rule);
      currentNode = dataTypeNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitBaseDataType(SpiceParser::BaseDataTypeContext *ctx) {
  auto baseDataTypeNode = dynamic_cast<BaseDataTypeNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::CustomDataTypeContext *>(subTree); rule != nullptr) { // CustomDataType
      baseDataTypeNode->type = BaseDataTypeNode::TY_CUSTOM;
      currentNode = baseDataTypeNode->createChild<CustomDataTypeNode>(CodeLoc(fileName, rule->start));
    } else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree);
               t->getSymbol()->getType() == SpiceParser::TYPE_DOUBLE)
      baseDataTypeNode->type = BaseDataTypeNode::TY_DOUBLE;
    else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::TYPE_INT)
      baseDataTypeNode->type = BaseDataTypeNode::TY_INT;
    else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::TYPE_SHORT)
      baseDataTypeNode->type = BaseDataTypeNode::TY_SHORT;
    else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::TYPE_LONG)
      baseDataTypeNode->type = BaseDataTypeNode::TY_LONG;
    else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::TYPE_BYTE)
      baseDataTypeNode->type = BaseDataTypeNode::TY_BYTE;
    else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::TYPE_CHAR)
      baseDataTypeNode->type = BaseDataTypeNode::TY_CHAR;
    else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::TYPE_STRING)
      baseDataTypeNode->type = BaseDataTypeNode::TY_STRING;
    else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::TYPE_BOOL)
      baseDataTypeNode->type = BaseDataTypeNode::TY_BOOL;
    else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::TYPE_DYN)
      baseDataTypeNode->type = BaseDataTypeNode::TY_DYN;
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != baseDataTypeNode) {
      visit(rule);
      currentNode = baseDataTypeNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitCustomDataType(SpiceParser::CustomDataTypeContext *ctx) {
  auto customDataTypeNode = dynamic_cast<CustomDataTypeNode *>(currentNode);

  for (auto subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) // TypeLst
      currentNode = customDataTypeNode->createChild<TypeLstNode>(CodeLoc(fileName, rule->start));
    else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
      std::string fragment = t->toString();
      customDataTypeNode->typeNameFragments.push_back(fragment);
      if (!customDataTypeNode->fqTypeName.empty())
        customDataTypeNode->fqTypeName += ".";
      customDataTypeNode->fqTypeName += fragment;
    } else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != customDataTypeNode) {
      visit(rule);
      currentNode = customDataTypeNode;
    }
  }
  return nullptr;
}

std::any AstBuilderVisitor::visitAssignOp(SpiceParser::AssignOpContext *ctx) {
  auto assignExprNode = dynamic_cast<AssignExprNode *>(currentNode);

  assignExprNode->hasOperator = true;

  // Extract assign operator
  if (ctx->ASSIGN())
    assignExprNode->op = AssignExprNode::OP_ASSIGN;
  else if (ctx->PLUS_EQUAL())
    assignExprNode->op = AssignExprNode::OP_PLUS_EQUAL;
  else if (ctx->MINUS_EQUAL())
    assignExprNode->op = AssignExprNode::OP_MINUS_EQUAL;
  else if (ctx->MUL_EQUAL())
    assignExprNode->op = AssignExprNode::OP_MUL_EQUAL;
  else if (ctx->DIV_EQUAL())
    assignExprNode->op = AssignExprNode::OP_DIV_EQUAL;
  else if (ctx->REM_EQUAL())
    assignExprNode->op = AssignExprNode::OP_REM_EQUAL;
  else if (ctx->SHL_EQUAL())
    assignExprNode->op = AssignExprNode::OP_SHL_EQUAL;
  else if (ctx->SHR_EQUAL())
    assignExprNode->op = AssignExprNode::OP_SHR_EQUAL;
  else if (ctx->AND_EQUAL())
    assignExprNode->op = AssignExprNode::OP_AND_EQUAL;
  else if (ctx->OR_EQUAL())
    assignExprNode->op = AssignExprNode::OP_OR_EQUAL;
  else if (ctx->XOR_EQUAL())
    assignExprNode->op = AssignExprNode::OP_XOR_EQUAL;
  else
    assert(false);

  return nullptr;
}

std::any AstBuilderVisitor::visitPrefixUnaryOp(SpiceParser::PrefixUnaryOpContext *ctx) {
  auto prefixUnaryExprNode = dynamic_cast<PrefixUnaryExprNode *>(currentNode);

  // Extract assign operator
  if (ctx->MINUS())
    prefixUnaryExprNode->opStack.push(PrefixUnaryExprNode::OP_MINUS);
  else if (ctx->PLUS_PLUS())
    prefixUnaryExprNode->opStack.push(PrefixUnaryExprNode::OP_PLUS_PLUS);
  else if (ctx->MINUS_MINUS())
    prefixUnaryExprNode->opStack.push(PrefixUnaryExprNode::OP_MINUS_MINUS);
  else if (ctx->NOT())
    prefixUnaryExprNode->opStack.push(PrefixUnaryExprNode::OP_NOT);
  else if (ctx->BITWISE_NOT())
    prefixUnaryExprNode->opStack.push(PrefixUnaryExprNode::OP_BITWISE_NOT);
  else if (ctx->MUL())
    prefixUnaryExprNode->opStack.push(PrefixUnaryExprNode::OP_INDIRECTION);
  else if (ctx->BITWISE_AND())
    prefixUnaryExprNode->opStack.push(PrefixUnaryExprNode::OP_ADDRESS_OF);
  else if (ctx->LOGICAL_AND()) {
    prefixUnaryExprNode->opStack.push(PrefixUnaryExprNode::OP_ADDRESS_OF);
    prefixUnaryExprNode->opStack.push(PrefixUnaryExprNode::OP_ADDRESS_OF);
  } else
    assert(false);

  return nullptr;
}

void AstBuilderVisitor::replaceEscapeChars(std::string &string) const {
  CommonUtil::replaceAll(string, "\\a", "\a");
  CommonUtil::replaceAll(string, "\\b", "\b");
  CommonUtil::replaceAll(string, "\\f", "\f");
  CommonUtil::replaceAll(string, "\\n", "\n");
  CommonUtil::replaceAll(string, "\\r", "\r");
  CommonUtil::replaceAll(string, "\\t", "\t");
  CommonUtil::replaceAll(string, "\\v", "\v");
  CommonUtil::replaceAll(string, "\\'", "\'");
  CommonUtil::replaceAll(string, "\\?", "\?");
}