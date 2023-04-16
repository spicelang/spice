// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "ASTBuilder.h"

#include <regex>

#include <SourceFile.h>
#include <ast/ASTNodes.h>
#include <exception/ParserError.h>
#include <typechecker/OpRuleManager.h>
#include <util/CommonUtil.h>

namespace spice::compiler {

ASTBuilder::ASTBuilder(GlobalResourceManager &resourceManager, SourceFile *sourceFile, ASTNode *rootNode,
                       antlr4::ANTLRInputStream *inputStream)
    : CompilerPass(resourceManager, sourceFile), currentNode(rootNode), filePath(sourceFile->filePath), inputStream(inputStream) {
}

std::any ASTBuilder::visitEntry(SpiceParser::EntryContext *ctx) {
  auto entryNode = static_cast<EntryNode *>(currentNode);
  entryNode->reserveChildren(ctx->children.size());

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    // Create child for the current node
    ParserRuleContext *rule;
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
    else if (rule = dynamic_cast<SpiceParser::AliasDefContext *>(subTree); rule != nullptr) // AliasDef
      currentNode = entryNode->createChild<AliasDefNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::GlobalVarDefContext *>(subTree); rule != nullptr) // GlobalVarDef
      currentNode = entryNode->createChild<GlobalVarDefNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::ImportStmtContext *>(subTree); rule != nullptr) // ImportStmt
      currentNode = entryNode->createChild<ImportStmtNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::ExtDeclContext *>(subTree); rule != nullptr) // ExtDecl
      currentNode = entryNode->createChild<ExtDeclNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != entryNode) {
      visit(rule);
      currentNode = entryNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitMainFunctionDef(SpiceParser::MainFunctionDefContext *ctx) {
  auto mainFctDefNode = static_cast<MainFctDefNode *>(currentNode);
  mainFctDefNode->reserveChildren(ctx->children.size());
  saveErrorMessage(mainFctDefNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::ParamLstContext *>(subTree); rule != nullptr) { // ArgLstDef
      currentNode = mainFctDefNode->createChild<ParamLstNode>(CodeLoc(rule->start, filePath));
      mainFctDefNode->takesArgs = true;
      isParam = true;
    } else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = mainFctDefNode->createChild<StmtLstNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != mainFctDefNode) {
      visit(rule);
      currentNode = mainFctDefNode;
      isParam = false;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitFunctionDef(SpiceParser::FunctionDefContext *ctx) {
  auto fctDefNode = static_cast<FctDefNode *>(currentNode);
  fctDefNode->reserveChildren(ctx->children.size());
  saveErrorMessage(fctDefNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierLstContext *>(subTree); rule != nullptr) // SpecifierLst
      currentNode = fctDefNode->createChild<SpecifierLstNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) { // DataType
      currentNode = fctDefNode->createChild<DataTypeNode>(CodeLoc(rule->start, filePath));
      isReturnType = true;
    } else if (rule = dynamic_cast<SpiceParser::FctNameContext *>(subTree); rule != nullptr) // FctName
      currentNode = fctDefNode->createChild<FctNameNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) { // TypeLst
      currentNode = fctDefNode->createChild<TypeLstNode>(CodeLoc(rule->start, filePath));
      fctDefNode->hasTemplateTypes = true;
    } else if (rule = dynamic_cast<SpiceParser::ParamLstContext *>(subTree); rule != nullptr) { // ParamLst
      currentNode = fctDefNode->createChild<ParamLstNode>(CodeLoc(rule->start, filePath));
      fctDefNode->hasParams = true;
      isParam = true;
    } else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = fctDefNode->createChild<StmtLstNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != fctDefNode) {
      visit(rule);
      currentNode = fctDefNode;
      isParam = false;
      isReturnType = false;
    }
  }

  // Retrieve information from the symbol name
  fctDefNode->fctName = fctDefNode->getChild<FctNameNode>();
  fctDefNode->isMethod = fctDefNode->fctName->nameFragments.size() > 1;

  return nullptr;
}

std::any ASTBuilder::visitProcedureDef(SpiceParser::ProcedureDefContext *ctx) {
  auto procDefNode = static_cast<ProcDefNode *>(currentNode);
  procDefNode->reserveChildren(ctx->children.size());
  saveErrorMessage(procDefNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierLstContext *>(subTree); rule != nullptr) // SpecifierLst
      currentNode = procDefNode->createChild<SpecifierLstNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::FctNameContext *>(subTree); rule != nullptr) { // FctName
      currentNode = procDefNode->createChild<FctNameNode>(CodeLoc(rule->start, filePath));
    } else if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) { // TypeLst
      currentNode = procDefNode->createChild<TypeLstNode>(CodeLoc(rule->start, filePath));
      procDefNode->hasTemplateTypes = true;
    } else if (rule = dynamic_cast<SpiceParser::ParamLstContext *>(subTree); rule != nullptr) { // ParamLst
      currentNode = procDefNode->createChild<ParamLstNode>(CodeLoc(rule->start, filePath));
      procDefNode->hasParams = true;
      isParam = true;
    } else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = procDefNode->createChild<StmtLstNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != procDefNode) {
      visit(rule);
      currentNode = procDefNode;
      isParam = false;
    }
  }

  // Retrieve information from the symbol name
  procDefNode->procName = procDefNode->getChild<FctNameNode>();
  procDefNode->isMethod = procDefNode->procName->nameFragments.size() > 1;

  return nullptr;
}

std::any ASTBuilder::visitFctName(SpiceParser::FctNameContext *ctx) {
  auto fctNameNode = static_cast<FctNameNode *>(currentNode);
  fctNameNode->reserveChildren(ctx->children.size());
  saveErrorMessage(fctNameNode, ctx);

  // Extract function name
  if (!ctx->IDENTIFIER().empty()) {
    fctNameNode->name = fctNameNode->fqName = getIdentifier(ctx->IDENTIFIER().back());
    if (ctx->IDENTIFIER().size() > 1) {
      fctNameNode->structName = getIdentifier(ctx->IDENTIFIER().front());
      fctNameNode->fqName = fctNameNode->structName + MEMBER_ACCESS_TOKEN + fctNameNode->name;
      fctNameNode->nameFragments.push_back(fctNameNode->structName);
    }
    fctNameNode->nameFragments.push_back(fctNameNode->name);
  }

  // Extract overloaded operator
  if (ctx->OPERATOR()) {
    // Visit overloadable operator
    currentNode = fctNameNode;
    visit(ctx->overloadableOp());
  }

  return nullptr;
}

std::any ASTBuilder::visitStructDef(SpiceParser::StructDefContext *ctx) {
  auto structDefNode = static_cast<StructDefNode *>(currentNode);
  structDefNode->reserveChildren(ctx->children.size());
  saveErrorMessage(structDefNode, ctx);

  // Extract struct name
  structDefNode->structName = getIdentifier(ctx->IDENTIFIER());

  bool seenStructKeyword = false;
  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierLstContext *>(subTree); rule != nullptr) // SpecifierLst
      currentNode = structDefNode->createChild<SpecifierLstNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) { // TypeLst
      if (!seenStructKeyword) {                                                              // Template type list
        currentNode = structDefNode->createChild<TypeLstNode>(CodeLoc(rule->start, filePath));
        structDefNode->isGeneric = true;
      } else { // Interface type list
        currentNode = structDefNode->createChild<TypeLstNode>(CodeLoc(rule->start, filePath));
        structDefNode->hasInterfaces = true;
      }
    } else if (rule = dynamic_cast<SpiceParser::FieldContext *>(subTree); rule != nullptr) { // Field
      currentNode = structDefNode->createChild<FieldNode>(CodeLoc(rule->start, filePath));
      isField = true;
    } else {
      auto token = dynamic_cast<TerminalNode *>(subTree);
      assert(token); // Fail if we did not get a terminal
      if (token->getSymbol()->getType() == SpiceParser::STRUCT)
        seenStructKeyword = true;
    }

    if (currentNode != structDefNode) {
      visit(rule);
      currentNode = structDefNode;
      isField = false;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitInterfaceDef(SpiceParser::InterfaceDefContext *ctx) {
  auto interfaceDefNode = static_cast<InterfaceDefNode *>(currentNode);
  interfaceDefNode->reserveChildren(ctx->children.size());
  saveErrorMessage(interfaceDefNode, ctx);

  // Extract interface name
  interfaceDefNode->interfaceName = getIdentifier(ctx->IDENTIFIER());

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierLstContext *>(subTree); rule != nullptr) // SpecifierLst
      currentNode = interfaceDefNode->createChild<SpecifierLstNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) { // TypeLst
      currentNode = interfaceDefNode->createChild<TypeLstNode>(CodeLoc(rule->start, filePath));
      interfaceDefNode->isGeneric = true;
    } else if (rule = dynamic_cast<SpiceParser::SignatureContext *>(subTree); rule != nullptr) { // Signature
      currentNode = interfaceDefNode->createChild<SignatureNode>(CodeLoc(rule->start, filePath));
    } else {
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal
    }

    if (currentNode != interfaceDefNode) {
      visit(rule);
      currentNode = interfaceDefNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitEnumDef(SpiceParser::EnumDefContext *ctx) {
  auto enumDefNode = static_cast<EnumDefNode *>(currentNode);
  enumDefNode->reserveChildren(ctx->children.size());
  saveErrorMessage(enumDefNode, ctx);

  // Extract enum name
  enumDefNode->enumName = getIdentifier(ctx->IDENTIFIER());

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierLstContext *>(subTree); rule != nullptr) // SpecifierLst
      currentNode = enumDefNode->createChild<SpecifierLstNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::EnumItemLstContext *>(subTree); rule != nullptr) { // EnumItemLst
      auto enumItemLst = enumDefNode->createChild<EnumItemLstNode>(CodeLoc(rule->start, filePath));
      enumItemLst->enumDef = enumDefNode;
      currentNode = enumItemLst;
    } else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != enumDefNode) {
      visit(rule);
      currentNode = enumDefNode;
    }
  }

  return nullptr;
}

