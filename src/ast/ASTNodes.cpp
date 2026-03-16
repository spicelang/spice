// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include <ast/ASTNodes.h>

#include <SourceFile.h>
#include <ast/Attributes.h>
#include <exception/SemanticError.h>
#include <symboltablebuilder/SymbolTableBuilder.h>
#include <typechecker/Builtins.h>

namespace spice::compiler {

// Constant definitions
static constexpr size_t ERROR_MESSAGE_CONTEXT = 20;

std::string ASTNode::getErrorMessage() const {
  antlr4::CharStream *inputStream = codeLoc.sourceFile->antlrCtx.inputStream.get();
  const antlr4::misc::Interval &sourceInterval = codeLoc.sourceInterval;
  antlr4::misc::Interval extSourceInterval(sourceInterval);

  // If we have a multi-line interval, only use the first line
  if (const size_t offset = inputStream->getText(extSourceInterval).find('\n'); offset != std::string::npos)
    extSourceInterval.b = extSourceInterval.a + static_cast<ssize_t>(offset);

  size_t markerIndentation = 0;
  for (; markerIndentation < ERROR_MESSAGE_CONTEXT; markerIndentation++) {
    extSourceInterval.a--;
    if (extSourceInterval.a < 0 || inputStream->getText(extSourceInterval).find('\n') != std::string::npos) {
      extSourceInterval.a++;
      break;
    }
  }
  for (size_t suffixContext = 0; suffixContext < ERROR_MESSAGE_CONTEXT; suffixContext++) {
    extSourceInterval.b++;
    if (static_cast<size_t>(extSourceInterval.b) > inputStream->size() ||
        inputStream->getText(extSourceInterval).find('\n') != std::string::npos) {
      extSourceInterval.b--;
      break;
    }
  }

  // Trim start
  while (inputStream->getText(extSourceInterval)[0] == ' ') {
    extSourceInterval.a++;
    markerIndentation--;
  }

  // Trim end
  if (inputStream->getText(extSourceInterval)[extSourceInterval.length() - 1] == '\n')
    extSourceInterval.b--;

  const std::string lineNumberStr = std::to_string(codeLoc.line);
  markerIndentation += lineNumberStr.length() + 2;

  // Build error message
  std::stringstream ss;
  ss << lineNumberStr << "  " << inputStream->getText(extSourceInterval) << "\n";
  ss << std::string(markerIndentation, ' ');
  ss << std::string(std::min(sourceInterval.length(), extSourceInterval.length()), '^');
  return ss.str();
}

const StmtLstNode *ASTNode::getNextOuterStmtLst() const { // NOLINT(*-no-recursion)
  assert(parent != nullptr);
  return isStmtLst() ? spice_pointer_cast<const StmtLstNode *>(this) : parent->getNextOuterStmtLst();
}

bool MainFctDefNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable, size_t manIdx) const {
  return body->returnsOnAllControlPaths(doSetPredecessorsUnreachable, manIdx);
}

bool FctDefBaseNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable, size_t manIdx) const {
  return body->returnsOnAllControlPaths(doSetPredecessorsUnreachable, manIdx);
}

CompileTimeValue GlobalVarDefNode::getCompileTimeValue(size_t manIdx) const { return constant->getCompileTimeValue(manIdx); }

bool ForLoopNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable, size_t manIdx) const {
  // If we have the guarantee that the loop condition is always true and the loop body returns on all control paths,
  // we can assume that the loop itself will always return
  const bool condAlwaysTrue = condAssign->hasCompileTimeValue(manIdx) && condAssign->getCompileTimeValue(manIdx).boolValue;
  return condAlwaysTrue && body->returnsOnAllControlPaths(doSetPredecessorsUnreachable, manIdx);
}

bool WhileLoopNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable, size_t manIdx) const {
  // If we have the guarantee that the loop condition is always true and the loop body returns on all control paths,
  // we can assume that the loop itself will always return
  const bool condAlwaysTrue = condition->hasCompileTimeValue(manIdx) && condition->getCompileTimeValue(manIdx).boolValue;
  return condAlwaysTrue && body->returnsOnAllControlPaths(doSetPredecessorsUnreachable, manIdx);
}

