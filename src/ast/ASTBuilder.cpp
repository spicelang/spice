// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "ASTBuilder.h"

#include <regex>

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <exception/ParserError.h>
#include <util/CommonUtil.h>

ASTBuilder::ASTBuilder(GlobalResourceManager &resourceManager, SourceFile *sourceFile, ASTNode *rootNode,
                       antlr4::ANTLRInputStream *inputStream)
    : CompilerPass(resourceManager, sourceFile), currentNode(rootNode), filePath(sourceFile->filePath), inputStream(inputStream) {
}

std::any ASTBuilder::visitEntry(SpiceParser::EntryContext *ctx) {
  auto entryNode = dynamic_cast<EntryNode *>(currentNode);
  for (const auto &subTree : ctx->children) {
    // Create child for the current node
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::MainFunctionDefContext *>(subTree); rule != nullptr) // MainFctDef
      currentNode = entryNode->createChild<MainFctDefNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::FunctionDefContext *>(subTree); rule != nullptr) // FctDef
      currentNode = entryNode->createChild<FctDefNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::ProcedureDefContext *>(subTree); rule != nullptr) // ProcDef
      currentNode = entryNode->createChild<ProcDefNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::StructDefContext *>(subTree); rule != nullptr) // StructDef
      currentNode = entryNode->createChild<StructDefNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::InterfaceDefContext *>(subTree); rule != nullptr) // InterfaceDef
      currentNode = entryNode->createChild<InterfaceDefNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::EnumDefContext *>(subTree); rule != nullptr) // EnumDef
      currentNode = entryNode->createChild<EnumDefNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::GenericTypeDefContext *>(subTree); rule != nullptr) // GenericTypeDef
      currentNode = entryNode->createChild<GenericTypeDefNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::GlobalVarDefContext *>(subTree); rule != nullptr) // GlobalVarDef
      currentNode = entryNode->createChild<GlobalVarDefNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::ImportStmtContext *>(subTree); rule != nullptr) // ImportStmt
      currentNode = entryNode->createChild<ImportStmtNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::ExtDeclContext *>(subTree); rule != nullptr) // ExtDecl
      currentNode = entryNode->createChild<ExtDeclNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != entryNode) {
      visit(rule);
      currentNode = entryNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitMainFunctionDef(SpiceParser::MainFunctionDefContext *ctx) {
  auto mainFctDefNode = dynamic_cast<MainFctDefNode *>(currentNode);
  saveErrorMessage(mainFctDefNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::ParamLstContext *>(subTree); rule != nullptr) { // ArgLstDef
      currentNode = mainFctDefNode->createChild<ParamLstNode>(CodeLoc(rule->start, filePath));
      mainFctDefNode->takesArgs = true;
    } else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = mainFctDefNode->createChild<StmtLstNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != mainFctDefNode) {
      visit(rule);
      currentNode = mainFctDefNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitFunctionDef(SpiceParser::FunctionDefContext *ctx) {
  auto fctDefNode = dynamic_cast<FctDefNode *>(currentNode);
  saveErrorMessage(fctDefNode, ctx);

  // Extract function name
  fctDefNode->functionName = fctDefNode->fqFunctionName = getIdentifier(ctx->IDENTIFIER().back());
  if (ctx->IDENTIFIER().size() > 1) {
    fctDefNode->structName = getIdentifier(ctx->IDENTIFIER().front());
    fctDefNode->fqFunctionName = fctDefNode->structName + "." + fctDefNode->functionName;
    fctDefNode->isMethod = true;
  }

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierLstContext *>(subTree); rule != nullptr) // DeclSpecifiers
      currentNode = fctDefNode->createChild<SpecifierLstNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) // DataType
      currentNode = fctDefNode->createChild<DataTypeNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) { // TypeLst
      currentNode = fctDefNode->createChild<TypeLstNode>(CodeLoc(rule->start, filePath));
      fctDefNode->hasTemplateTypes = true;
    } else if (rule = dynamic_cast<SpiceParser::ParamLstContext *>(subTree); rule != nullptr) { // ParamLst
      currentNode = fctDefNode->createChild<ParamLstNode>(CodeLoc(rule->start, filePath));
      fctDefNode->hasParams = true;
    } else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = fctDefNode->createChild<StmtLstNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != fctDefNode) {
      visit(rule);
      currentNode = fctDefNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) {
  auto procDefNode = dynamic_cast<ProcDefNode *>(currentNode);
  saveErrorMessage(procDefNode, ctx);

  // Extract procedure name
  procDefNode->procedureName = procDefNode->fqProcedureName = getIdentifier(ctx->IDENTIFIER().back());
  if (ctx->IDENTIFIER().size() > 1) {
    procDefNode->structName = getIdentifier(ctx->IDENTIFIER().front());
    procDefNode->fqProcedureName = procDefNode->structName + "." + procDefNode->procedureName;
    procDefNode->isMethod = true;
  }

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierLstContext *>(subTree); rule != nullptr) // DeclSpecifiers
      currentNode = procDefNode->createChild<SpecifierLstNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) { // TypeLst
      currentNode = procDefNode->createChild<TypeLstNode>(CodeLoc(rule->start, filePath));
      procDefNode->hasTemplateTypes = true;
    } else if (rule = dynamic_cast<SpiceParser::ParamLstContext *>(subTree); rule != nullptr) { // ParamLst
      currentNode = procDefNode->createChild<ParamLstNode>(CodeLoc(rule->start, filePath));
      procDefNode->hasParams = true;
    } else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = procDefNode->createChild<StmtLstNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != procDefNode) {
      visit(rule);
      currentNode = procDefNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitStructDef(SpiceParser::StructDefContext *ctx) {
  auto structDefNode = dynamic_cast<StructDefNode *>(currentNode);
  saveErrorMessage(structDefNode, ctx);

  // Extract struct name
  structDefNode->structName = getIdentifier(ctx->IDENTIFIER());

  bool seenStructKeyword = false;
  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierLstContext *>(subTree); rule != nullptr) // DeclSpecifiers
      currentNode = structDefNode->createChild<SpecifierLstNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) { // TypeLst
      if (!seenStructKeyword) {                                                              // Template type list
        currentNode = structDefNode->createChild<TypeLstNode>(CodeLoc(rule->start, filePath));
        structDefNode->isGeneric = true;
      } else { // Interface type list
        currentNode = structDefNode->createChild<TypeLstNode>(CodeLoc(rule->start, filePath));
        structDefNode->hasInterfaces = true;
      }
    } else if (rule = dynamic_cast<SpiceParser::FieldContext *>(subTree); rule != nullptr) // Field
      currentNode = structDefNode->createChild<FieldNode>(CodeLoc(rule->start, filePath));
    else {
      auto token = dynamic_cast<antlr4::tree::TerminalNode *>(subTree);
      assert(token); // Fail if we did not get a terminal
      if (token->getSymbol()->getType() == SpiceParser::STRUCT)
        seenStructKeyword = true;
    }

    if (currentNode != structDefNode) {
      visit(rule);
      currentNode = structDefNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitInterfaceDef(SpiceParser::InterfaceDefContext *ctx) {
  auto interfaceDefNode = dynamic_cast<InterfaceDefNode *>(currentNode);
  saveErrorMessage(interfaceDefNode, ctx);

  // Extract interface name
  interfaceDefNode->interfaceName = getIdentifier(ctx->IDENTIFIER());

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierLstContext *>(subTree); rule != nullptr) // DeclSpecifiers
      currentNode = interfaceDefNode->createChild<SpecifierLstNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::SignatureContext *>(subTree); rule != nullptr) // Signature
      currentNode = interfaceDefNode->createChild<SignatureNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != interfaceDefNode) {
      visit(rule);
      currentNode = interfaceDefNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitEnumDef(SpiceParser::EnumDefContext *ctx) {
  auto enumDefNode = dynamic_cast<EnumDefNode *>(currentNode);
  saveErrorMessage(enumDefNode, ctx);

  // Extract enum name
  enumDefNode->enumName = getIdentifier(ctx->IDENTIFIER());

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierLstContext *>(subTree); rule != nullptr) // DeclSpecifiers
      currentNode = enumDefNode->createChild<SpecifierLstNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::EnumItemLstContext *>(subTree); rule != nullptr) // EnumItemLst
      currentNode = enumDefNode->createChild<EnumItemLstNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != enumDefNode) {
      visit(rule);
      currentNode = enumDefNode;
    }
  }

  return nullptr;
}