std::any ASTBuilder::visitGenericTypeDef(SpiceParser::GenericTypeDefContext *ctx) {
  auto genericTypeDefNode = static_cast<GenericTypeDefNode *>(currentNode);
  genericTypeDefNode->reserveChildren(ctx->children.size());
  saveErrorMessage(genericTypeDefNode, ctx);

  // Extract type name
  genericTypeDefNode->typeName = getIdentifier(ctx->IDENTIFIER());

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::TypeAltsLstContext *>(subTree); rule != nullptr) // TypeAltsLst
      currentNode = genericTypeDefNode->createChild<TypeAltsLstNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != genericTypeDefNode) {
      visit(rule);
      currentNode = genericTypeDefNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitAliasDef(SpiceParser::AliasDefContext *ctx) {
  auto aliasDefNode = static_cast<AliasDefNode *>(currentNode);
  aliasDefNode->reserveChildren(ctx->children.size());
  saveErrorMessage(aliasDefNode, ctx);

  // Extract type name
  aliasDefNode->aliasName = getIdentifier(ctx->IDENTIFIER());

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) { // DataType
      currentNode = aliasDefNode->createChild<DataTypeNode>(CodeLoc(rule->start, filePath));
      aliasDefNode->dataTypeString = rule->getText();
    } else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != aliasDefNode) {
      visit(rule);
      currentNode = aliasDefNode;
    }
  }

  return nullptr;
}