bool DoWhileLoopNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable, size_t manIdx) const {
  // Do-while loops will always be executed at least once. So if the body returns on all control paths, the loop will as well
  return body->returnsOnAllControlPaths(doSetPredecessorsUnreachable, manIdx);
}

bool IfStmtNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable, size_t manIdx) const { // NOLINT(misc-no-recursion)
  // If the condition always evaluates to 'true' the then block must return
  if (!compileElseBranch)
    return thenBody->returnsOnAllControlPaths(doSetPredecessorsUnreachable, manIdx);

  // If the condition always evaluates to 'false' the else block must return
  if (!compileThenBranch)
    return elseStmt != nullptr && elseStmt->returnsOnAllControlPaths(doSetPredecessorsUnreachable, manIdx);

  // If the condition does not always evaluate to 'true' or 'false' we need to check both branches
  return thenBody->returnsOnAllControlPaths(doSetPredecessorsUnreachable, manIdx) && elseStmt != nullptr &&
         elseStmt->returnsOnAllControlPaths(doSetPredecessorsUnreachable, manIdx);
}

bool ElseStmtNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable,
                                            size_t manIdx) const { // NOLINT(misc-no-recursion)
  if (isElseIf)
    return ifStmt->returnsOnAllControlPaths(doSetPredecessorsUnreachable, manIdx);
  return body->returnsOnAllControlPaths(doSetPredecessorsUnreachable, manIdx);
}

bool SwitchStmtNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable, size_t manIdx) const {
  const auto pred = [=](const CaseBranchNode *node) {
    return node->returnsOnAllControlPaths(doSetPredecessorsUnreachable, manIdx);
  };
  const bool allCaseBranchesReturn = std::ranges::all_of(caseBranches, pred);
  const bool defaultBranchReturns =
      !defaultBranch || defaultBranch->returnsOnAllControlPaths(doSetPredecessorsUnreachable, manIdx);
  return allCaseBranchesReturn && defaultBranchReturns;
}

bool CaseBranchNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable, size_t manIdx) const {
  return body->returnsOnAllControlPaths(doSetPredecessorsUnreachable, manIdx);
}

bool DefaultBranchNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable, size_t manIdx) const {
  return body->returnsOnAllControlPaths(doSetPredecessorsUnreachable, manIdx);
}

bool StmtLstNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable, size_t manIdx) const {
  // An empty statement list does not return at all
  if (statements.empty())
    return false;
  // A statement list returns on all control paths, if the one direct child statement returns on all control paths
  bool returnsOnAllControlPaths = false;
  for (StmtNode *child : statements) {
    assert(child != nullptr);

    // Prevent marking instructions as unreachable if doSetPredecessorsUnreachable is set to false
    if (returnsOnAllControlPaths && *doSetPredecessorsUnreachable)
      child->unreachable = true;

    if (child->returnsOnAllControlPaths(doSetPredecessorsUnreachable, manIdx))
      returnsOnAllControlPaths = true;
  }
  return returnsOnAllControlPaths;
}

std::vector<const CompileTimeValue *> AttrLstNode::getAttrValuesByName(const std::string &key) const {
  assert(ATTR_CONFIGS.contains(key));

  std::vector<const CompileTimeValue *> attributeValues;
  for (const AttrNode *attrNode : attributes) {
    // Skip attributes with different keys
    if (attrNode->key != key)
      continue;

    // Found a matching attribute
    const CompileTimeValue *value = attrNode->getValue();
    if (!value) {
      // If the attribute has no value, we use the default value
      attributeValues.push_back(&DEFAULT_BOOL_COMPILE_VALUE);
    } else {
      // If the attribute has a value, we use the value
      attributeValues.push_back(value);
    }
  }

  return attributeValues;
}

