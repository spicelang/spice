// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "IRGenerator.h"

#include <ast/ASTNodes.h>

std::any IRGenerator::visitValue(const ValueNode *node) {
  diGenerator.setSourceLocation(node);

  // Constant
  if (node->constant()) {
    // Visit constant value
    auto constantValue = std::any_cast<llvm::Constant *>(visit(node->constant()));
    return ExprResult{.constant = constantValue};
  }

  // Function call
  if (node->functionCall())
    return visit(node->functionCall());

  // Array initialization
  if (node->arrayInitialization())
    return visit(node->arrayInitialization());

  // Struct instantiation
  if (node->structInstantiation())
    return visit(node->structInstantiation());

  if (node->isNil) {
    // Retrieve type of the nil constant
    auto nilType = any_cast<llvm::Type *>(visit(node->nilType()));
    // Create constant nil value
    llvm::Constant *nilValue = llvm::Constant::getNullValue(nilType);
    // Return it
    return ExprResult{.constant = nilValue};
  }

  throw std::runtime_error("Internal compiler error: Value fall-through"); // GCOV_EXCL_LINE
}

std::any IRGenerator::visitConstant(const ConstantNode *node) {
  const CompileTimeValue &compileTimeValue = node->getCompileTimeValue();

  // Value is a double constant
  if (node->type == ConstantNode::TYPE_DOUBLE) {
    llvm::Constant *value = llvm::ConstantFP::get(context, llvm::APFloat(compileTimeValue.doubleValue));
    return value;
  }

  // Value is an integer constant
  if (node->type == ConstantNode::TYPE_INT) {
    llvm::Type *intTy = builder.getInt32Ty();
    llvm::Constant *constInt = llvm::ConstantInt::get(intTy, compileTimeValue.intValue, node->isSigned);
    return constInt;
  }

  // Value is a short constant
  if (node->type == ConstantNode::TYPE_SHORT) {
    llvm::Type *shortTy = builder.getInt16Ty();
    llvm::Constant *constShort = llvm::ConstantInt::get(shortTy, compileTimeValue.shortValue, node->isSigned);
    return constShort;
  }

  // Value is a long constant
  if (node->type == ConstantNode::TYPE_LONG) {
    llvm::Type *longTy = builder.getInt64Ty();
    llvm::Constant *constLong = llvm::ConstantInt::get(longTy, compileTimeValue.longValue, node->isSigned);
    return constLong;
  }

  // Value is a char constant
  if (node->type == ConstantNode::TYPE_CHAR) {
    llvm::Type *charTy = builder.getInt8Ty();
    llvm::Constant *constChar = llvm::ConstantInt::get(charTy, compileTimeValue.charValue, node->isSigned);
    return constChar;
  }

  // Value is a string constant
  if (node->type == ConstantNode::TYPE_STRING) {
    std::string stringValue(compileTimeValue.stringValue);
    llvm::Constant *constString = builder.CreateGlobalStringPtr(stringValue, "", 0, module);
    return constString;
  }

  // Value is a boolean constant
  if (node->type == ConstantNode::TYPE_BOOL) {
    llvm::Constant *constBool = compileTimeValue.boolValue ? builder.getTrue() : builder.getFalse();
    return constBool;
  }

  throw std::runtime_error("Internal compiler error: Constant fall-through"); // GCOV_EXCL_LINE
}

std::any IRGenerator::visitFunctionCall(const FunctionCallNode *node) {
  return ParallelizableASTVisitor::visitFunctionCall(node);
}

std::any IRGenerator::visitArrayInitialization(const ArrayInitializationNode *node) {
  return ParallelizableASTVisitor::visitArrayInitialization(node);
}

std::any IRGenerator::visitStructInstantiation(const StructInstantiationNode *node) {
  return ParallelizableASTVisitor::visitStructInstantiation(node);
}

std::any IRGenerator::visitDataType(const DataTypeNode *node) {
  diGenerator.setSourceLocation(node);

  // Retrieve symbol type
  SymbolType symbolType = node->getEvaluatedSymbolType();
  assert(!symbolType.is(TY_DYN)); // Symbol type should not be dyn anymore at this point

  return symbolType.toLLVMType(context, currentScope);
}