std::any ASTBuilder::visitGenericTypeDef(SpiceParser::GenericTypeDefContext *ctx) {
  auto genericTypeDefNode = dynamic_cast<GenericTypeDefNode *>(currentNode);
  saveErrorMessage(genericTypeDefNode, ctx);

  // Extract type name
  genericTypeDefNode->typeName = getIdentifier(ctx->IDENTIFIER());

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierLstContext *>(subTree); rule != nullptr) // DeclSpecifiers
      currentNode = genericTypeDefNode->createChild<SpecifierLstNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::TypeAltsLstContext *>(subTree); rule != nullptr) // TypeAltsLst
      currentNode = genericTypeDefNode->createChild<TypeAltsLstNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != genericTypeDefNode) {
      visit(rule);
      currentNode = genericTypeDefNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitGlobalVarDef(SpiceParser::GlobalVarDefContext *ctx) {
  auto globalVarDefNode = dynamic_cast<GlobalVarDefNode *>(currentNode);
  saveErrorMessage(globalVarDefNode, ctx);

  // Extract function name
  globalVarDefNode->varName = getIdentifier(ctx->IDENTIFIER());

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierLstContext *>(subTree); rule != nullptr) // DeclSpecifiers
      currentNode = globalVarDefNode->createChild<SpecifierLstNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) // DataType
      currentNode = globalVarDefNode->createChild<DataTypeNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::ConstantContext *>(subTree); rule != nullptr) // Constant
      currentNode = globalVarDefNode->createChild<ConstantNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != globalVarDefNode) {
      visit(rule);
      currentNode = globalVarDefNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitExtDecl(SpiceParser::ExtDeclContext *ctx) {
  auto extDeclNode = dynamic_cast<ExtDeclNode *>(currentNode);
  saveErrorMessage(extDeclNode, ctx);

  // Extract function name
  extDeclNode->extFunctionName = getIdentifier(ctx->IDENTIFIER());
  // Extract isDll
  extDeclNode->isDll = ctx->DLL() != nullptr;

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) // DataType
      currentNode = extDeclNode->createChild<DataTypeNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) { // TypeLst
      currentNode = extDeclNode->createChild<TypeLstNode>(CodeLoc(rule->start, filePath));
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

std::any ASTBuilder::visitThreadDef(SpiceParser::ThreadDefContext *ctx) {
  auto threadDefNode = dynamic_cast<ThreadDefNode *>(currentNode);
  saveErrorMessage(threadDefNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = threadDefNode->createChild<StmtLstNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != threadDefNode) {
      visit(rule);
      currentNode = threadDefNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitUnsafeBlockDef(SpiceParser::UnsafeBlockDefContext *ctx) {
  auto unsafeBlockDefNode = dynamic_cast<UnsafeBlockDefNode *>(currentNode);
  saveErrorMessage(unsafeBlockDefNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = unsafeBlockDefNode->createChild<StmtLstNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != unsafeBlockDefNode) {
      visit(rule);
      currentNode = unsafeBlockDefNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitForLoop(SpiceParser::ForLoopContext *ctx) {
  auto forLoopNode = dynamic_cast<ForLoopNode *>(currentNode);
  saveErrorMessage(forLoopNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::ForHeadContext *>(subTree); rule != nullptr) // ForHead
      visit(rule);
    else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = forLoopNode->createChild<StmtLstNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != forLoopNode) {
      visit(rule);
      currentNode = forLoopNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitForHead(SpiceParser::ForHeadContext *ctx) {
  auto forLoopNode = dynamic_cast<ForLoopNode *>(currentNode);
  saveErrorMessage(forLoopNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DeclStmtContext *>(subTree); rule != nullptr) // DeclStmt
      currentNode = forLoopNode->createChild<DeclStmtNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = forLoopNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != forLoopNode) {
      visit(rule);
      currentNode = forLoopNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitForeachLoop(SpiceParser::ForeachLoopContext *ctx) {
  auto foreachLoopNode = dynamic_cast<ForeachLoopNode *>(currentNode);
  saveErrorMessage(foreachLoopNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::ForeachHeadContext *>(subTree); rule != nullptr) // ForeachHead
      visit(rule);
    else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = foreachLoopNode->createChild<StmtLstNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != foreachLoopNode) {
      visit(rule);
      currentNode = foreachLoopNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitForeachHead(SpiceParser::ForeachHeadContext *ctx) {
  auto foreachLoopNode = dynamic_cast<ForeachLoopNode *>(currentNode);
  saveErrorMessage(foreachLoopNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DeclStmtContext *>(subTree); rule != nullptr) // DeclStmt
      currentNode = foreachLoopNode->createChild<DeclStmtNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = foreachLoopNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != foreachLoopNode) {
      visit(rule);
      currentNode = foreachLoopNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitWhileLoop(SpiceParser::WhileLoopContext *ctx) {
  auto whileLoopNode = dynamic_cast<WhileLoopNode *>(currentNode);
  saveErrorMessage(whileLoopNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = whileLoopNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = whileLoopNode->createChild<StmtLstNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != whileLoopNode) {
      visit(rule);
      currentNode = whileLoopNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitIfStmt(SpiceParser::IfStmtContext *ctx) {
  auto ifStmtNode = dynamic_cast<IfStmtNode *>(currentNode);
  saveErrorMessage(ifStmtNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = ifStmtNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = ifStmtNode->createChild<StmtLstNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::ElseStmtContext *>(subTree); rule != nullptr) // ElseStmt
      currentNode = ifStmtNode->createChild<ElseStmtNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != ifStmtNode) {
      visit(rule);
      currentNode = ifStmtNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitElseStmt(SpiceParser::ElseStmtContext *ctx) {
  auto elseStmtNode = dynamic_cast<ElseStmtNode *>(currentNode);
  saveErrorMessage(elseStmtNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::IfStmtContext *>(subTree); rule != nullptr) { // IfStmt
      currentNode = elseStmtNode->createChild<IfStmtNode>(CodeLoc(rule->start, filePath));
      elseStmtNode->isElseIf = true;
    } else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = elseStmtNode->createChild<StmtLstNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != elseStmtNode) {
      visit(rule);
      currentNode = elseStmtNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitAssertStmt(SpiceParser::AssertStmtContext *ctx) {
  auto assertStmtNode = dynamic_cast<AssertStmtNode *>(currentNode);
  saveErrorMessage(assertStmtNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) { // AssignExpr
      currentNode = assertStmtNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
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

std::any ASTBuilder::visitAnonymousBlockStmt(SpiceParser::AnonymousBlockStmtContext *ctx) {
  auto blockStmtNode = dynamic_cast<AnonymousBlockStmtNode *>(currentNode);
  saveErrorMessage(blockStmtNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = blockStmtNode->createChild<StmtLstNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != blockStmtNode) {
      visit(rule);
      currentNode = blockStmtNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitStmtLst(SpiceParser::StmtLstContext *ctx) {
  auto stmtLstNode = dynamic_cast<StmtLstNode *>(currentNode);
  saveErrorMessage(stmtLstNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::StmtContext *>(subTree); rule != nullptr) // Stmt
      currentNode = stmtLstNode->createChild<StmtNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::ForLoopContext *>(subTree); rule != nullptr) // ForLoop
      currentNode = stmtLstNode->createChild<ForLoopNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::ForeachLoopContext *>(subTree); rule != nullptr) // ForeachLoop
      currentNode = stmtLstNode->createChild<ForeachLoopNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::WhileLoopContext *>(subTree); rule != nullptr) // WhileLoop
      currentNode = stmtLstNode->createChild<WhileLoopNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::IfStmtContext *>(subTree); rule != nullptr) // IfStmt
      currentNode = stmtLstNode->createChild<IfStmtNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::AssertStmtContext *>(subTree); rule != nullptr) // AssertStmt
      currentNode = stmtLstNode->createChild<AssertStmtNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::ThreadDefContext *>(subTree); rule != nullptr) // ThreadDef
      currentNode = stmtLstNode->createChild<ThreadDefNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::UnsafeBlockDefContext *>(subTree); rule != nullptr) // UnsafeBlockDef
      currentNode = stmtLstNode->createChild<UnsafeBlockDefNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::AnonymousBlockStmtContext *>(subTree); rule != nullptr) // AnonymousBlockStmt
      currentNode = stmtLstNode->createChild<AnonymousBlockStmtNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    // Sum statements as complexity
    stmtLstNode->complexity++;

    if (currentNode != stmtLstNode) {
      visit(rule);
      currentNode = stmtLstNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitTypeLst(SpiceParser::TypeLstContext *ctx) {
  auto typeLstNode = dynamic_cast<TypeLstNode *>(currentNode);
  saveErrorMessage(typeLstNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) { // DataType
      currentNode = typeLstNode->createChild<DataTypeNode>(CodeLoc(rule->start, filePath));
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

std::any ASTBuilder::visitTypeAltsLst(SpiceParser::TypeAltsLstContext *ctx) {
  auto typeAltsLstNode = dynamic_cast<TypeAltsLstNode *>(currentNode);
  saveErrorMessage(typeAltsLstNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) { // DataType
      currentNode = typeAltsLstNode->createChild<DataTypeNode>(CodeLoc(rule->start, filePath));
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

std::any ASTBuilder::visitParamLst(SpiceParser::ParamLstContext *ctx) {
  auto argLstDefNode = dynamic_cast<ParamLstNode *>(currentNode);
  saveErrorMessage(argLstDefNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DeclStmtContext *>(subTree); rule != nullptr) // DeclStmt
      currentNode = argLstDefNode->createChild<DeclStmtNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != argLstDefNode) {
      visit(rule);
      currentNode = argLstDefNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitArgLst(SpiceParser::ArgLstContext *ctx) {
  auto argLstNode = dynamic_cast<ArgLstNode *>(currentNode);
  saveErrorMessage(argLstNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignStmt
      currentNode = argLstNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != argLstNode) {
      visit(rule);
      currentNode = argLstNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitEnumItemLst(SpiceParser::EnumItemLstContext *ctx) {
  auto enumItemLstNode = dynamic_cast<EnumItemLstNode *>(currentNode);
  saveErrorMessage(enumItemLstNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::EnumItemContext *>(subTree); rule != nullptr) // EnumItem
      currentNode = enumItemLstNode->createChild<EnumItemNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != enumItemLstNode) {
      visit(rule);
      currentNode = enumItemLstNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitEnumItem(SpiceParser::EnumItemContext *ctx) {
  auto enumItemNode = dynamic_cast<EnumItemNode *>(currentNode);
  saveErrorMessage(enumItemNode, ctx);

  for (const auto &subTree : ctx->children) {
    if (auto t1 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t1->getSymbol()->getType() == SpiceParser::IDENTIFIER)
      enumItemNode->itemName = getIdentifier(t1);
    else if (auto t2 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t2->getSymbol()->getType() == SpiceParser::INT_LIT) {
      enumItemNode->itemValue = parseInt(t2);
      enumItemNode->hasValue = true;
    } else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal
  }
  return nullptr;
}

std::any ASTBuilder::visitField(SpiceParser::FieldContext *ctx) {
  auto fieldNode = dynamic_cast<FieldNode *>(currentNode);
  saveErrorMessage(fieldNode, ctx);

  // Extract field name
  fieldNode->fieldName = getIdentifier(ctx->IDENTIFIER());

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierLstContext *>(subTree); rule != nullptr) // DeclSpecifiers
      currentNode = fieldNode->createChild<SpecifierLstNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) // DataType
      currentNode = fieldNode->createChild<DataTypeNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != fieldNode) {
      visit(rule);
      currentNode = fieldNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitSignature(SpiceParser::SignatureContext *ctx) {
  auto signatureNode = dynamic_cast<SignatureNode *>(currentNode);
  saveErrorMessage(signatureNode, ctx);

  // Extract method name
  signatureNode->methodName = getIdentifier(ctx->IDENTIFIER());
  // Extract signature type
  signatureNode->signatureType = ctx->F() != nullptr ? SignatureNode::TYPE_FUNCTION : SignatureNode::TYPE_PROCEDURE;

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierLstContext *>(subTree); rule != nullptr) // DeclSpecifiers
      currentNode = signatureNode->createChild<SpecifierLstNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) { // DataType
      currentNode = signatureNode->createChild<DataTypeNode>(CodeLoc(rule->start, filePath));
      signatureNode->signatureType = SignatureNode::TYPE_FUNCTION;
    } else if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) { // TypeLst
      currentNode = signatureNode->createChild<TypeLstNode>(CodeLoc(rule->start, filePath));
      signatureNode->hasParams = true;
    } else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != signatureNode) {
      visit(rule);
      currentNode = signatureNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitStmt(SpiceParser::StmtContext *ctx) {
  auto stmtNode = dynamic_cast<StmtNode *>(currentNode);
  saveErrorMessage(stmtNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DeclStmtContext *>(subTree); rule != nullptr) // DeclStmt
      currentNode = stmtNode->createChild<DeclStmtNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = stmtNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::ReturnStmtContext *>(subTree); rule != nullptr) // ReturnStmt
      currentNode = stmtNode->createChild<ReturnStmtNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::BreakStmtContext *>(subTree); rule != nullptr) // BreakStmt
      currentNode = stmtNode->createChild<BreakStmtNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::ContinueStmtContext *>(subTree); rule != nullptr) // ContinueStmt
      currentNode = stmtNode->createChild<ContinueStmtNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != stmtNode) {
      visit(rule);
      currentNode = stmtNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitDeclStmt(SpiceParser::DeclStmtContext *ctx) {
  auto declStmtNode = dynamic_cast<DeclStmtNode *>(currentNode);
  saveErrorMessage(declStmtNode, ctx);

  // Extract var name
  declStmtNode->varName = getIdentifier(ctx->IDENTIFIER());

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierLstContext *>(subTree); rule != nullptr) // DeclSpecifiers
      currentNode = declStmtNode->createChild<SpecifierLstNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) // DataType
      currentNode = declStmtNode->createChild<DataTypeNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) { // AssignExpr
      currentNode = declStmtNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
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

std::any ASTBuilder::visitSpecifierLst(SpiceParser::SpecifierLstContext *ctx) {
  auto specifierLstNode = dynamic_cast<SpecifierLstNode *>(currentNode);
  saveErrorMessage(specifierLstNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierContext *>(subTree); rule != nullptr) // DeclSpecifier
      currentNode = specifierLstNode->createChild<SpecifierNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != specifierLstNode) {
      visit(rule);
      currentNode = specifierLstNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitSpecifier(SpiceParser::SpecifierContext *ctx) {
  auto specifierNode = dynamic_cast<SpecifierNode *>(currentNode);
  saveErrorMessage(specifierNode, ctx);

  for (const auto &subTree : ctx->children) {
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
    else if (symbolType == SpiceParser::HEAP)
      specifierNode->type = SpecifierNode::TY_HEAP;
    else
      assert(false);
  }
  return nullptr;
}

std::any ASTBuilder::visitImportStmt(SpiceParser::ImportStmtContext *ctx) {
  auto importStmtNode = dynamic_cast<ImportStmtNode *>(currentNode);
  saveErrorMessage(importStmtNode, ctx);

  // Extract path
  std::string pathStr = ctx->STRING_LIT()->getText();
  importStmtNode->importPath = pathStr.substr(1, pathStr.size() - 2);

  // Extract name
  importStmtNode->importName = getIdentifier(ctx->IDENTIFIER());

  return nullptr;
}

std::any ASTBuilder::visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) {
  auto returnStmtNode = dynamic_cast<ReturnStmtNode *>(currentNode);
  saveErrorMessage(returnStmtNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) { // AssignExpr
      currentNode = returnStmtNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
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

std::any ASTBuilder::visitBreakStmt(SpiceParser::BreakStmtContext *ctx) {
  auto breakStmtNode = dynamic_cast<BreakStmtNode *>(currentNode);
  saveErrorMessage(breakStmtNode, ctx);

  // Extract number of breaks
  if (ctx->INT_LIT())
    breakStmtNode->breakTimes = std::stoi(ctx->INT_LIT()->toString());

  return nullptr;
}

std::any ASTBuilder::visitContinueStmt(SpiceParser::ContinueStmtContext *ctx) {
  auto continueStmtNode = dynamic_cast<ContinueStmtNode *>(currentNode);
  saveErrorMessage(continueStmtNode, ctx);

  // Extract number of continues
  if (ctx->INT_LIT())
    continueStmtNode->continueTimes = std::stoi(ctx->INT_LIT()->toString());

  return nullptr;
}

std::any ASTBuilder::visitBuiltinCall(SpiceParser::BuiltinCallContext *ctx) {
  auto atomicExprNode = dynamic_cast<AtomicExprNode *>(currentNode);
  saveErrorMessage(atomicExprNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::PrintfCallContext *>(subTree); rule != nullptr) // PrintfCall
      currentNode = atomicExprNode->createChild<PrintfCallNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::SizeOfCallContext *>(subTree); rule != nullptr) // SizeofCall
      currentNode = atomicExprNode->createChild<SizeofCallNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::LenCallContext *>(subTree); rule != nullptr) // LenCall
      currentNode = atomicExprNode->createChild<LenCallNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::TidCallContext *>(subTree); rule != nullptr) // TidCall
      currentNode = atomicExprNode->createChild<TidCallNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::JoinCallContext *>(subTree); rule != nullptr) // JoinCall
      currentNode = atomicExprNode->createChild<JoinCallNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != atomicExprNode) {
      visit(rule);
      currentNode = atomicExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitPrintfCall(SpiceParser::PrintfCallContext *ctx) {
  auto printfCallNode = dynamic_cast<PrintfCallNode *>(currentNode);
  saveErrorMessage(printfCallNode, ctx);

  // Extract templated string
  std::string templatedString = ctx->STRING_LIT()->getText();
  templatedString = templatedString.substr(1, templatedString.size() - 2);
  replaceEscapeChars(templatedString);
  printfCallNode->templatedString = templatedString;

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = printfCallNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != printfCallNode) {
      visit(rule);
      currentNode = printfCallNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitSizeOfCall(SpiceParser::SizeOfCallContext *ctx) {
  auto sizeofCallNode = dynamic_cast<SizeofCallNode *>(currentNode);
  saveErrorMessage(sizeofCallNode, ctx);

  // Check if type or value
  sizeofCallNode->isType = ctx->TYPE();

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = sizeofCallNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) // DataType
      currentNode = sizeofCallNode->createChild<DataTypeNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != sizeofCallNode) {
      visit(rule);
      currentNode = sizeofCallNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitLenCall(SpiceParser::LenCallContext *ctx) {
  auto lenCallNode = dynamic_cast<LenCallNode *>(currentNode);
  saveErrorMessage(lenCallNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = lenCallNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != lenCallNode) {
      visit(rule);
      currentNode = lenCallNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitTidCall(SpiceParser::TidCallContext *ctx) {
  auto tidCallNode = dynamic_cast<TidCallNode *>(currentNode);
  saveErrorMessage(tidCallNode, ctx);
  return nullptr;
}

std::any ASTBuilder::visitJoinCall(SpiceParser::JoinCallContext *ctx) {
  auto joinCallNode = dynamic_cast<JoinCallNode *>(currentNode);
  saveErrorMessage(joinCallNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = joinCallNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != joinCallNode) {
      visit(rule);
      currentNode = joinCallNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitAssignExpr(SpiceParser::AssignExprContext *ctx) {
  auto assignExprNode = dynamic_cast<AssignExprNode *>(currentNode);
  saveErrorMessage(assignExprNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::PrefixUnaryExprContext *>(subTree); rule != nullptr) // PrefixUnaryExpr
      currentNode = assignExprNode->createChild<PrefixUnaryExprNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::AssignOpContext *>(subTree); rule != nullptr) // AssignOp
      visit(rule);
    else if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = assignExprNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::TernaryExprContext *>(subTree); rule != nullptr) // TernaryExpr
      currentNode = assignExprNode->createChild<TernaryExprNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::ThreadDefContext *>(subTree); rule != nullptr) // ThreadDef
      currentNode = assignExprNode->createChild<ThreadDefNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != assignExprNode) {
      visit(rule);
      currentNode = assignExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitTernaryExpr(SpiceParser::TernaryExprContext *ctx) {
  auto ternaryExprNode = dynamic_cast<TernaryExprNode *>(currentNode);
  saveErrorMessage(ternaryExprNode, ctx);

  // Check if is shortened
  ternaryExprNode->isShortened = ctx->logicalOrExpr().size() == 2;

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::LogicalOrExprContext *>(subTree); rule != nullptr) // LogicalOrExpr
      currentNode = ternaryExprNode->createChild<LogicalOrExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != ternaryExprNode) {
      visit(rule);
      currentNode = ternaryExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) {
  auto logicalOrExprNode = dynamic_cast<LogicalOrExprNode *>(currentNode);
  saveErrorMessage(logicalOrExprNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::LogicalAndExprContext *>(subTree); rule != nullptr) // LogicalAndExpr
      currentNode = logicalOrExprNode->createChild<LogicalAndExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != logicalOrExprNode) {
      visit(rule);
      currentNode = logicalOrExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) {
  auto logicalAndExprNode = dynamic_cast<LogicalAndExprNode *>(currentNode);
  saveErrorMessage(logicalAndExprNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::BitwiseOrExprContext *>(subTree); rule != nullptr) // BitwiseOr
      currentNode = logicalAndExprNode->createChild<BitwiseOrExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != logicalAndExprNode) {
      visit(rule);
      currentNode = logicalAndExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) {
  auto bitwiseOrExprNode = dynamic_cast<BitwiseOrExprNode *>(currentNode);
  saveErrorMessage(bitwiseOrExprNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::BitwiseXorExprContext *>(subTree); rule != nullptr) // BitwiseXor
      currentNode = bitwiseOrExprNode->createChild<BitwiseXorExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != bitwiseOrExprNode) {
      visit(rule);
      currentNode = bitwiseOrExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitBitwiseXorExpr(SpiceParser::BitwiseXorExprContext *ctx) {
  auto bitwiseXorExprNode = dynamic_cast<BitwiseXorExprNode *>(currentNode);
  saveErrorMessage(bitwiseXorExprNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::BitwiseAndExprContext *>(subTree); rule != nullptr) // BitwiseAnd
      currentNode = bitwiseXorExprNode->createChild<BitwiseAndExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != bitwiseXorExprNode) {
      visit(rule);
      currentNode = bitwiseXorExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) {
  auto bitwiseAndExprNode = dynamic_cast<BitwiseAndExprNode *>(currentNode);
  saveErrorMessage(bitwiseAndExprNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::EqualityExprContext *>(subTree); rule != nullptr) // EqualityExpr
      currentNode = bitwiseAndExprNode->createChild<EqualityExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != bitwiseAndExprNode) {
      visit(rule);
      currentNode = bitwiseAndExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitEqualityExpr(SpiceParser::EqualityExprContext *ctx) {
  auto equalityExprNode = dynamic_cast<EqualityExprNode *>(currentNode);
  saveErrorMessage(equalityExprNode, ctx);

  // Extract operator
  if (ctx->EQUAL())
    equalityExprNode->op = EqualityExprNode::OP_EQUAL;
  else if (ctx->NOT_EQUAL())
    equalityExprNode->op = EqualityExprNode::OP_NOT_EQUAL;

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::RelationalExprContext *>(subTree); rule != nullptr) // RelationalExpr
      currentNode = equalityExprNode->createChild<RelationalExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != equalityExprNode) {
      visit(rule);
      currentNode = equalityExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) {
  auto relationalExprNode = dynamic_cast<RelationalExprNode *>(currentNode);
  saveErrorMessage(relationalExprNode, ctx);

  // Extract operator
  if (ctx->LESS())
    relationalExprNode->op = RelationalExprNode::OP_LESS;
  else if (ctx->GREATER())
    relationalExprNode->op = RelationalExprNode::OP_GREATER;
  else if (ctx->LESS_EQUAL())
    relationalExprNode->op = RelationalExprNode::OP_LESS_EQUAL;
  else if (ctx->GREATER_EQUAL())
    relationalExprNode->op = RelationalExprNode::OP_GREATER_EQUAL;

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::ShiftExprContext *>(subTree); rule != nullptr) // ShiftExpr
      currentNode = relationalExprNode->createChild<ShiftExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != relationalExprNode) {
      visit(rule);
      currentNode = relationalExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitShiftExpr(SpiceParser::ShiftExprContext *ctx) {
  auto shiftExprNode = dynamic_cast<ShiftExprNode *>(currentNode);
  saveErrorMessage(shiftExprNode, ctx);

  // Extract operator
  if (!ctx->LESS().empty())
    shiftExprNode->op = ShiftExprNode::OP_SHIFT_LEFT;
  else if (!ctx->GREATER().empty())
    shiftExprNode->op = ShiftExprNode::OP_SHIFT_RIGHT;

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AdditiveExprContext *>(subTree); rule != nullptr) // AdditiveExpr
      currentNode = shiftExprNode->createChild<AdditiveExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != shiftExprNode) {
      visit(rule);
      currentNode = shiftExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) {
  auto additiveExprNode = dynamic_cast<AdditiveExprNode *>(currentNode);
  saveErrorMessage(additiveExprNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::MultiplicativeExprContext *>(subTree); rule != nullptr) // MultiplicativeExpr
      currentNode = additiveExprNode->createChild<MultiplicativeExprNode>(CodeLoc(rule->start, filePath));
    else if (auto t1 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t1->getSymbol()->getType() == SpiceParser::PLUS)
      additiveExprNode->opQueue.emplace(AdditiveExprNode::OP_PLUS, SymbolType(TY_INVALID));
    else if (auto t2 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t2->getSymbol()->getType() == SpiceParser::MINUS)
      additiveExprNode->opQueue.emplace(AdditiveExprNode::OP_MINUS, SymbolType(TY_INVALID));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != additiveExprNode) {
      visit(rule);
      currentNode = additiveExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) {
  auto multiplicativeExprNode = dynamic_cast<MultiplicativeExprNode *>(currentNode);
  saveErrorMessage(multiplicativeExprNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::CastExprContext *>(subTree); rule != nullptr) // CastExpr
      currentNode = multiplicativeExprNode->createChild<CastExprNode>(CodeLoc(rule->start, filePath));
    else if (auto t1 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t1->getSymbol()->getType() == SpiceParser::MUL)
      multiplicativeExprNode->opQueue.emplace(MultiplicativeExprNode::OP_MUL, SymbolType(TY_INVALID));
    else if (auto t2 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t2->getSymbol()->getType() == SpiceParser::DIV)
      multiplicativeExprNode->opQueue.emplace(MultiplicativeExprNode::OP_DIV, SymbolType(TY_INVALID));
    else if (auto t3 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t3->getSymbol()->getType() == SpiceParser::REM)
      multiplicativeExprNode->opQueue.emplace(MultiplicativeExprNode::OP_REM, SymbolType(TY_INVALID));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != multiplicativeExprNode) {
      visit(rule);
      currentNode = multiplicativeExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitCastExpr(SpiceParser::CastExprContext *ctx) {
  auto castExprNode = dynamic_cast<CastExprNode *>(currentNode);
  saveErrorMessage(castExprNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::PrefixUnaryExprContext *>(subTree); rule != nullptr) // PrefixUnaryExpr
      currentNode = castExprNode->createChild<PrefixUnaryExprNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) { // DataType
      currentNode = castExprNode->createChild<DataTypeNode>(CodeLoc(rule->start, filePath));
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

std::any ASTBuilder::visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext *ctx) {
  auto prefixUnaryExprNode = dynamic_cast<PrefixUnaryExprNode *>(currentNode);
  saveErrorMessage(prefixUnaryExprNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::PrefixUnaryOpContext *>(subTree); rule != nullptr) // PrefixUnaryOp
      visit(rule);
    else if (rule = dynamic_cast<SpiceParser::PostfixUnaryExprContext *>(subTree); rule != nullptr) // PostfixUnaryExpr
      currentNode = prefixUnaryExprNode->createChild<PostfixUnaryExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != prefixUnaryExprNode) {
      visit(rule);
      currentNode = prefixUnaryExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext *ctx) {
  auto postfixUnaryExprNode = dynamic_cast<PostfixUnaryExprNode *>(currentNode);
  saveErrorMessage(postfixUnaryExprNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AtomicExprContext *>(subTree); rule != nullptr) // AtomicExpr
      currentNode = postfixUnaryExprNode->createChild<AtomicExprNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = postfixUnaryExprNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::PostfixUnaryExprContext *>(subTree); rule != nullptr) // PostfixUnaryExpr
      currentNode = postfixUnaryExprNode->createChild<PostfixUnaryExprNode>(CodeLoc(rule->start, filePath));
    else if (auto t1 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t1->getSymbol()->getType() == SpiceParser::LBRACKET)
      postfixUnaryExprNode->opQueue.emplace(PostfixUnaryExprNode::OP_SUBSCRIPT, SymbolType(TY_INVALID));
    else if (auto t2 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t2->getSymbol()->getType() == SpiceParser::DOT)
      postfixUnaryExprNode->opQueue.emplace(PostfixUnaryExprNode::OP_MEMBER_ACCESS, SymbolType(TY_INVALID));
    else if (auto t3 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t3->getSymbol()->getType() == SpiceParser::PLUS_PLUS)
      postfixUnaryExprNode->opQueue.emplace(PostfixUnaryExprNode::OP_PLUS_PLUS, SymbolType(TY_INVALID));
    else if (auto t4 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree);
             t4->getSymbol()->getType() == SpiceParser::MINUS_MINUS)
      postfixUnaryExprNode->opQueue.emplace(PostfixUnaryExprNode::OP_MINUS_MINUS, SymbolType(TY_INVALID));
    else if (auto t5 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree);
             t5->getSymbol()->getType() == SpiceParser::SCOPE_ACCESS)
      postfixUnaryExprNode->opQueue.emplace(PostfixUnaryExprNode::OP_SCOPE_ACCESS, SymbolType(TY_INVALID));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != postfixUnaryExprNode) {
      visit(rule);
      currentNode = postfixUnaryExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) {
  auto atomicExprNode = dynamic_cast<AtomicExprNode *>(currentNode);
  saveErrorMessage(atomicExprNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::ConstantContext *>(subTree); rule != nullptr) // Constant
      currentNode = atomicExprNode->createChild<ConstantNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::ValueContext *>(subTree); rule != nullptr) // Value
      currentNode = atomicExprNode->createChild<ValueNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::BuiltinCallContext *>(subTree); rule != nullptr) // BuiltinCall
      visit(rule);
    else if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = atomicExprNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
    else if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::IDENTIFIER)
      atomicExprNode->identifier = getIdentifier(t);
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != atomicExprNode) {
      visit(rule);
      currentNode = atomicExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitValue(SpiceParser::ValueContext *ctx) {
  auto valueNode = dynamic_cast<ValueNode *>(currentNode);
  saveErrorMessage(valueNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::FunctionCallContext *>(subTree); rule != nullptr) // FunctionCall
      currentNode = valueNode->createChild<FunctionCallNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::ArrayInitializationContext *>(subTree); rule != nullptr) // ArrayInitialization
      currentNode = valueNode->createChild<ArrayInitializationNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::StructInstantiationContext *>(subTree); rule != nullptr) // StructInstantiation
      currentNode = valueNode->createChild<StructInstantiationNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) { // DataType
      currentNode = valueNode->createChild<DataTypeNode>(CodeLoc(rule->start, filePath));
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

std::any ASTBuilder::visitConstant(SpiceParser::ConstantContext *ctx) {
  auto constantNode = dynamic_cast<ConstantNode *>(currentNode);
  saveErrorMessage(constantNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (auto t = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::DOUBLE_LIT) {
      constantNode->type = ConstantNode::TYPE_DOUBLE;
      constantNode->compileTimeValue.doubleValue = std::stod(t->toString());
      constantNode->hasDirectCompileTimeValue = true;
    } else if (auto t1 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree);
               t1->getSymbol()->getType() == SpiceParser::INT_LIT) {
      constantNode->type = ConstantNode::TYPE_INT;
      constantNode->compileTimeValue.intValue = parseInt(t1);
      constantNode->hasDirectCompileTimeValue = true;
    } else if (auto t2 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree);
               t2->getSymbol()->getType() == SpiceParser::SHORT_LIT) {
      constantNode->type = ConstantNode::TYPE_SHORT;
      constantNode->compileTimeValue.shortValue = parseShort(t2);
      constantNode->hasDirectCompileTimeValue = true;
    } else if (auto t3 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree);
               t3->getSymbol()->getType() == SpiceParser::LONG_LIT) {
      constantNode->type = ConstantNode::TYPE_LONG;
      constantNode->compileTimeValue.longValue = parseLong(t3);
      constantNode->hasDirectCompileTimeValue = true;
    } else if (auto t4 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree);
               t4->getSymbol()->getType() == SpiceParser::CHAR_LIT) {
      constantNode->type = ConstantNode::TYPE_CHAR;
      constantNode->compileTimeValue.charValue = parseChar(ctx->CHAR_LIT());
      constantNode->hasDirectCompileTimeValue = true;
    } else if (auto t5 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree);
               t5->getSymbol()->getType() == SpiceParser::STRING_LIT) {
      std::string stringValue = parseString(ctx->STRING_LIT()->toString());

      constantNode->type = ConstantNode::TYPE_STRING;
      constantNode->compileTimeStringValue = stringValue;
      constantNode->compileTimeValue.stringValue = constantNode->compileTimeStringValue.c_str();
      constantNode->hasDirectCompileTimeValue = true;
    } else if (auto t6 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t6->getSymbol()->getType() == SpiceParser::TRUE) {
      constantNode->type = ConstantNode::TYPE_BOOL;
      constantNode->compileTimeValue.boolValue = true;
      constantNode->hasDirectCompileTimeValue = true;
    } else if (auto t7 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t7->getSymbol()->getType() == SpiceParser::FALSE) {
      constantNode->type = ConstantNode::TYPE_BOOL;
      constantNode->compileTimeValue.boolValue = false;
      constantNode->hasDirectCompileTimeValue = true;
    } else {
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal
    }

    if (currentNode != constantNode) {
      visit(rule);
      currentNode = constantNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitFunctionCall(SpiceParser::FunctionCallContext *ctx) {
  auto fctCallNode = dynamic_cast<FunctionCallNode *>(currentNode);
  saveErrorMessage(fctCallNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) { // TypeLst
      currentNode = fctCallNode->createChild<TypeLstNode>(CodeLoc(rule->start, filePath));
      fctCallNode->hasTemplateTypes = true;
    } else if (rule = dynamic_cast<SpiceParser::ArgLstContext *>(subTree); rule != nullptr) { // ArgLst
      currentNode = fctCallNode->createChild<ArgLstNode>(CodeLoc(rule->start, filePath));
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

std::any ASTBuilder::visitArrayInitialization(SpiceParser::ArrayInitializationContext *ctx) {
  auto arrayInitializationNode = dynamic_cast<ArrayInitializationNode *>(currentNode);
  saveErrorMessage(arrayInitializationNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::ArgLstContext *>(subTree); rule != nullptr) // ArgLst
      currentNode = arrayInitializationNode->createChild<ArgLstNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != arrayInitializationNode) {
      visit(rule);
      currentNode = arrayInitializationNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitStructInstantiation(SpiceParser::StructInstantiationContext *ctx) {
  auto structInstantiationNode = dynamic_cast<StructInstantiationNode *>(currentNode);
  saveErrorMessage(structInstantiationNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) // TypeLst
      currentNode = structInstantiationNode->createChild<TypeLstNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::ArgLstContext *>(subTree); rule != nullptr) // ArgLst
      currentNode = structInstantiationNode->createChild<ArgLstNode>(CodeLoc(rule->start, filePath));
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

std::any ASTBuilder::visitDataType(SpiceParser::DataTypeContext *ctx) {
  auto dataTypeNode = dynamic_cast<DataTypeNode *>(currentNode);
  saveErrorMessage(dataTypeNode, ctx);

  for (int i = 0; i < ctx->children.size(); i++) {
    auto subTree = ctx->children[i];
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::BaseDataTypeContext *>(subTree); rule != nullptr) // BaseDataType
      currentNode = dataTypeNode->createChild<BaseDataTypeNode>(CodeLoc(rule->start, filePath));
    else if (auto t1 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t1->getSymbol()->getType() == SpiceParser::MUL)
      dataTypeNode->tmQueue.push({DataTypeNode::TYPE_PTR, false, false, 0});
    else if (auto t2 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree);
             t1->getSymbol()->getType() == SpiceParser::BITWISE_AND)
      dataTypeNode->tmQueue.push({DataTypeNode::TYPE_REF, false, false, 0});
    else if (auto t3 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t2->getSymbol()->getType() == SpiceParser::LBRACKET) {
      i++; // Consume LBRACKET
      subTree = ctx->children[i];
      bool hasSize = false;
      bool isHardcoded = false;
      int hardCodedSize = 0;
      if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) { // AssignExpr
        hasSize = true;
        hardCodedSize = -1;
        currentNode = dataTypeNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
        i++; // Consume INTEGER
      } else if (auto t4 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree);
                 t4->getSymbol()->getType() == SpiceParser::INT_LIT) {
        hasSize = true;
        isHardcoded = true;
        hardCodedSize = std::stoi(t4->getSymbol()->getText());
        i++; // Consume INTEGER
      }
      dataTypeNode->tmQueue.push({DataTypeNode::TYPE_ARRAY, hasSize, isHardcoded, hardCodedSize});
    } else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != dataTypeNode) {
      visit(rule);
      currentNode = dataTypeNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitBaseDataType(SpiceParser::BaseDataTypeContext *ctx) {
  auto baseDataTypeNode = dynamic_cast<BaseDataTypeNode *>(currentNode);
  saveErrorMessage(baseDataTypeNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::CustomDataTypeContext *>(subTree); rule != nullptr) { // CustomDataType
      baseDataTypeNode->type = BaseDataTypeNode::TY_CUSTOM;
      currentNode = baseDataTypeNode->createChild<CustomDataTypeNode>(CodeLoc(rule->start, filePath));
    } else if (auto t1 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree);
               t1->getSymbol()->getType() == SpiceParser::TYPE_DOUBLE)
      baseDataTypeNode->type = BaseDataTypeNode::TYPE_DOUBLE;
    else if (auto t2 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t2->getSymbol()->getType() == SpiceParser::TYPE_INT)
      baseDataTypeNode->type = BaseDataTypeNode::TYPE_INT;
    else if (auto t3 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t3->getSymbol()->getType() == SpiceParser::TYPE_SHORT)
      baseDataTypeNode->type = BaseDataTypeNode::TYPE_SHORT;
    else if (auto t4 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t4->getSymbol()->getType() == SpiceParser::TYPE_LONG)
      baseDataTypeNode->type = BaseDataTypeNode::TYPE_LONG;
    else if (auto t5 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t5->getSymbol()->getType() == SpiceParser::TYPE_BYTE)
      baseDataTypeNode->type = BaseDataTypeNode::TYPE_BYTE;
    else if (auto t6 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t6->getSymbol()->getType() == SpiceParser::TYPE_CHAR)
      baseDataTypeNode->type = BaseDataTypeNode::TYPE_CHAR;
    else if (auto t7 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree);
             t7->getSymbol()->getType() == SpiceParser::TYPE_STRING)
      baseDataTypeNode->type = BaseDataTypeNode::TYPE_STRING;
    else if (auto t8 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t8->getSymbol()->getType() == SpiceParser::TYPE_BOOL)
      baseDataTypeNode->type = BaseDataTypeNode::TYPE_BOOL;
    else if (auto t9 = dynamic_cast<antlr4::tree::TerminalNode *>(subTree); t9->getSymbol()->getType() == SpiceParser::TYPE_DYN)
      baseDataTypeNode->type = BaseDataTypeNode::TYPE_DYN;
    else
      assert(dynamic_cast<antlr4::tree::TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != baseDataTypeNode) {
      visit(rule);
      currentNode = baseDataTypeNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitCustomDataType(SpiceParser::CustomDataTypeContext *ctx) {
  auto customDataTypeNode = dynamic_cast<CustomDataTypeNode *>(currentNode);
  saveErrorMessage(customDataTypeNode, ctx);

  for (const auto &subTree : ctx->children) {
    antlr4::ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) // TypeLst
      currentNode = customDataTypeNode->createChild<TypeLstNode>(CodeLoc(rule->start, filePath));
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

std::any ASTBuilder::visitAssignOp(SpiceParser::AssignOpContext *ctx) {
  auto assignExprNode = dynamic_cast<AssignExprNode *>(currentNode);
  saveErrorMessage(assignExprNode, ctx);

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

std::any ASTBuilder::visitPrefixUnaryOp(SpiceParser::PrefixUnaryOpContext *ctx) {
  auto prefixUnaryExprNode = dynamic_cast<PrefixUnaryExprNode *>(currentNode);
  saveErrorMessage(prefixUnaryExprNode, ctx);

  // Extract assign operator
  if (ctx->MINUS())
    prefixUnaryExprNode->opQueue.emplace(PrefixUnaryExprNode::OP_MINUS, SymbolType(TY_INVALID));
  else if (ctx->PLUS_PLUS())
    prefixUnaryExprNode->opQueue.emplace(PrefixUnaryExprNode::OP_PLUS_PLUS, SymbolType(TY_INVALID));
  else if (ctx->MINUS_MINUS())
    prefixUnaryExprNode->opQueue.emplace(PrefixUnaryExprNode::OP_MINUS_MINUS, SymbolType(TY_INVALID));
  else if (ctx->NOT())
    prefixUnaryExprNode->opQueue.emplace(PrefixUnaryExprNode::OP_NOT, SymbolType(TY_INVALID));
  else if (ctx->BITWISE_NOT())
    prefixUnaryExprNode->opQueue.emplace(PrefixUnaryExprNode::OP_BITWISE_NOT, SymbolType(TY_INVALID));
  else if (ctx->MUL())
    prefixUnaryExprNode->opQueue.emplace(PrefixUnaryExprNode::OP_INDIRECTION, SymbolType(TY_INVALID));
  else if (ctx->BITWISE_AND())
    prefixUnaryExprNode->opQueue.emplace(PrefixUnaryExprNode::OP_ADDRESS_OF, SymbolType(TY_INVALID));
  else if (ctx->LOGICAL_AND()) {
    prefixUnaryExprNode->opQueue.emplace(PrefixUnaryExprNode::OP_ADDRESS_OF, SymbolType(TY_INVALID));
    prefixUnaryExprNode->opQueue.emplace(PrefixUnaryExprNode::OP_ADDRESS_OF, SymbolType(TY_INVALID));
  } else
    assert(false);

  return nullptr;
}

void ASTBuilder::replaceEscapeChars(std::string &string) {
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

int32_t ASTBuilder::parseInt(antlr4::tree::TerminalNode *terminal) {
  std::function<int32_t(const std::string &, int)> cb = [](const std::string &substr, int base) {
    return std::stoi(substr, nullptr, base);
  };
  return parseNumeric(terminal, cb);
}
int16_t ASTBuilder::parseShort(antlr4::tree::TerminalNode *terminal) {
  std::function<int16_t(const std::string &, int)> cb = [](const std::string &substr, int base) {
    return (int16_t)std::stoi(substr, nullptr, base);
  };
  return parseNumeric(terminal, cb);
}

int64_t ASTBuilder::parseLong(antlr4::tree::TerminalNode *terminal) {
  std::function<int64_t(const std::string &, int)> cb = [](const std::string &substr, int base) {
    return std::stoll(substr, nullptr, base);
  };
  return parseNumeric(terminal, cb);
}

int8_t ASTBuilder::parseChar(antlr4::tree::TerminalNode *terminal) {
  std::string input = terminal->toString();
  if (input.length() == 3) { // Normal char literals
    return input[1];
  } else if (input.length() == 4 && input[1] == '\\') { // Char literals with escape sequence
    switch (input[2]) {
    case '\'':
      return '\'';
    case '"':
      return '\"';
    case '\\':
      return '\\';
    case 'n':
      return '\n';
    case 'r':
      return '\r';
    case 't':
      return '\t';
    case 'b':
      return '\b';
    case 'f':
      return '\f';
    case 'v':
      return '\v';
    case '0':
      return '\0';
    default:
      CodeLoc codeLoc = CodeLoc(terminal->getSymbol(), filePath);
      throw ParserError(codeLoc, INVALID_CHAR_LITERAL, "Invalid escape sequence " + input);
    }
  } else {
    CodeLoc codeLoc = CodeLoc(terminal->getSymbol(), filePath);
    throw ParserError(codeLoc, INVALID_CHAR_LITERAL, "Invalid char literal " + input);
  }
}

std::string ASTBuilder::parseString(std::string input) {
  input = input.substr(1, input.size() - 2);
  replaceEscapeChars(input);
  return input;
}

template <typename T>
T ASTBuilder::parseNumeric(antlr4::tree::TerminalNode *terminal, std::function<T(const std::string &, int)> cb) {
  std::string input = terminal->toString();
  try {
    if (input.length() >= 3) {
      char c1 = input[0];
      char c2 = input[1];
      std::string substr = input.substr(2);
      if (c1 == '0') {
        switch (c2) {
        case 'd':
        case 'D':
          return cb(substr, 10);
        case 'b':
        case 'B':
          return cb(substr, 2);
        case 'h':
        case 'H':
        case 'x':
        case 'X':
          return cb(substr, 16);
        case 'o':
        case 'O':
          return cb(substr, 8);
        default:
          return cb(input, 10);
        }
      }
    }
    return cb(input, 10);
  } catch (std::out_of_range &e) {
    CodeLoc codeLoc = CodeLoc(terminal->getSymbol(), filePath);
    throw ParserError(codeLoc, NUMBER_OUT_OF_RANGE, "The provided number is out of range");
  } catch (std::invalid_argument &e) {
    CodeLoc codeLoc = CodeLoc(terminal->getSymbol(), filePath);
    throw ParserError(codeLoc, NUMBER_OUT_OF_RANGE, "You tried to parse '" + input + "' as an integer, but it was no integer");
  }
}

void ASTBuilder::saveErrorMessage(ASTNode *node, const antlr4::ParserRuleContext *ctx) {
  const antlr4::misc::Interval sourceInterval(ctx->start->getStartIndex(), ctx->stop->getStopIndex());
  antlr4::misc::Interval extendedSourceInterval(sourceInterval);

  if (inputStream->getText(extendedSourceInterval).find('\n') != std::string::npos) {
    extendedSourceInterval.b = extendedSourceInterval.a;
    while (inputStream->getText(extendedSourceInterval).find('\n') == std::string::npos)
      extendedSourceInterval.b++;
  }

  size_t indentation = 0;
  for (; indentation < ERROR_MESSAGE_CONTEXT; indentation++) {
    extendedSourceInterval.a--;
    if (extendedSourceInterval.a < 0 || inputStream->getText(extendedSourceInterval).find('\n') != std::string::npos) {
      extendedSourceInterval.a++;
      break;
    }
  }
  for (size_t suffixContext = 0; suffixContext < ERROR_MESSAGE_CONTEXT; suffixContext++) {
    extendedSourceInterval.b++;
    if (extendedSourceInterval.b > inputStream->size() ||
        inputStream->getText(extendedSourceInterval).find('\n') != std::string::npos) {
      extendedSourceInterval.b--;
      break;
    }
  }

  // Trim start
  while (inputStream->getText(extendedSourceInterval)[0] == ' ') {
    extendedSourceInterval.a++;
    indentation--;
  }

  // Trim end
  if (inputStream->getText(extendedSourceInterval)[extendedSourceInterval.length() - 1] == '\n')
    extendedSourceInterval.b--;

  std::string lineNumberStr = std::to_string(ctx->start->getLine());
  indentation += lineNumberStr.length() + 2;

  std::stringstream ss;
  ss << lineNumberStr << "  " << inputStream->getText(extendedSourceInterval) << "\n";
  for (size_t i = 0; i < indentation; i++)
    ss << " ";
  for (size_t i = 0; i < std::min(sourceInterval.length(), extendedSourceInterval.length()); i++)
    ss << "^";
  node->errorMessage = ss.str();
}

std::string ASTBuilder::getIdentifier(antlr4::tree::TerminalNode *terminal) {
  std::string identifier = terminal->getText();

  // Check if the identifier is 'String' and this is no std source file
  bool isReserved = identifier == STROBJ_NAME && !sourceFile->stdFile;
  // Check if the list of reserved keywords contains the given identifier
  isReserved |= std::find(std::begin(RESERVED_KEYWORDS), std::end(RESERVED_KEYWORDS), identifier) != std::end(RESERVED_KEYWORDS);
  // Print error message
  if (isReserved) {
    const CodeLoc codeLoc = CodeLoc(terminal->getSymbol(), filePath);
    throw ParserError(codeLoc, RESERVED_KEYWORD, "'" + identifier + "' is a reserved keyword. Please use another name instead.");
  }

  return identifier;
}