const CompileTimeValue *AttrLstNode::getAttrValueByName(const std::string &key) const {
  const std::vector<const CompileTimeValue *> attrs = getAttrValuesByName(key);
  return attrs.empty() ? nullptr : attrs.back();
}

bool AttrLstNode::hasAttr(const std::string &key) const {
  return std::ranges::any_of(attributes, [&](const AttrNode *attr) { return attr->key == key; });
}

const CompileTimeValue *AttrNode::getValue() const { return value ? &value->compileTimeValue : nullptr; }

bool AssertStmtNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable, size_t manIdx) const {
  // If the expression, passed to the assert statement is always evaluated to false, the assert statement will never succeed
  return assignExpr->hasCompileTimeValue(manIdx) && !assignExpr->getCompileTimeValue(manIdx).boolValue;
}

bool AssignExprNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable, size_t manIdx) const {
  // If it's a ternary, do the default thing
  if (op == AssignOp::OP_NONE)
    return ternaryExpr->returnsOnAllControlPaths(doSetPredecessorsUnreachable, manIdx);

  // If it's a modification on the result variable, we technically return from the function, but at the end of the function.
  const AtomicExprNode *atomicExpr = lhs->postfixUnaryExpr ? lhs->postfixUnaryExpr->atomicExpr : nullptr;
  if (atomicExpr && atomicExpr->fqIdentifier == RETURN_VARIABLE_NAME) {
    // If we assign the result variable, we technically return from the function, but at the end of the function.
    // Therefore, the following code is not unreachable, but will be executed in any case.
    *doSetPredecessorsUnreachable = false;
    return true;
  }

  return false;
}

bool TernaryExprNode::hasCompileTimeValue(size_t manIdx) const {
  const bool trueExprHasCompileTimeValue = !trueExpr || trueExpr->hasCompileTimeValue(manIdx);
  const bool falseExprHasCompileTimeValue = !falseExpr || falseExpr->hasCompileTimeValue(manIdx);
  return condition->hasCompileTimeValue(manIdx) && trueExprHasCompileTimeValue && falseExprHasCompileTimeValue;
}

CompileTimeValue TernaryExprNode::getCompileTimeValue(size_t manIdx) const {
  assert(condition != nullptr);
  if (!trueExpr && !falseExpr)
    return condition->getCompileTimeValue(manIdx);

  // If the condition has no compile time value, we do not need to evaluate the true and false values
  if (!condition->hasCompileTimeValue(manIdx))
    return {};

  // Check if the condition always evaluates to 'true'
  if (condition->getCompileTimeValue(manIdx).boolValue) {
    const ExprNode *trueValue = isShortened ? condition : trueExpr;
    assert(trueValue != nullptr);
    return trueValue->getCompileTimeValue(manIdx);
  }

  assert(falseExpr != nullptr);
  return falseExpr->getCompileTimeValue(manIdx);
}

bool LogicalOrExprNode::hasCompileTimeValue(size_t manIdx) const {
  return std::ranges::all_of(operands, [=](const ExprNode *node) { return node->hasCompileTimeValue(manIdx); });
}

CompileTimeValue LogicalOrExprNode::getCompileTimeValue(size_t manIdx) const {
  if (operands.size() == 1)
    return operands.front()->getCompileTimeValue(manIdx);

  // Check if one expression evaluates to 'true'
  for (const ExprNode *op : operands) {
    assert(op->hasCompileTimeValue(manIdx));
    // If one operand evaluates to 'true' the whole expression is 'true'
    if (const CompileTimeValue opCompileTimeValue = op->getCompileTimeValue(manIdx); opCompileTimeValue.boolValue)
      return CompileTimeValue{.boolValue = true};
  }

  // Return 'false'
  return CompileTimeValue{.boolValue = false};
}

bool LogicalAndExprNode::hasCompileTimeValue(size_t manIdx) const {
  return std::ranges::all_of(operands, [=](const BitwiseOrExprNode *node) { return node->hasCompileTimeValue(manIdx); });
}