std::any ASTBuilder::visitGlobalVarDef(SpiceParser::GlobalVarDefContext *ctx) {
  auto globalVarDefNode = static_cast<GlobalVarDefNode *>(currentNode);
  globalVarDefNode->reserveChildren(ctx->children.size());
  saveErrorMessage(globalVarDefNode, ctx);

  // Extract function name
  globalVarDefNode->varName = getIdentifier(ctx->IDENTIFIER());

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) { // DataType
      currentNode = globalVarDefNode->createChild<DataTypeNode>(CodeLoc(rule->start, filePath));
      isGlobal = true;
    } else if (rule = dynamic_cast<SpiceParser::ConstantContext *>(subTree); rule != nullptr) { // Constant
      currentNode = globalVarDefNode->createChild<ConstantNode>(CodeLoc(rule->start, filePath));
      globalVarDefNode->hasValue = true;
    } else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != globalVarDefNode) {
      visit(rule);
      currentNode = globalVarDefNode;
      isGlobal = false;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitExtDecl(SpiceParser::ExtDeclContext *ctx) {
  auto extDeclNode = static_cast<ExtDeclNode *>(currentNode);
  extDeclNode->reserveChildren(ctx->children.size());
  saveErrorMessage(extDeclNode, ctx);

  // Extract function name
  extDeclNode->extFunctionName = getIdentifier(ctx->IDENTIFIER());
  // Extract isDll
  extDeclNode->isDll = ctx->DLL() != nullptr;

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) // DataType
      currentNode = extDeclNode->createChild<DataTypeNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) { // TypeLst
      currentNode = extDeclNode->createChild<TypeLstNode>(CodeLoc(rule->start, filePath));
      extDeclNode->hasArgs = true;
    } else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != extDeclNode) {
      visit(rule);
      currentNode = extDeclNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitThreadDef(SpiceParser::ThreadDefContext *ctx) {
  auto threadDefNode = static_cast<ThreadDefNode *>(currentNode);
  threadDefNode->reserveChildren(ctx->children.size());
  saveErrorMessage(threadDefNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = threadDefNode->createChild<StmtLstNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != threadDefNode) {
      visit(rule);
      currentNode = threadDefNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitUnsafeBlockDef(SpiceParser::UnsafeBlockDefContext *ctx) {
  auto unsafeBlockDefNode = static_cast<UnsafeBlockDefNode *>(currentNode);
  unsafeBlockDefNode->reserveChildren(ctx->children.size());
  saveErrorMessage(unsafeBlockDefNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = unsafeBlockDefNode->createChild<StmtLstNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != unsafeBlockDefNode) {
      visit(rule);
      currentNode = unsafeBlockDefNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitForLoop(SpiceParser::ForLoopContext *ctx) {
  auto forLoopNode = static_cast<ForLoopNode *>(currentNode);
  forLoopNode->reserveChildren(ctx->children.size());
  saveErrorMessage(forLoopNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::ForHeadContext *>(subTree); rule != nullptr) // ForHead
      visit(rule);
    else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = forLoopNode->createChild<StmtLstNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != forLoopNode) {
      visit(rule);
      currentNode = forLoopNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitForHead(SpiceParser::ForHeadContext *ctx) {
  auto forLoopNode = static_cast<ForLoopNode *>(currentNode);
  forLoopNode->reserveChildren(ctx->children.size());
  saveErrorMessage(forLoopNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DeclStmtContext *>(subTree); rule != nullptr) // DeclStmt
      currentNode = forLoopNode->createChild<DeclStmtNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = forLoopNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != forLoopNode) {
      visit(rule);
      currentNode = forLoopNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitForeachLoop(SpiceParser::ForeachLoopContext *ctx) {
  auto foreachLoopNode = static_cast<ForeachLoopNode *>(currentNode);
  foreachLoopNode->reserveChildren(ctx->children.size());
  saveErrorMessage(foreachLoopNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::ForeachHeadContext *>(subTree); rule != nullptr) // ForeachHead
      visit(rule);
    else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = foreachLoopNode->createChild<StmtLstNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != foreachLoopNode) {
      visit(rule);
      currentNode = foreachLoopNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitForeachHead(SpiceParser::ForeachHeadContext *ctx) {
  auto foreachLoopNode = static_cast<ForeachLoopNode *>(currentNode);
  foreachLoopNode->reserveChildren(ctx->children.size());
  saveErrorMessage(foreachLoopNode, ctx);

  bool seenDeclStmt = false;

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DeclStmtContext *>(subTree); rule != nullptr) { // DeclStmt
      auto declStmtNode = foreachLoopNode->createChild<DeclStmtNode>(CodeLoc(rule->start, filePath));
      declStmtNode->isForEachItem = !ctx->COMMA() || seenDeclStmt;
      seenDeclStmt = true;
      currentNode = declStmtNode;
    } else if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = foreachLoopNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != foreachLoopNode) {
      visit(rule);
      currentNode = foreachLoopNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitWhileLoop(SpiceParser::WhileLoopContext *ctx) {
  auto whileLoopNode = static_cast<WhileLoopNode *>(currentNode);
  whileLoopNode->reserveChildren(ctx->children.size());
  saveErrorMessage(whileLoopNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = whileLoopNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = whileLoopNode->createChild<StmtLstNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != whileLoopNode) {
      visit(rule);
      currentNode = whileLoopNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitDoWhileLoop(SpiceParser::DoWhileLoopContext *ctx) {
  auto doWhileLoopNode = static_cast<DoWhileLoopNode *>(currentNode);
  doWhileLoopNode->reserveChildren(ctx->children.size());
  saveErrorMessage(doWhileLoopNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = doWhileLoopNode->createChild<StmtLstNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = doWhileLoopNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != doWhileLoopNode) {
      visit(rule);
      currentNode = doWhileLoopNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitIfStmt(SpiceParser::IfStmtContext *ctx) {
  auto ifStmtNode = static_cast<IfStmtNode *>(currentNode);
  ifStmtNode->reserveChildren(ctx->children.size());
  saveErrorMessage(ifStmtNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = ifStmtNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = ifStmtNode->createChild<StmtLstNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::ElseStmtContext *>(subTree); rule != nullptr) // ElseStmt
      currentNode = ifStmtNode->createChild<ElseStmtNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != ifStmtNode) {
      visit(rule);
      currentNode = ifStmtNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitElseStmt(SpiceParser::ElseStmtContext *ctx) {
  auto elseStmtNode = static_cast<ElseStmtNode *>(currentNode);
  elseStmtNode->reserveChildren(ctx->children.size());
  saveErrorMessage(elseStmtNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::IfStmtContext *>(subTree); rule != nullptr) { // IfStmt
      currentNode = elseStmtNode->createChild<IfStmtNode>(CodeLoc(rule->start, filePath));
      elseStmtNode->isElseIf = true;
    } else if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = elseStmtNode->createChild<StmtLstNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != elseStmtNode) {
      visit(rule);
      currentNode = elseStmtNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitAnonymousBlockStmt(SpiceParser::AnonymousBlockStmtContext *ctx) {
  auto blockStmtNode = static_cast<AnonymousBlockStmtNode *>(currentNode);
  blockStmtNode->reserveChildren(ctx->children.size());
  saveErrorMessage(blockStmtNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::StmtLstContext *>(subTree); rule != nullptr) // StmtLst
      currentNode = blockStmtNode->createChild<StmtLstNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != blockStmtNode) {
      visit(rule);
      currentNode = blockStmtNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitStmtLst(SpiceParser::StmtLstContext *ctx) {
  auto stmtLstNode = static_cast<StmtLstNode *>(currentNode);
  stmtLstNode->reserveChildren(ctx->children.size());
  saveErrorMessage(stmtLstNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::StmtContext *>(subTree); rule != nullptr) // Stmt
      currentNode = stmtLstNode->createChild<StmtNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::ForLoopContext *>(subTree); rule != nullptr) // ForLoop
      currentNode = stmtLstNode->createChild<ForLoopNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::ForeachLoopContext *>(subTree); rule != nullptr) // ForeachLoop
      currentNode = stmtLstNode->createChild<ForeachLoopNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::WhileLoopContext *>(subTree); rule != nullptr) // WhileLoop
      currentNode = stmtLstNode->createChild<WhileLoopNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::DoWhileLoopContext *>(subTree); rule != nullptr) // DoWhileLoop
      currentNode = stmtLstNode->createChild<DoWhileLoopNode>(CodeLoc(rule->start, filePath));
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
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

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
  auto typeLstNode = static_cast<TypeLstNode *>(currentNode);
  typeLstNode->reserveChildren(ctx->children.size());
  saveErrorMessage(typeLstNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) { // DataType
      currentNode = typeLstNode->createChild<DataTypeNode>(CodeLoc(rule->start, filePath));
      typeLstNode->numberOfTypes++;
    } else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != typeLstNode) {
      visit(rule);
      currentNode = typeLstNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitTypeAltsLst(SpiceParser::TypeAltsLstContext *ctx) {
  auto typeAltsLstNode = static_cast<TypeAltsLstNode *>(currentNode);
  typeAltsLstNode->reserveChildren(ctx->children.size());
  saveErrorMessage(typeAltsLstNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) { // DataType
      currentNode = typeAltsLstNode->createChild<DataTypeNode>(CodeLoc(rule->start, filePath));
      typeAltsLstNode->numberOfAlts++;
    } else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != typeAltsLstNode) {
      visit(rule);
      currentNode = typeAltsLstNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitParamLst(SpiceParser::ParamLstContext *ctx) {
  auto argLstDefNode = static_cast<ParamLstNode *>(currentNode);
  argLstDefNode->reserveChildren(ctx->children.size());
  saveErrorMessage(argLstDefNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DeclStmtContext *>(subTree); rule != nullptr) // DeclStmt
      currentNode = argLstDefNode->createChild<DeclStmtNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != argLstDefNode) {
      visit(rule);
      currentNode = argLstDefNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitArgLst(SpiceParser::ArgLstContext *ctx) {
  auto argLstNode = static_cast<ArgLstNode *>(currentNode);
  argLstNode->reserveChildren(ctx->children.size());
  saveErrorMessage(argLstNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignStmt
      currentNode = argLstNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != argLstNode) {
      visit(rule);
      currentNode = argLstNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitEnumItemLst(SpiceParser::EnumItemLstContext *ctx) {
  auto enumItemLstNode = static_cast<EnumItemLstNode *>(currentNode);
  enumItemLstNode->reserveChildren(ctx->children.size());
  saveErrorMessage(enumItemLstNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::EnumItemContext *>(subTree); rule != nullptr) { // EnumItem
      auto enumItem = enumItemLstNode->createChild<EnumItemNode>(CodeLoc(rule->start, filePath));
      enumItem->enumDef = enumItemLstNode->enumDef;
      currentNode = enumItem;
    } else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != enumItemLstNode) {
      visit(rule);
      currentNode = enumItemLstNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitEnumItem(SpiceParser::EnumItemContext *ctx) {
  auto enumItemNode = static_cast<EnumItemNode *>(currentNode);
  enumItemNode->reserveChildren(ctx->children.size());
  saveErrorMessage(enumItemNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    if (auto t1 = dynamic_cast<TerminalNode *>(subTree); t1->getSymbol()->getType() == SpiceParser::IDENTIFIER)
      enumItemNode->itemName = getIdentifier(t1);
    else if (auto t2 = dynamic_cast<TerminalNode *>(subTree); t2->getSymbol()->getType() == SpiceParser::INT_LIT) {
      enumItemNode->itemValue = parseInt(nullptr, t2);
      enumItemNode->hasValue = true;
    } else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal
  }
  return nullptr;
}

std::any ASTBuilder::visitField(SpiceParser::FieldContext *ctx) {
  auto fieldNode = static_cast<FieldNode *>(currentNode);
  fieldNode->reserveChildren(ctx->children.size());
  saveErrorMessage(fieldNode, ctx);

  // Extract field name
  fieldNode->fieldName = getIdentifier(ctx->IDENTIFIER());

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) // DataType
      currentNode = fieldNode->createChild<DataTypeNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != fieldNode) {
      visit(rule);
      currentNode = fieldNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitSignature(SpiceParser::SignatureContext *ctx) {
  auto signatureNode = static_cast<SignatureNode *>(currentNode);
  signatureNode->reserveChildren(ctx->children.size());
  saveErrorMessage(signatureNode, ctx);

  // Extract method name
  signatureNode->methodName = getIdentifier(ctx->IDENTIFIER());
  // Extract signature type
  signatureNode->signatureType = ctx->F() ? SignatureNode::TYPE_FUNCTION : SignatureNode::TYPE_PROCEDURE;
  signatureNode->signatureSpecifiers = ctx->F() ? TypeSpecifiers::of(TY_FUNCTION) : TypeSpecifiers::of(TY_PROCEDURE);

  bool seenLParen = false;
  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierLstContext *>(subTree); rule != nullptr) // SpecifierLst
      currentNode = signatureNode->createChild<SpecifierLstNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) // DataType
      currentNode = signatureNode->createChild<DataTypeNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) { // TypeLst
      if (seenLParen) {                                                                      // Param type list
        currentNode = signatureNode->createChild<TypeLstNode>(CodeLoc(rule->start, filePath));
        signatureNode->hasParams = true;
      } else { // Template type list
        currentNode = signatureNode->createChild<TypeLstNode>(CodeLoc(rule->start, filePath));
        signatureNode->hasTemplateTypes = true;
      }
    } else {
      auto token = dynamic_cast<TerminalNode *>(subTree);
      assert(token); // Fail if we did not get a terminal
      if (token->getSymbol()->getType() == SpiceParser::LPAREN)
        seenLParen = true;
    }

    if (currentNode != signatureNode) {
      visit(rule);
      currentNode = signatureNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitStmt(SpiceParser::StmtContext *ctx) {
  auto stmtNode = static_cast<StmtNode *>(currentNode);
  stmtNode->reserveChildren(ctx->children.size());
  saveErrorMessage(stmtNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
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
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != stmtNode) {
      visit(rule);
      currentNode = stmtNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitDeclStmt(SpiceParser::DeclStmtContext *ctx) {
  auto declStmtNode = static_cast<DeclStmtNode *>(currentNode);
  declStmtNode->reserveChildren(ctx->children.size());
  saveErrorMessage(declStmtNode, ctx);

  // Extract var name
  declStmtNode->varName = getIdentifier(ctx->IDENTIFIER());
  declStmtNode->isParam = isParam;

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) // DataType
      currentNode = declStmtNode->createChild<DataTypeNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) { // AssignExpr
      currentNode = declStmtNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
      declStmtNode->hasAssignment = true;
    } else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != declStmtNode) {
      visit(rule);
      currentNode = declStmtNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitSpecifierLst(SpiceParser::SpecifierLstContext *ctx) {
  auto specifierLstNode = static_cast<SpecifierLstNode *>(currentNode);
  specifierLstNode->reserveChildren(ctx->children.size());
  saveErrorMessage(specifierLstNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierContext *>(subTree); rule != nullptr) // DeclSpecifier
      currentNode = specifierLstNode->createChild<SpecifierNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != specifierLstNode) {
      visit(rule);
      currentNode = specifierLstNode;
    }
  }

  // Check if we have both, signed and unsigned specifier
  bool seenSignedOrUnsigned = false;
  for (const SpecifierNode *specifier : specifierLstNode->specifiers()) {
    if (specifier->type != SpecifierNode::TY_SIGNED && specifier->type != SpecifierNode::TY_UNSIGNED)
      continue;
    if (seenSignedOrUnsigned)
      throw ParserError(specifier->codeLoc, INVALID_SPECIFIER_COMBINATION, "A variable can not be signed and unsigned");
    seenSignedOrUnsigned = true;
  }

  return nullptr;
}

std::any ASTBuilder::visitSpecifier(SpiceParser::SpecifierContext *ctx) {
  auto specifierNode = static_cast<SpecifierNode *>(currentNode);
  specifierNode->reserveChildren(ctx->children.size());
  saveErrorMessage(specifierNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    auto token = dynamic_cast<TerminalNode *>(subTree);
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
  auto importStmtNode = static_cast<ImportStmtNode *>(currentNode);
  importStmtNode->reserveChildren(ctx->children.size());
  saveErrorMessage(importStmtNode, ctx);

  // Extract path
  const std::string pathStr = ctx->STRING_LIT()->getText();
  importStmtNode->importPath = pathStr.substr(1, pathStr.size() - 2);

  // If no name is given, use the path as name
  importStmtNode->importName = ctx->AS() ? getIdentifier(ctx->IDENTIFIER()) : importStmtNode->importPath;

  return nullptr;
}

std::any ASTBuilder::visitReturnStmt(SpiceParser::ReturnStmtContext *ctx) {
  auto returnStmtNode = static_cast<ReturnStmtNode *>(currentNode);
  returnStmtNode->reserveChildren(ctx->children.size());
  saveErrorMessage(returnStmtNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) { // AssignExpr
      currentNode = returnStmtNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
      returnStmtNode->hasReturnValue = true;
    } else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != returnStmtNode) {
      visit(rule);
      currentNode = returnStmtNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitBreakStmt(SpiceParser::BreakStmtContext *ctx) {
  auto breakStmtNode = static_cast<BreakStmtNode *>(currentNode);
  breakStmtNode->reserveChildren(ctx->children.size());
  saveErrorMessage(breakStmtNode, ctx);

  // Extract number of breaks
  if (ctx->INT_LIT())
    breakStmtNode->breakTimes = std::stoi(ctx->INT_LIT()->toString());

  return nullptr;
}

std::any ASTBuilder::visitContinueStmt(SpiceParser::ContinueStmtContext *ctx) {
  auto continueStmtNode = static_cast<ContinueStmtNode *>(currentNode);
  continueStmtNode->reserveChildren(ctx->children.size());
  saveErrorMessage(continueStmtNode, ctx);

  // Extract number of continues
  if (ctx->INT_LIT())
    continueStmtNode->continueTimes = std::stoi(ctx->INT_LIT()->toString());

  return nullptr;
}

std::any ASTBuilder::visitAssertStmt(SpiceParser::AssertStmtContext *ctx) {
  auto assertStmtNode = static_cast<AssertStmtNode *>(currentNode);
  assertStmtNode->reserveChildren(ctx->children.size());
  saveErrorMessage(assertStmtNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) { // AssignExpr
      currentNode = assertStmtNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
      antlr4::misc::Interval interval(rule->start->getStartIndex(), rule->stop->getStopIndex());
      assertStmtNode->expressionString = inputStream->getText(interval);
    } else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != assertStmtNode) {
      visit(rule);
      currentNode = assertStmtNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitBuiltinCall(SpiceParser::BuiltinCallContext *ctx) {
  auto atomicExprNode = static_cast<AtomicExprNode *>(currentNode);
  atomicExprNode->reserveChildren(ctx->children.size());
  saveErrorMessage(atomicExprNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
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
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != atomicExprNode) {
      visit(rule);
      currentNode = atomicExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitPrintfCall(SpiceParser::PrintfCallContext *ctx) {
  auto printfCallNode = static_cast<PrintfCallNode *>(currentNode);
  printfCallNode->reserveChildren(ctx->children.size());
  saveErrorMessage(printfCallNode, ctx);

  // Extract templated string
  std::string templatedString = ctx->STRING_LIT()->getText();
  templatedString = templatedString.substr(1, templatedString.size() - 2);
  replaceEscapeChars(templatedString);
  printfCallNode->templatedString = templatedString;

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = printfCallNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != printfCallNode) {
      visit(rule);
      currentNode = printfCallNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitSizeOfCall(SpiceParser::SizeOfCallContext *ctx) {
  auto sizeofCallNode = static_cast<SizeofCallNode *>(currentNode);
  sizeofCallNode->reserveChildren(ctx->children.size());
  saveErrorMessage(sizeofCallNode, ctx);

  // Check if type or value
  sizeofCallNode->isType = ctx->TYPE();

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = sizeofCallNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) // DataType
      currentNode = sizeofCallNode->createChild<DataTypeNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != sizeofCallNode) {
      visit(rule);
      currentNode = sizeofCallNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitLenCall(SpiceParser::LenCallContext *ctx) {
  auto lenCallNode = static_cast<LenCallNode *>(currentNode);
  lenCallNode->reserveChildren(ctx->children.size());
  saveErrorMessage(lenCallNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = lenCallNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != lenCallNode) {
      visit(rule);
      currentNode = lenCallNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitTidCall(SpiceParser::TidCallContext *ctx) {
  auto tidCallNode = static_cast<TidCallNode *>(currentNode);
  tidCallNode->reserveChildren(ctx->children.size());
  saveErrorMessage(tidCallNode, ctx);
  return nullptr;
}

std::any ASTBuilder::visitJoinCall(SpiceParser::JoinCallContext *ctx) {
  auto joinCallNode = static_cast<JoinCallNode *>(currentNode);
  joinCallNode->reserveChildren(ctx->children.size());
  saveErrorMessage(joinCallNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = joinCallNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != joinCallNode) {
      visit(rule);
      currentNode = joinCallNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitAssignExpr(SpiceParser::AssignExprContext *ctx) {
  auto assignExprNode = static_cast<AssignExprNode *>(currentNode);
  assignExprNode->reserveChildren(ctx->children.size());
  saveErrorMessage(assignExprNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
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
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != assignExprNode) {
      visit(rule);
      currentNode = assignExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitTernaryExpr(SpiceParser::TernaryExprContext *ctx) {
  auto ternaryExprNode = static_cast<TernaryExprNode *>(currentNode);
  ternaryExprNode->reserveChildren(ctx->children.size());
  saveErrorMessage(ternaryExprNode, ctx);

  // Check if is shortened
  ternaryExprNode->isShortened = ctx->logicalOrExpr().size() == 2;

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::LogicalOrExprContext *>(subTree); rule != nullptr) // LogicalOrExpr
      currentNode = ternaryExprNode->createChild<LogicalOrExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != ternaryExprNode) {
      visit(rule);
      currentNode = ternaryExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitLogicalOrExpr(SpiceParser::LogicalOrExprContext *ctx) {
  auto logicalOrExprNode = static_cast<LogicalOrExprNode *>(currentNode);
  logicalOrExprNode->reserveChildren(ctx->children.size());
  saveErrorMessage(logicalOrExprNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::LogicalAndExprContext *>(subTree); rule != nullptr) // LogicalAndExpr
      currentNode = logicalOrExprNode->createChild<LogicalAndExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != logicalOrExprNode) {
      visit(rule);
      currentNode = logicalOrExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitLogicalAndExpr(SpiceParser::LogicalAndExprContext *ctx) {
  auto logicalAndExprNode = static_cast<LogicalAndExprNode *>(currentNode);
  logicalAndExprNode->reserveChildren(ctx->children.size());
  saveErrorMessage(logicalAndExprNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::BitwiseOrExprContext *>(subTree); rule != nullptr) // BitwiseOr
      currentNode = logicalAndExprNode->createChild<BitwiseOrExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != logicalAndExprNode) {
      visit(rule);
      currentNode = logicalAndExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitBitwiseOrExpr(SpiceParser::BitwiseOrExprContext *ctx) {
  auto bitwiseOrExprNode = static_cast<BitwiseOrExprNode *>(currentNode);
  bitwiseOrExprNode->reserveChildren(ctx->children.size());
  saveErrorMessage(bitwiseOrExprNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::BitwiseXorExprContext *>(subTree); rule != nullptr) // BitwiseXor
      currentNode = bitwiseOrExprNode->createChild<BitwiseXorExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != bitwiseOrExprNode) {
      visit(rule);
      currentNode = bitwiseOrExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitBitwiseXorExpr(SpiceParser::BitwiseXorExprContext *ctx) {
  auto bitwiseXorExprNode = static_cast<BitwiseXorExprNode *>(currentNode);
  bitwiseXorExprNode->reserveChildren(ctx->children.size());
  saveErrorMessage(bitwiseXorExprNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::BitwiseAndExprContext *>(subTree); rule != nullptr) // BitwiseAnd
      currentNode = bitwiseXorExprNode->createChild<BitwiseAndExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != bitwiseXorExprNode) {
      visit(rule);
      currentNode = bitwiseXorExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitBitwiseAndExpr(SpiceParser::BitwiseAndExprContext *ctx) {
  auto bitwiseAndExprNode = static_cast<BitwiseAndExprNode *>(currentNode);
  bitwiseAndExprNode->reserveChildren(ctx->children.size());
  saveErrorMessage(bitwiseAndExprNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::EqualityExprContext *>(subTree); rule != nullptr) // EqualityExpr
      currentNode = bitwiseAndExprNode->createChild<EqualityExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != bitwiseAndExprNode) {
      visit(rule);
      currentNode = bitwiseAndExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitEqualityExpr(SpiceParser::EqualityExprContext *ctx) {
  auto equalityExprNode = static_cast<EqualityExprNode *>(currentNode);
  equalityExprNode->reserveChildren(ctx->children.size());
  saveErrorMessage(equalityExprNode, ctx);

  // Extract operator
  if (ctx->EQUAL())
    equalityExprNode->op = EqualityExprNode::OP_EQUAL;
  else if (ctx->NOT_EQUAL())
    equalityExprNode->op = EqualityExprNode::OP_NOT_EQUAL;

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::RelationalExprContext *>(subTree); rule != nullptr) // RelationalExpr
      currentNode = equalityExprNode->createChild<RelationalExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != equalityExprNode) {
      visit(rule);
      currentNode = equalityExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitRelationalExpr(SpiceParser::RelationalExprContext *ctx) {
  auto relationalExprNode = static_cast<RelationalExprNode *>(currentNode);
  relationalExprNode->reserveChildren(ctx->children.size());
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

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::ShiftExprContext *>(subTree); rule != nullptr) // ShiftExpr
      currentNode = relationalExprNode->createChild<ShiftExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != relationalExprNode) {
      visit(rule);
      currentNode = relationalExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitShiftExpr(SpiceParser::ShiftExprContext *ctx) {
  auto shiftExprNode = static_cast<ShiftExprNode *>(currentNode);
  shiftExprNode->reserveChildren(ctx->children.size());
  saveErrorMessage(shiftExprNode, ctx);

  // Extract operator
  if (!ctx->LESS().empty())
    shiftExprNode->op = ShiftExprNode::OP_SHIFT_LEFT;
  else if (!ctx->GREATER().empty())
    shiftExprNode->op = ShiftExprNode::OP_SHIFT_RIGHT;

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AdditiveExprContext *>(subTree); rule != nullptr) // AdditiveExpr
      currentNode = shiftExprNode->createChild<AdditiveExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != shiftExprNode) {
      visit(rule);
      currentNode = shiftExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitAdditiveExpr(SpiceParser::AdditiveExprContext *ctx) {
  auto additiveExprNode = static_cast<AdditiveExprNode *>(currentNode);
  additiveExprNode->reserveChildren(ctx->children.size());
  saveErrorMessage(additiveExprNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::MultiplicativeExprContext *>(subTree); rule != nullptr) // MultiplicativeExpr
      currentNode = additiveExprNode->createChild<MultiplicativeExprNode>(CodeLoc(rule->start, filePath));
    else if (auto t1 = dynamic_cast<TerminalNode *>(subTree); t1->getSymbol()->getType() == SpiceParser::PLUS)
      additiveExprNode->opQueue.emplace(AdditiveExprNode::OP_PLUS, SymbolType(TY_INVALID));
    else if (auto t2 = dynamic_cast<TerminalNode *>(subTree); t2->getSymbol()->getType() == SpiceParser::MINUS)
      additiveExprNode->opQueue.emplace(AdditiveExprNode::OP_MINUS, SymbolType(TY_INVALID));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != additiveExprNode) {
      visit(rule);
      currentNode = additiveExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitMultiplicativeExpr(SpiceParser::MultiplicativeExprContext *ctx) {
  auto multiplicativeExprNode = static_cast<MultiplicativeExprNode *>(currentNode);
  multiplicativeExprNode->reserveChildren(ctx->children.size());
  saveErrorMessage(multiplicativeExprNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::CastExprContext *>(subTree); rule != nullptr) // CastExpr
      currentNode = multiplicativeExprNode->createChild<CastExprNode>(CodeLoc(rule->start, filePath));
    else if (auto t1 = dynamic_cast<TerminalNode *>(subTree); t1->getSymbol()->getType() == SpiceParser::MUL)
      multiplicativeExprNode->opQueue.emplace(MultiplicativeExprNode::OP_MUL, SymbolType(TY_INVALID));
    else if (auto t2 = dynamic_cast<TerminalNode *>(subTree); t2->getSymbol()->getType() == SpiceParser::DIV)
      multiplicativeExprNode->opQueue.emplace(MultiplicativeExprNode::OP_DIV, SymbolType(TY_INVALID));
    else if (auto t3 = dynamic_cast<TerminalNode *>(subTree); t3->getSymbol()->getType() == SpiceParser::REM)
      multiplicativeExprNode->opQueue.emplace(MultiplicativeExprNode::OP_REM, SymbolType(TY_INVALID));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != multiplicativeExprNode) {
      visit(rule);
      currentNode = multiplicativeExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitCastExpr(SpiceParser::CastExprContext *ctx) {
  auto castExprNode = static_cast<CastExprNode *>(currentNode);
  castExprNode->reserveChildren(ctx->children.size());
  saveErrorMessage(castExprNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::PrefixUnaryExprContext *>(subTree); rule != nullptr) // PrefixUnaryExpr
      currentNode = castExprNode->createChild<PrefixUnaryExprNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::DataTypeContext *>(subTree); rule != nullptr) { // DataType
      currentNode = castExprNode->createChild<DataTypeNode>(CodeLoc(rule->start, filePath));
      castExprNode->isCasted = true;
    } else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != castExprNode) {
      visit(rule);
      currentNode = castExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitPrefixUnaryExpr(SpiceParser::PrefixUnaryExprContext *ctx) {
  auto prefixUnaryExprNode = static_cast<PrefixUnaryExprNode *>(currentNode);
  prefixUnaryExprNode->reserveChildren(ctx->children.size());
  saveErrorMessage(prefixUnaryExprNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::PrefixUnaryExprContext *>(subTree); rule != nullptr) //
      currentNode = prefixUnaryExprNode->createChild<PrefixUnaryExprNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::PrefixUnaryOpContext *>(subTree); rule != nullptr) // PrefixUnaryOp
      prefixUnaryExprNode = std::any_cast<PrefixUnaryExprNode *>(visit(rule));
    else if (rule = dynamic_cast<SpiceParser::PostfixUnaryExprContext *>(subTree); rule != nullptr) // PostfixUnaryExpr
      currentNode = prefixUnaryExprNode->createChild<PostfixUnaryExprNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != prefixUnaryExprNode) {
      visit(rule);
      currentNode = prefixUnaryExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitPostfixUnaryExpr(SpiceParser::PostfixUnaryExprContext *ctx) {
  auto postfixUnaryExprNode = static_cast<PostfixUnaryExprNode *>(currentNode);
  postfixUnaryExprNode->reserveChildren(ctx->children.size());
  saveErrorMessage(postfixUnaryExprNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::AtomicExprContext *>(subTree); rule != nullptr) // AtomicExpr
      currentNode = postfixUnaryExprNode->createChild<AtomicExprNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = postfixUnaryExprNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::PostfixUnaryExprContext *>(subTree); rule != nullptr) // PostfixUnaryExpr
      currentNode = postfixUnaryExprNode->createChild<PostfixUnaryExprNode>(CodeLoc(rule->start, filePath));
    else if (auto t1 = dynamic_cast<TerminalNode *>(subTree); t1->getSymbol()->getType() == SpiceParser::LBRACKET)
      postfixUnaryExprNode->op = PostfixUnaryExprNode::OP_SUBSCRIPT;
    else if (auto t2 = dynamic_cast<TerminalNode *>(subTree); t2->getSymbol()->getType() == SpiceParser::DOT) {
      postfixUnaryExprNode->op = PostfixUnaryExprNode::OP_MEMBER_ACCESS;
    } else if (auto t = dynamic_cast<TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::IDENTIFIER)
      postfixUnaryExprNode->identifier = getIdentifier(t);
    else if (auto t3 = dynamic_cast<TerminalNode *>(subTree); t3->getSymbol()->getType() == SpiceParser::PLUS_PLUS)
      postfixUnaryExprNode->op = PostfixUnaryExprNode::OP_PLUS_PLUS;
    else if (auto t4 = dynamic_cast<TerminalNode *>(subTree); t4->getSymbol()->getType() == SpiceParser::MINUS_MINUS)
      postfixUnaryExprNode->op = PostfixUnaryExprNode::OP_MINUS_MINUS;
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != postfixUnaryExprNode) {
      visit(rule);
      currentNode = postfixUnaryExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitAtomicExpr(SpiceParser::AtomicExprContext *ctx) {
  auto atomicExprNode = static_cast<AtomicExprNode *>(currentNode);
  atomicExprNode->reserveChildren(ctx->children.size());
  saveErrorMessage(atomicExprNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::ConstantContext *>(subTree); rule != nullptr) // Constant
      currentNode = atomicExprNode->createChild<ConstantNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::ValueContext *>(subTree); rule != nullptr) // Value
      currentNode = atomicExprNode->createChild<ValueNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::BuiltinCallContext *>(subTree); rule != nullptr) // BuiltinCall
      visit(rule);
    else if (rule = dynamic_cast<SpiceParser::AssignExprContext *>(subTree); rule != nullptr) // AssignExpr
      currentNode = atomicExprNode->createChild<AssignExprNode>(CodeLoc(rule->start, filePath));
    else if (auto t = dynamic_cast<TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
      atomicExprNode->identifierFragments.push_back(getIdentifier(t));
      if (!atomicExprNode->fqIdentifier.empty())
        atomicExprNode->fqIdentifier += SCOPE_ACCESS_TOKEN;
      atomicExprNode->fqIdentifier += atomicExprNode->identifierFragments.back();
    } else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != atomicExprNode) {
      visit(rule);
      currentNode = atomicExprNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitValue(SpiceParser::ValueContext *ctx) {
  auto valueNode = static_cast<ValueNode *>(currentNode);
  valueNode->reserveChildren(ctx->children.size());
  saveErrorMessage(valueNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
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
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != valueNode) {
      visit(rule);
      currentNode = valueNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitConstant(SpiceParser::ConstantContext *ctx) {
  auto constantNode = static_cast<ConstantNode *>(currentNode);
  constantNode->reserveChildren(ctx->children.size());
  saveErrorMessage(constantNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (auto t = dynamic_cast<TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::DOUBLE_LIT) {
      constantNode->type = ConstantNode::TYPE_DOUBLE;
      constantNode->compileTimeValue.doubleValue = std::stod(t->toString());
      constantNode->hasDirectCompileTimeValue = true;
    } else if (auto t1 = dynamic_cast<TerminalNode *>(subTree); t1->getSymbol()->getType() == SpiceParser::INT_LIT) {
      constantNode->type = ConstantNode::TYPE_INT;
      constantNode->compileTimeValue.intValue = parseInt(constantNode, t1);
      constantNode->hasDirectCompileTimeValue = true;
    } else if (auto t2 = dynamic_cast<TerminalNode *>(subTree); t2->getSymbol()->getType() == SpiceParser::SHORT_LIT) {
      constantNode->type = ConstantNode::TYPE_SHORT;
      constantNode->compileTimeValue.shortValue = parseShort(constantNode, t2);
      constantNode->hasDirectCompileTimeValue = true;
    } else if (auto t3 = dynamic_cast<TerminalNode *>(subTree); t3->getSymbol()->getType() == SpiceParser::LONG_LIT) {
      constantNode->type = ConstantNode::TYPE_LONG;
      constantNode->compileTimeValue.longValue = parseLong(constantNode, t3);
      constantNode->hasDirectCompileTimeValue = true;
    } else if (auto t4 = dynamic_cast<TerminalNode *>(subTree); t4->getSymbol()->getType() == SpiceParser::CHAR_LIT) {
      constantNode->type = ConstantNode::TYPE_CHAR;
      constantNode->compileTimeValue.charValue = parseChar(ctx->CHAR_LIT());
      constantNode->hasDirectCompileTimeValue = true;
    } else if (auto t5 = dynamic_cast<TerminalNode *>(subTree); t5->getSymbol()->getType() == SpiceParser::STRING_LIT) {
      // Transfer ownership to the AST node
      constantNode->compileTimeStringValue = parseString(ctx->STRING_LIT()->toString());
      // Save a pointer to the string in the compile time value
      constantNode->type = ConstantNode::TYPE_STRING;
      constantNode->compileTimeValue.stringValue = constantNode->compileTimeStringValue.c_str();
      constantNode->hasDirectCompileTimeValue = true;
    } else if (auto t6 = dynamic_cast<TerminalNode *>(subTree); t6->getSymbol()->getType() == SpiceParser::TRUE) {
      constantNode->type = ConstantNode::TYPE_BOOL;
      constantNode->compileTimeValue.boolValue = true;
      constantNode->hasDirectCompileTimeValue = true;
    } else if (auto t7 = dynamic_cast<TerminalNode *>(subTree); t7->getSymbol()->getType() == SpiceParser::FALSE) {
      constantNode->type = ConstantNode::TYPE_BOOL;
      constantNode->compileTimeValue.boolValue = false;
      constantNode->hasDirectCompileTimeValue = true;
    } else {
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal
    }

    if (currentNode != constantNode) {
      visit(rule);
      currentNode = constantNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitFunctionCall(SpiceParser::FunctionCallContext *ctx) {
  auto fctCallNode = static_cast<FunctionCallNode *>(currentNode);
  fctCallNode->reserveChildren(ctx->children.size());
  saveErrorMessage(fctCallNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::ArgLstContext *>(subTree); rule != nullptr) { // ArgLst
      currentNode = fctCallNode->createChild<ArgLstNode>(CodeLoc(rule->start, filePath));
      fctCallNode->hasArgs = true;
    } else if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) { // TypeLst
      currentNode = fctCallNode->createChild<TypeLstNode>(CodeLoc(rule->start, filePath));
      fctCallNode->hasTemplateTypes = true;
    } else if (auto t1 = dynamic_cast<TerminalNode *>(subTree); t1->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
      fctCallNode->functionNameFragments.push_back(t1->toString());
      fctCallNode->fqFunctionName += fctCallNode->functionNameFragments.back();
    } else if (auto t2 = dynamic_cast<TerminalNode *>(subTree); t2->getSymbol()->getType() == SpiceParser::DOT)
      fctCallNode->fqFunctionName += MEMBER_ACCESS_TOKEN;
    else if (auto t3 = dynamic_cast<TerminalNode *>(subTree); t3->getSymbol()->getType() == SpiceParser::SCOPE_ACCESS)
      fctCallNode->fqFunctionName += SCOPE_ACCESS_TOKEN;
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != fctCallNode) {
      visit(rule);
      currentNode = fctCallNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitArrayInitialization(SpiceParser::ArrayInitializationContext *ctx) {
  auto arrayInitializationNode = static_cast<ArrayInitializationNode *>(currentNode);
  arrayInitializationNode->reserveChildren(ctx->children.size());
  saveErrorMessage(arrayInitializationNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::ArgLstContext *>(subTree); rule != nullptr) // ArgLst
      currentNode = arrayInitializationNode->createChild<ArgLstNode>(CodeLoc(rule->start, filePath));
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != arrayInitializationNode) {
      visit(rule);
      currentNode = arrayInitializationNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitStructInstantiation(SpiceParser::StructInstantiationContext *ctx) {
  auto structInstantiationNode = static_cast<StructInstantiationNode *>(currentNode);
  structInstantiationNode->reserveChildren(ctx->children.size());
  saveErrorMessage(structInstantiationNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) // TypeLst
      currentNode = structInstantiationNode->createChild<TypeLstNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::ArgLstContext *>(subTree); rule != nullptr) // ArgLst
      currentNode = structInstantiationNode->createChild<ArgLstNode>(CodeLoc(rule->start, filePath));
    else if (auto t = dynamic_cast<TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
      std::string fragment = t->toString();
      structInstantiationNode->structNameFragments.push_back(fragment);
      if (!structInstantiationNode->fqStructName.empty())
        structInstantiationNode->fqStructName += SCOPE_ACCESS_TOKEN;
      structInstantiationNode->fqStructName += fragment;
    } else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != structInstantiationNode) {
      visit(rule);
      currentNode = structInstantiationNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitDataType(SpiceParser::DataTypeContext *ctx) {
  auto dataTypeNode = static_cast<DataTypeNode *>(currentNode);
  dataTypeNode->reserveChildren(ctx->children.size());
  saveErrorMessage(dataTypeNode, ctx);

  dataTypeNode->isParamType = isParam;
  dataTypeNode->isGlobalType = isGlobal;
  dataTypeNode->isFieldType = isField;
  dataTypeNode->isReturnType = isReturnType;

  for (int i = 0; i < ctx->children.size(); i++) {
    auto subTree = ctx->children[i];
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::SpecifierLstContext *>(subTree); rule != nullptr) // SpecifierLst
      currentNode = dataTypeNode->createChild<SpecifierLstNode>(CodeLoc(rule->start, filePath));
    else if (rule = dynamic_cast<SpiceParser::BaseDataTypeContext *>(subTree); rule != nullptr) // BaseDataType
      currentNode = dataTypeNode->createChild<BaseDataTypeNode>(CodeLoc(rule->start, filePath));
    else if (auto t1 = dynamic_cast<TerminalNode *>(subTree); t1->getSymbol()->getType() == SpiceParser::MUL)
      dataTypeNode->tmQueue.push({DataTypeNode::TYPE_PTR, false, 0});
    else if (auto t2 = dynamic_cast<TerminalNode *>(subTree); t1->getSymbol()->getType() == SpiceParser::BITWISE_AND)
      dataTypeNode->tmQueue.push({DataTypeNode::TYPE_REF, false, 0});
    else if (auto t3 = dynamic_cast<TerminalNode *>(subTree); t2->getSymbol()->getType() == SpiceParser::LBRACKET) {
      i++; // Consume LBRACKET
      subTree = ctx->children[i];
      bool hasSize = false;
      unsigned int hardCodedSize = 0;
      std::string sizeVarName;
      if (auto t4 = dynamic_cast<TerminalNode *>(subTree); t4->getSymbol()->getType() == SpiceParser::INT_LIT) {
        hasSize = true;
        hardCodedSize = std::stoi(t4->getSymbol()->getText());
        i++; // Consume INTEGER
      } else if (auto t5 = dynamic_cast<TerminalNode *>(subTree); t5->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
        hasSize = true;
        sizeVarName = getIdentifier(t5);
        i++; // Consume IDENTIFIER
      }
      dataTypeNode->tmQueue.push({DataTypeNode::TYPE_ARRAY, hasSize, hardCodedSize, sizeVarName});
    } else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != dataTypeNode) {
      visit(rule);
      currentNode = dataTypeNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitBaseDataType(SpiceParser::BaseDataTypeContext *ctx) {
  auto baseDataTypeNode = static_cast<BaseDataTypeNode *>(currentNode);
  baseDataTypeNode->reserveChildren(ctx->children.size());
  saveErrorMessage(baseDataTypeNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::CustomDataTypeContext *>(subTree); rule != nullptr) { // CustomDataType
      baseDataTypeNode->type = BaseDataTypeNode::TY_CUSTOM;
      currentNode = baseDataTypeNode->createChild<CustomDataTypeNode>(CodeLoc(rule->start, filePath));
    } else if (auto t1 = dynamic_cast<TerminalNode *>(subTree); t1->getSymbol()->getType() == SpiceParser::TYPE_DOUBLE)
      baseDataTypeNode->type = BaseDataTypeNode::TYPE_DOUBLE;
    else if (auto t2 = dynamic_cast<TerminalNode *>(subTree); t2->getSymbol()->getType() == SpiceParser::TYPE_INT)
      baseDataTypeNode->type = BaseDataTypeNode::TYPE_INT;
    else if (auto t3 = dynamic_cast<TerminalNode *>(subTree); t3->getSymbol()->getType() == SpiceParser::TYPE_SHORT)
      baseDataTypeNode->type = BaseDataTypeNode::TYPE_SHORT;
    else if (auto t4 = dynamic_cast<TerminalNode *>(subTree); t4->getSymbol()->getType() == SpiceParser::TYPE_LONG)
      baseDataTypeNode->type = BaseDataTypeNode::TYPE_LONG;
    else if (auto t5 = dynamic_cast<TerminalNode *>(subTree); t5->getSymbol()->getType() == SpiceParser::TYPE_BYTE)
      baseDataTypeNode->type = BaseDataTypeNode::TYPE_BYTE;
    else if (auto t6 = dynamic_cast<TerminalNode *>(subTree); t6->getSymbol()->getType() == SpiceParser::TYPE_CHAR)
      baseDataTypeNode->type = BaseDataTypeNode::TYPE_CHAR;
    else if (auto t7 = dynamic_cast<TerminalNode *>(subTree); t7->getSymbol()->getType() == SpiceParser::TYPE_STRING)
      baseDataTypeNode->type = BaseDataTypeNode::TYPE_STRING;
    else if (auto t8 = dynamic_cast<TerminalNode *>(subTree); t8->getSymbol()->getType() == SpiceParser::TYPE_BOOL)
      baseDataTypeNode->type = BaseDataTypeNode::TYPE_BOOL;
    else if (auto t9 = dynamic_cast<TerminalNode *>(subTree); t9->getSymbol()->getType() == SpiceParser::TYPE_DYN)
      baseDataTypeNode->type = BaseDataTypeNode::TYPE_DYN;
    else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != baseDataTypeNode) {
      visit(rule);
      currentNode = baseDataTypeNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitCustomDataType(SpiceParser::CustomDataTypeContext *ctx) {
  auto customDataTypeNode = static_cast<CustomDataTypeNode *>(currentNode);
  customDataTypeNode->reserveChildren(ctx->children.size());
  saveErrorMessage(customDataTypeNode, ctx);

  for (ParserRuleContext::ParseTree *subTree : ctx->children) {
    ParserRuleContext *rule;
    if (rule = dynamic_cast<SpiceParser::TypeLstContext *>(subTree); rule != nullptr) // TypeLst
      currentNode = customDataTypeNode->createChild<TypeLstNode>(CodeLoc(rule->start, filePath));
    else if (auto t = dynamic_cast<TerminalNode *>(subTree); t->getSymbol()->getType() == SpiceParser::IDENTIFIER) {
      std::string fragment = t->toString();
      customDataTypeNode->typeNameFragments.push_back(fragment);
      if (!customDataTypeNode->fqTypeName.empty())
        customDataTypeNode->fqTypeName += SCOPE_ACCESS_TOKEN;
      customDataTypeNode->fqTypeName += fragment;
    } else
      assert(dynamic_cast<TerminalNode *>(subTree)); // Fail if we did not get a terminal

    if (currentNode != customDataTypeNode) {
      visit(rule);
      currentNode = customDataTypeNode;
    }
  }
  return nullptr;
}

std::any ASTBuilder::visitAssignOp(SpiceParser::AssignOpContext *ctx) {
  auto assignExprNode = static_cast<AssignExprNode *>(currentNode);
  assignExprNode->reserveChildren(ctx->children.size());
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
  auto prefixUnaryExprNode = static_cast<PrefixUnaryExprNode *>(currentNode);
  prefixUnaryExprNode->reserveChildren(ctx->children.size());
  saveErrorMessage(prefixUnaryExprNode, ctx);

  // Extract assign operator
  if (ctx->MINUS())
    prefixUnaryExprNode->op = PrefixUnaryExprNode::OP_MINUS;
  else if (ctx->PLUS_PLUS())
    prefixUnaryExprNode->op = PrefixUnaryExprNode::OP_PLUS_PLUS;
  else if (ctx->MINUS_MINUS())
    prefixUnaryExprNode->op = PrefixUnaryExprNode::OP_MINUS_MINUS;
  else if (ctx->NOT())
    prefixUnaryExprNode->op = PrefixUnaryExprNode::OP_NOT;
  else if (ctx->BITWISE_NOT())
    prefixUnaryExprNode->op = PrefixUnaryExprNode::OP_BITWISE_NOT;
  else if (ctx->MUL())
    prefixUnaryExprNode->op = PrefixUnaryExprNode::OP_INDIRECTION;
  else if (ctx->BITWISE_AND())
    prefixUnaryExprNode->op = PrefixUnaryExprNode::OP_ADDRESS_OF;
  else if (ctx->LOGICAL_AND()) {
    // Here, the logical and (&&) stands for two bitwise and (&) to represent two consecutive address-of operators
    // What we do here, is to bring it to the form like two bitwise and would be parsed, so that we need no special
    // handling for '&&' in the other compile stages
    prefixUnaryExprNode->op = PrefixUnaryExprNode::OP_ADDRESS_OF;
    prefixUnaryExprNode = prefixUnaryExprNode->createChild<PrefixUnaryExprNode>(CodeLoc(ctx->start, filePath));
    prefixUnaryExprNode->op = PrefixUnaryExprNode::OP_ADDRESS_OF;
  } else
    assert(false);

  return prefixUnaryExprNode;
}

std::any ASTBuilder::visitOverloadableOp(SpiceParser::OverloadableOpContext *ctx) {
  auto fctNameNode = static_cast<FctNameNode *>(currentNode);
  fctNameNode->reserveChildren(ctx->children.size());
  saveErrorMessage(fctNameNode, ctx);

  if (ctx->PLUS()) {
    fctNameNode->overloadedOperator = FctNameNode::OP_PLUS;
    fctNameNode->name = OP_FCT_PLUS;
  } else if (ctx->MINUS()) {
    fctNameNode->overloadedOperator = FctNameNode::OP_MINUS;
    fctNameNode->name = OP_FCT_MINUS;
  } else if (ctx->MUL()) {
    fctNameNode->overloadedOperator = FctNameNode::OP_MUL;
    fctNameNode->name = OP_FCT_MUL;
  } else if (ctx->DIV()) {
    fctNameNode->overloadedOperator = FctNameNode::OP_DIV;
    fctNameNode->name = OP_FCT_DIV;
  } else if (ctx->EQUAL()) {
    fctNameNode->overloadedOperator = FctNameNode::OP_EQUAL;
    fctNameNode->name = OP_FCT_EQUAL;
  } else if (ctx->NOT_EQUAL()) {
    fctNameNode->overloadedOperator = FctNameNode::OP_NOT_EQUAL;
    fctNameNode->name = OP_FCT_NOT_EQUAL;
  } else if (ctx->PLUS_EQUAL()) {
    fctNameNode->overloadedOperator = FctNameNode::OP_NOT_EQUAL;
    fctNameNode->name = OP_FCT_PLUS_EQUAL;
  } else if (ctx->MINUS_EQUAL()) {
    fctNameNode->overloadedOperator = FctNameNode::OP_NOT_EQUAL;
    fctNameNode->name = OP_FCT_MINUS_EQUAL;
  } else if (ctx->MUL_EQUAL()) {
    fctNameNode->overloadedOperator = FctNameNode::OP_NOT_EQUAL;
    fctNameNode->name = OP_FCT_MUL_EQUAL;
  } else if (ctx->DIV_EQUAL()) {
    fctNameNode->overloadedOperator = FctNameNode::OP_NOT_EQUAL;
    fctNameNode->name = OP_FCT_DIV_EQUAL;
  } else if (ctx->PLUS_PLUS()) {
    fctNameNode->overloadedOperator = FctNameNode::OP_PLUS_PLUS;
    fctNameNode->name = OP_FCT_POSTFIX_PLUS_PLUS;
  } else if (ctx->MINUS_MINUS()) {
    fctNameNode->overloadedOperator = FctNameNode::OP_MINUS_MINUS;
    fctNameNode->name = OP_FCT_POSTFIX_MINUS_MINUS;
  } else {
    assert(false);
  }
  fctNameNode->fqName = fctNameNode->name;
  fctNameNode->nameFragments.push_back(fctNameNode->name);

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

int32_t ASTBuilder::parseInt(ConstantNode *constantNode, TerminalNode *terminal) {
  std::function<int32_t(const std::string &, int)> cb = [](const std::string &substr, int base) {
    return std::stoi(substr, nullptr, base);
  };
  return parseNumeric(constantNode, terminal, cb);
}
int16_t ASTBuilder::parseShort(ConstantNode *constantNode, TerminalNode *terminal) {
  std::function<int16_t(const std::string &, int)> cb = [](const std::string &substr, int base) {
    return (int16_t)std::stoi(substr, nullptr, base);
  };
  return parseNumeric(constantNode, terminal, cb);
}

int64_t ASTBuilder::parseLong(ConstantNode *constantNode, TerminalNode *terminal) {
  std::function<int64_t(const std::string &, int)> cb = [](const std::string &substr, int base) {
    return std::stoll(substr, nullptr, base);
  };
  return parseNumeric(constantNode, terminal, cb);
}

int8_t ASTBuilder::parseChar(TerminalNode *terminal) {
  const std::string input = terminal->toString();
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
      const CodeLoc codeLoc(terminal->getSymbol(), filePath);
      throw ParserError(codeLoc, INVALID_CHAR_LITERAL, "Invalid escape sequence " + input);
    }
  } else {
    const CodeLoc codeLoc(terminal->getSymbol(), filePath);
    throw ParserError(codeLoc, INVALID_CHAR_LITERAL, "Invalid char literal " + input);
  }
}

std::string ASTBuilder::parseString(std::string input) {
  input = input.substr(1, input.size() - 2);
  replaceEscapeChars(input);
  return input;
}

template <typename T>
T ASTBuilder::parseNumeric(ConstantNode *constantNode, TerminalNode *terminal, std::function<T(const std::string &, int)> cb) {
  const std::string input = terminal->toString();

  // Set to signed if the input string ends with 'u'
  if (constantNode)
    constantNode->isSigned = input.ends_with('u');

  try {
    if (input.length() >= 3) {
      const char c1 = input[0];
      const char c2 = input[1];
      const std::string subStr = input.substr(2);
      if (c1 == '0') {
        switch (c2) {
        case 'd':
        case 'D':
          return cb(subStr, 10);
        case 'b':
        case 'B':
          return cb(subStr, 2);
        case 'h':
        case 'H':
        case 'x':
        case 'X':
          return cb(subStr, 16);
        case 'o':
        case 'O':
          return cb(subStr, 8);
        default:
          return cb(input, 10);
        }
      }
    }
    return cb(input, 10);
  } catch (std::out_of_range &e) {
    const CodeLoc codeLoc(terminal->getSymbol(), filePath);
    throw ParserError(codeLoc, NUMBER_OUT_OF_RANGE, "The provided number is out of range");
  } catch (std::invalid_argument &e) {
    const CodeLoc codeLoc(terminal->getSymbol(), filePath);
    throw ParserError(codeLoc, NUMBER_OUT_OF_RANGE, "You tried to parse '" + input + "' as an integer, but it was no integer");
  }
}

void ASTBuilder::saveErrorMessage(ASTNode *node, const ParserRuleContext *ctx) {
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

  const std::string lineNumberStr = std::to_string(ctx->start->getLine());
  indentation += lineNumberStr.length() + 2;

  std::stringstream ss;
  ss << lineNumberStr << "  " << inputStream->getText(extendedSourceInterval) << "\n";
  for (size_t i = 0; i < indentation; i++)
    ss << " ";
  for (size_t i = 0; i < std::min(sourceInterval.length(), extendedSourceInterval.length()); i++)
    ss << "^";
  node->errorMessage = ss.str();
}

std::string ASTBuilder::getIdentifier(TerminalNode *terminal) {
  std::string identifier = terminal->getText();

  // Check if the identifier is 'String' and this is no std source file
  bool isReserved = identifier == STROBJ_NAME && !sourceFile->stdFile;
  // Check if the list of reserved keywords contains the given identifier
  isReserved |= std::find(std::begin(RESERVED_KEYWORDS), std::end(RESERVED_KEYWORDS), identifier) != std::end(RESERVED_KEYWORDS);
  // Print error message
  if (isReserved) {
    const CodeLoc codeLoc(terminal->getSymbol(), filePath);
    throw ParserError(codeLoc, RESERVED_KEYWORD, "'" + identifier + "' is a reserved keyword. Please use another name instead");
  }

  return identifier;
}

} // namespace spice::compiler