CompileTimeValue LogicalAndExprNode::getCompileTimeValue(size_t manIdx) const {
  if (operands.size() == 1)
    return operands.front()->getCompileTimeValue(manIdx);

  // Check if all expressions evaluate to 'true'
  for (const BitwiseOrExprNode *op : operands) {
    assert(op->hasCompileTimeValue(manIdx));
    // If one operand evaluates to 'false' the whole expression is 'false'
    if (const CompileTimeValue opCompileTimeValue = op->getCompileTimeValue(manIdx); !opCompileTimeValue.boolValue)
      return CompileTimeValue{.boolValue = false};
  }

  // Return 'false'
  return CompileTimeValue{.boolValue = false};
}

bool BitwiseOrExprNode::hasCompileTimeValue(size_t manIdx) const {
  return std::ranges::all_of(operands, [=](const BitwiseXorExprNode *node) { return node->hasCompileTimeValue(manIdx); });
}

CompileTimeValue BitwiseOrExprNode::getCompileTimeValue(size_t manIdx) const {
  if (operands.size() == 1)
    return operands.front()->getCompileTimeValue(manIdx);

  CompileTimeValue result = operands.front()->getCompileTimeValue(manIdx);
  for (size_t i = 1; i < operands.size(); i++) {
    assert(operands.at(i)->hasCompileTimeValue(manIdx));
    const CompileTimeValue opCompileTimeValue = operands.at(i)->getCompileTimeValue(manIdx);
    result.longValue |= opCompileTimeValue.longValue;
  }

  return result;
}

bool BitwiseXorExprNode::hasCompileTimeValue(size_t manIdx) const {
  return std::ranges::all_of(operands, [=](const BitwiseAndExprNode *node) { return node->hasCompileTimeValue(manIdx); });
}

CompileTimeValue BitwiseXorExprNode::getCompileTimeValue(size_t manIdx) const {
  if (operands.size() == 1)
    return operands.front()->getCompileTimeValue(manIdx);

  CompileTimeValue result = operands.front()->getCompileTimeValue(manIdx);
  for (size_t i = 1; i < operands.size(); i++) {
    assert(operands.at(i)->hasCompileTimeValue(manIdx));
    const CompileTimeValue opCompileTimeValue = operands.at(i)->getCompileTimeValue(manIdx);
    result.longValue ^= opCompileTimeValue.longValue;
  }

  return result;
}

bool BitwiseAndExprNode::hasCompileTimeValue(size_t manIdx) const {
  return std::ranges::all_of(operands, [=](const EqualityExprNode *node) { return node->hasCompileTimeValue(manIdx); });
}

CompileTimeValue BitwiseAndExprNode::getCompileTimeValue(size_t manIdx) const {
  if (operands.size() == 1)
    return operands.front()->getCompileTimeValue(manIdx);

  CompileTimeValue result = operands.front()->getCompileTimeValue(manIdx);
  for (size_t i = 1; i < operands.size(); i++) {
    assert(operands.at(i)->hasCompileTimeValue(manIdx));
    const CompileTimeValue opCompileTimeValue = operands.at(i)->getCompileTimeValue(manIdx);
    result.longValue &= opCompileTimeValue.longValue;
  }

  return result;
}

bool EqualityExprNode::hasCompileTimeValue(size_t manIdx) const {
  return std::ranges::all_of(operands, [=](const RelationalExprNode *node) { return node->hasCompileTimeValue(manIdx); });
}

CompileTimeValue EqualityExprNode::getCompileTimeValue(size_t manIdx) const {
  if (operands.size() == 1)
    return operands.front()->getCompileTimeValue(manIdx);

  const CompileTimeValue op0Value = operands.at(0)->getCompileTimeValue(manIdx);
  const CompileTimeValue op1Value = operands.at(1)->getCompileTimeValue(manIdx);
  if (op == EqualityOp::OP_EQUAL)
    return CompileTimeValue{.boolValue = op0Value.longValue == op1Value.longValue};
  if (op == EqualityOp::OP_NOT_EQUAL)
    return CompileTimeValue{.boolValue = op0Value.longValue != op1Value.longValue};

  throw CompilerError(UNHANDLED_BRANCH, "EqualityExprNode::getCompileTimeValue()");
}

bool RelationalExprNode::hasCompileTimeValue(size_t manIdx) const {
  return std::ranges::all_of(operands, [=](const ShiftExprNode *node) { return node->hasCompileTimeValue(manIdx); });
}

CompileTimeValue RelationalExprNode::getCompileTimeValue(size_t manIdx) const {
  if (operands.size() == 1)
    return operands.front()->getCompileTimeValue(manIdx);

  const CompileTimeValue op0Value = operands.at(0)->getCompileTimeValue(manIdx);
  const CompileTimeValue op1Value = operands.at(1)->getCompileTimeValue(manIdx);
  if (op == RelationalOp::OP_LESS)
    return CompileTimeValue{.boolValue = op0Value.longValue < op1Value.longValue};
  if (op == RelationalOp::OP_GREATER)
    return CompileTimeValue{.boolValue = op0Value.longValue > op1Value.longValue};
  if (op == RelationalOp::OP_LESS_EQUAL)
    return CompileTimeValue{.boolValue = op0Value.longValue <= op1Value.longValue};
  if (op == RelationalOp::OP_GREATER_EQUAL)
    return CompileTimeValue{.boolValue = op0Value.longValue >= op1Value.longValue};

  throw CompilerError(UNHANDLED_BRANCH, "RelationalExprNode::getCompileTimeValue()");
}

bool ShiftExprNode::hasCompileTimeValue(size_t manIdx) const {
  return std::ranges::all_of(operands, [=](const AdditiveExprNode *node) { return node->hasCompileTimeValue(manIdx); });
}

CompileTimeValue ShiftExprNode::getCompileTimeValue(size_t manIdx) const {
  if (operands.size() == 1)
    return operands.front()->getCompileTimeValue(manIdx);

  CompileTimeValue result = operands.front()->getCompileTimeValue(manIdx);
  OpQueue opQueueCopy = opQueue;
  for (size_t i = 1; i < operands.size(); i++) {
    assert(operands.at(i)->hasCompileTimeValue(manIdx));
    const CompileTimeValue opCompileTimeValue = operands.at(i)->getCompileTimeValue(manIdx);
    const ShiftOp op = opQueueCopy.front().first;
    opQueueCopy.pop();
    if (op == ShiftOp::OP_SHIFT_LEFT)
      result.longValue <<= opCompileTimeValue.longValue;
    else if (op == ShiftOp::OP_SHIFT_RIGHT)
      result.longValue >>= opCompileTimeValue.longValue;
    else
      throw CompilerError(UNHANDLED_BRANCH, "ShiftExprNode::getCompileTimeValue()");
  }
  return result;
}

bool AdditiveExprNode::hasCompileTimeValue(size_t manIdx) const {
  return std::ranges::all_of(operands, [=](const MultiplicativeExprNode *node) { return node->hasCompileTimeValue(manIdx); });
}

CompileTimeValue AdditiveExprNode::getCompileTimeValue(size_t manIdx) const {
  if (operands.size() == 1)
    return operands.front()->getCompileTimeValue(manIdx);

  CompileTimeValue result = operands.front()->getCompileTimeValue(manIdx);
  OpQueue opQueueCopy = opQueue;
  for (size_t i = 1; i < operands.size(); i++) {
    assert(operands.at(i)->hasCompileTimeValue(manIdx));
    const CompileTimeValue opCompileTimeValue = operands.at(i)->getCompileTimeValue(manIdx);
    const AdditiveOp op = opQueueCopy.front().first;
    opQueueCopy.pop();
    if (op == AdditiveOp::OP_PLUS)
      result.longValue += opCompileTimeValue.longValue;
    else if (op == AdditiveOp::OP_MINUS)
      result.longValue -= opCompileTimeValue.longValue;
    else
      throw CompilerError(UNHANDLED_BRANCH, "AdditiveExprNode::getCompileTimeValue()");
  }
  return result;
}

bool MultiplicativeExprNode::hasCompileTimeValue(size_t manIdx) const {
  return std::ranges::all_of(operands, [=](const CastExprNode *node) { return node->hasCompileTimeValue(manIdx); });
}

CompileTimeValue MultiplicativeExprNode::getCompileTimeValue(size_t manIdx) const {
  if (operands.size() == 1)
    return operands.front()->getCompileTimeValue(manIdx);

  CompileTimeValue result = operands.front()->getCompileTimeValue(manIdx);
  OpQueue opQueueCopy = opQueue;
  for (size_t i = 1; i < operands.size(); i++) {
    assert(operands.at(i)->hasCompileTimeValue(manIdx));
    const CompileTimeValue opCompileTimeValue = operands.at(i)->getCompileTimeValue(manIdx);
    const MultiplicativeOp op = opQueueCopy.front().first;
    opQueueCopy.pop();
    if (op == MultiplicativeOp::OP_MUL) {
      result.longValue *= opCompileTimeValue.longValue;
    } else if (op == MultiplicativeOp::OP_DIV) {
      if (opCompileTimeValue.longValue == 0)
        throw SemanticError(operands.at(i), DIVISION_BY_ZERO, "Dividing by zero is not allowed.");
      result.longValue /= opCompileTimeValue.longValue;
    } else if (op == MultiplicativeOp::OP_REM) {
      result.longValue %= opCompileTimeValue.longValue;
    } else {
      throw CompilerError(UNHANDLED_BRANCH, "MultiplicativeExprNode::getCompileTimeValue()");
    }
  }
  return result;
}

bool CastExprNode::hasCompileTimeValue(size_t manIdx) const {
  return isCast ? assignExpr->hasCompileTimeValue(manIdx) : prefixUnaryExpr->hasCompileTimeValue(manIdx);
}

CompileTimeValue CastExprNode::getCompileTimeValue(size_t manIdx) const {
  return isCast ? assignExpr->getCompileTimeValue(manIdx) : prefixUnaryExpr->getCompileTimeValue(manIdx);
}

bool PrefixUnaryExprNode::hasCompileTimeValue(size_t manIdx) const { // NOLINT(*-no-recursion)
  if (postfixUnaryExpr)
    return postfixUnaryExpr->hasCompileTimeValue(manIdx);

  const bool isSupported = op == PrefixUnaryOp::OP_NONE || op == PrefixUnaryOp::OP_MINUS || op == PrefixUnaryOp::OP_PLUS_PLUS ||
                           op == PrefixUnaryOp::OP_MINUS_MINUS || op == PrefixUnaryOp::OP_NOT ||
                           op == PrefixUnaryOp::OP_BITWISE_NOT;
  return isSupported && prefixUnaryExpr->hasCompileTimeValue(manIdx);
}

CompileTimeValue PrefixUnaryExprNode::getCompileTimeValue(size_t manIdx) const { // NOLINT(*-no-recursion)
  if (postfixUnaryExpr)
    return postfixUnaryExpr->getCompileTimeValue(manIdx);

  CompileTimeValue opValue = prefixUnaryExpr->getCompileTimeValue(manIdx);
  if (op == PrefixUnaryOp::OP_MINUS)
    return CompileTimeValue{.longValue = -opValue.longValue};
  if (op == PrefixUnaryOp::OP_PLUS_PLUS)
    return CompileTimeValue{.longValue = ++opValue.longValue};
  if (op == PrefixUnaryOp::OP_MINUS_MINUS)
    return CompileTimeValue{.longValue = --opValue.longValue};
  if (op == PrefixUnaryOp::OP_NOT)
    return CompileTimeValue{.boolValue = !opValue.boolValue};
  if (op == PrefixUnaryOp::OP_BITWISE_NOT)
    return CompileTimeValue{.longValue = ~opValue.longValue};

  throw CompilerError(UNHANDLED_BRANCH, "PrefixUnaryExprNode::getCompileTimeValue()");
}

bool PostfixUnaryExprNode::hasCompileTimeValue(size_t manIdx) const { // NOLINT(*-no-recursion)
  if (atomicExpr)
    return atomicExpr->hasCompileTimeValue(manIdx);

  const bool isSupported =
      op == PostfixUnaryOp::OP_NONE || op == PostfixUnaryOp::OP_PLUS_PLUS || op == PostfixUnaryOp::OP_MINUS_MINUS;
  return isSupported && postfixUnaryExpr->hasCompileTimeValue(manIdx);
}

CompileTimeValue PostfixUnaryExprNode::getCompileTimeValue(size_t manIdx) const { // NOLINT(*-no-recursion)
  if (atomicExpr)
    return atomicExpr->getCompileTimeValue(manIdx);

  CompileTimeValue opValue = postfixUnaryExpr->getCompileTimeValue(manIdx);
  if (op == PostfixUnaryOp::OP_PLUS_PLUS)
    return CompileTimeValue{.longValue = opValue.longValue++};
  if (op == PostfixUnaryOp::OP_MINUS_MINUS)
    return CompileTimeValue{.longValue = opValue.longValue--};

  throw CompilerError(UNHANDLED_BRANCH, "PostfixUnaryExprNode::getCompileTimeValue()");
}

bool ValueNode::hasCompileTimeValue(size_t manIdx) const { return isNil || ASTNode::hasCompileTimeValue(manIdx); }

CompileTimeValue ValueNode::getCompileTimeValue(size_t manIdx) const {
  return isNil ? CompileTimeValue{.longValue = 0} : ASTNode::getCompileTimeValue(manIdx);
}

bool FctCallNode::hasCompileTimeValue(size_t manIdx) const {
  return BUILTIN_FUNCTIONS_MAP.contains(fqFunctionName) && data.at(manIdx).compileTimeValueSet;
}

CompileTimeValue FctCallNode::getCompileTimeValue(size_t manIdx) const { return data.at(manIdx).compileTimeValue; }

void FctCallNode::setCompileTimeValue(const CompileTimeValue &value, size_t manIdx) {
  data.at(manIdx).setCompileTimeValue(value);
}

bool FctCallNode::returnsOnAllControlPaths(bool *overrideUnreachable, size_t manIdx) const {
  // Some builtin functions terminate the control flow, e.g. panic
  const auto it = BUILTIN_FUNCTIONS_MAP.find(fqFunctionName);
  return it != BUILTIN_FUNCTIONS_MAP.end() && it->second.isFunctionTerminator;
}

/**
 * Check if right above the closest assign expression ancestor is a statement node
 *
 * @return Has return value receiver or not
 */
bool FctCallNode::hasReturnValueReceiver() const {
  const ASTNode *node = parent;
  while (!node->isAssignExpr()) {
    // As soon as we have a node with more than one child, we know that the return value is used
    if (node->getChildren().size() > 1)
      return true;
    node = node->parent;
  }
  // Also check the condition of the assign expression
  return node->getChildren().size() > 1 || !node->parent->isExprStmt();
}

bool LambdaFuncNode::returnsOnAllControlPaths(bool *overrideUnreachable, size_t manIdx) const {
  return body->returnsOnAllControlPaths(overrideUnreachable, manIdx);
}

bool LambdaProcNode::returnsOnAllControlPaths(bool *overrideUnreachable, size_t manIdx) const {
  return body->returnsOnAllControlPaths(overrideUnreachable, manIdx);
}

void DataTypeNode::setFieldTypeRecursive() { // NOLINT(*-no-recursion)
  // Set the current node to field type
  isFieldType = true;
  // Do the same for all template nodes
  if (const CustomDataTypeNode *customType = baseDataType->customDataType; customType != nullptr && customType->templateTypeLst)
    for (DataTypeNode *templateNode : customType->templateTypeLst->dataTypes)
      templateNode->setFieldTypeRecursive();
}

} // namespace spice::compiler