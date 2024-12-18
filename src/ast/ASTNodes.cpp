// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include <ast/ASTNodes.h>

#include "SourceFile.h"
#include <ast/Attributes.h>
#include <exception/SemanticError.h>
#include <symboltablebuilder/SymbolTableBuilder.h>

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
  assert(parent != nullptr && "Could not find next outer statement list");
  return isStmtLstNode() ? spice_pointer_cast<const StmtLstNode *>(this) : parent->getNextOuterStmtLst();
}

bool MainFctDefNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const {
  return body()->returnsOnAllControlPaths(doSetPredecessorsUnreachable);
}

bool FctDefBaseNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const {
  return body()->returnsOnAllControlPaths(doSetPredecessorsUnreachable);
}

CompileTimeValue GlobalVarDefNode::getCompileTimeValue() const { return constant()->getCompileTimeValue(); }

bool ForLoopNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const {
  // If we have the guarantee that the loop condition is always true and the loop body returns on all control paths,
  // we can assume that the loop itself will always return
  const bool loopConditionAlwaysTrue = condAssign()->hasCompileTimeValue() && condAssign()->getCompileTimeValue().boolValue;
  return loopConditionAlwaysTrue && body()->returnsOnAllControlPaths(doSetPredecessorsUnreachable);
}

bool WhileLoopNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const {
  // If we have the guarantee that the loop condition is always true and the loop body returns on all control paths,
  // we can assume that the loop itself will always return
  const bool loopConditionAlwaysTrue = condition()->hasCompileTimeValue() && condition()->getCompileTimeValue().boolValue;
  return loopConditionAlwaysTrue && body()->returnsOnAllControlPaths(doSetPredecessorsUnreachable);
}

bool DoWhileLoopNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const {
  // Do-while loops will always be executed at least once. So if the body returns on all control paths, the loop will as well
  return body()->returnsOnAllControlPaths(doSetPredecessorsUnreachable);
}

bool IfStmtNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const { // NOLINT(misc-no-recursion)
  // If the condition always evaluates to 'true' the then block must return
  const AssignExprNode *cond = condition();
  if (cond->hasCompileTimeValue() && cond->getCompileTimeValue().boolValue)
    return thenBody()->returnsOnAllControlPaths(doSetPredecessorsUnreachable);

  // If the condition always evaluates to 'false' the else block must return
  if (cond->hasCompileTimeValue() && !cond->getCompileTimeValue().boolValue)
    return elseStmt() != nullptr && elseStmt()->returnsOnAllControlPaths(doSetPredecessorsUnreachable);

  // If the condition does not always evaluate to 'true' or 'false' we need to check both branches
  return thenBody()->returnsOnAllControlPaths(doSetPredecessorsUnreachable) && elseStmt() != nullptr &&
         elseStmt()->returnsOnAllControlPaths(doSetPredecessorsUnreachable);
}

bool ElseStmtNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const { // NOLINT(misc-no-recursion)
  if (isElseIf)
    return ifStmt()->returnsOnAllControlPaths(doSetPredecessorsUnreachable);
  return body()->returnsOnAllControlPaths(doSetPredecessorsUnreachable);
}

bool SwitchStmtNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const {
  const auto pred = [=](const CaseBranchNode *node) { return node->returnsOnAllControlPaths(doSetPredecessorsUnreachable); };
  const bool allCaseBranchesReturn = std::ranges::all_of(caseBranches(), pred);
  const bool defaultBranchReturns = !defaultBranch() || defaultBranch()->returnsOnAllControlPaths(doSetPredecessorsUnreachable);
  return allCaseBranchesReturn && defaultBranchReturns;
}

bool CaseBranchNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const {
  return body()->returnsOnAllControlPaths(doSetPredecessorsUnreachable);
}

bool DefaultBranchNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const {
  return body()->returnsOnAllControlPaths(doSetPredecessorsUnreachable);
}

bool StmtLstNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const {
  // An empty statement list does not return at all
  if (children.empty())
    return false;
  // A statement list returns on all control paths, if the one direct child statement returns on all control paths
  bool returnsOnAllControlPaths = false;
  for (ASTNode *child : children) {
    assert(child != nullptr);

    // Prevent marking instructions as unreachable if doSetPredecessorsUnreachable is set to false
    if (returnsOnAllControlPaths && *doSetPredecessorsUnreachable)
      child->unreachable = true;

    if (child->returnsOnAllControlPaths(doSetPredecessorsUnreachable))
      returnsOnAllControlPaths = true;
  }
  return returnsOnAllControlPaths;
}

std::vector<const CompileTimeValue *> AttrLstNode::getAttrValuesByName(const std::string &key) const {
  assert(ATTR_CONFIGS.contains(key));
  const std::vector<AttrNode *> attrNodes = attributes();

  std::vector<const CompileTimeValue *> attributeValues;
  for (const AttrNode *attrNode : attrNodes) {
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
  const std::vector<AttrNode *> attrs = attributes();
  return std::ranges::any_of(attrs, [&](const AttrNode *attr) { return attr->key == key; });
}

const CompileTimeValue *AttrNode::getValue() const { return value() ? &value()->compileTimeValue : nullptr; }

bool AssertStmtNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const {
  // If the expression, passed to the assert statement is always evaluated to false, the assert statement will never succeed
  return assignExpr()->hasCompileTimeValue() && !assignExpr()->getCompileTimeValue().boolValue;
}

bool AssignExprNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const {
  // If it's a ternary, do the default thing
  if (op == OP_NONE)
    return children.front()->returnsOnAllControlPaths(doSetPredecessorsUnreachable);

  // If it's a modification on the result variable, we technically return from the function, but at the end of the function.
  const bool hasAtomicExpr = lhs->postfixUnaryExpr && lhs->postfixUnaryExpr->atomicExpr;
  if (hasAtomicExpr && lhs->postfixUnaryExpr->atomicExpr->fqIdentifier == RETURN_VARIABLE_NAME) {
    // If we assign the result variable, we technically return from the function, but at the end of the function.
    // Therefore, the following code is not unreachable, but will be executed in any case.
    *doSetPredecessorsUnreachable = false;
    return true;
  }

  return false;
}

bool TernaryExprNode::hasCompileTimeValue() const {
  const bool trueExprHasCompileTimeValue = !trueExpr || trueExpr->hasCompileTimeValue();
  const bool falseExprHasCompileTimeValue = !falseExpr || falseExpr->hasCompileTimeValue();
  return condition->hasCompileTimeValue() && trueExprHasCompileTimeValue && falseExprHasCompileTimeValue;
}

CompileTimeValue TernaryExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();

  // If the condition has no compile time value, we do not need to evaluate the true and false values
  if (!condition->hasCompileTimeValue())
    return {};

  // Check if condition always evaluates to 'true'
  if (condition->getCompileTimeValue().boolValue) {
    const LogicalOrExprNode *trueValue = isShortened ? condition : trueExpr;
    return trueValue->getCompileTimeValue();
  } else {
    return falseExpr->getCompileTimeValue();
  }
}

bool LogicalOrExprNode::hasCompileTimeValue() const {
  return std::ranges::all_of(operands, [](const LogicalAndExprNode *node) { return node->hasCompileTimeValue(); });
}

CompileTimeValue LogicalOrExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();

  // Check if one expression evaluates to 'true'
  for (const LogicalAndExprNode *op : operands) {
    assert(op->hasCompileTimeValue());
    // If one operand evaluates to 'true' the whole expression is 'true'
    const CompileTimeValue opCompileTimeValue = op->getCompileTimeValue();
    if (opCompileTimeValue.boolValue)
      return CompileTimeValue{.boolValue = true};
  }

  // Return 'false'
  return CompileTimeValue{.boolValue = false};
}

bool LogicalAndExprNode::hasCompileTimeValue() const {
  return std::ranges::all_of(operands, [](const BitwiseOrExprNode *node) { return node->hasCompileTimeValue(); });
}

CompileTimeValue LogicalAndExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();

  // Check if all expressions evaluate to 'true'
  for (const BitwiseOrExprNode *op : operands) {
    assert(op->hasCompileTimeValue());
    // If one operand evaluates to 'false' the whole expression is 'false'
    const CompileTimeValue opCompileTimeValue = op->getCompileTimeValue();
    if (!opCompileTimeValue.boolValue)
      return CompileTimeValue{.boolValue = false};
  }

  // Return 'false'
  return CompileTimeValue{.boolValue = false};
}

bool BitwiseOrExprNode::hasCompileTimeValue() const {
  return std::ranges::all_of(operands, [](const BitwiseXorExprNode *node) { return node->hasCompileTimeValue(); });
}

CompileTimeValue BitwiseOrExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();

  CompileTimeValue result = operands.front()->getCompileTimeValue();
  for (size_t i = 1; i < operands.size(); i++) {
    assert(operands.at(i)->hasCompileTimeValue());
    const CompileTimeValue opCompileTimeValue = operands.at(i)->getCompileTimeValue();
    result.longValue |= opCompileTimeValue.longValue;
  }

  return result;
}

bool BitwiseXorExprNode::hasCompileTimeValue() const {
  return std::ranges::all_of(operands, [](const BitwiseAndExprNode *node) { return node->hasCompileTimeValue(); });
}

CompileTimeValue BitwiseXorExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();

  CompileTimeValue result = operands.front()->getCompileTimeValue();
  for (size_t i = 1; i < operands.size(); i++) {
    assert(operands.at(i)->hasCompileTimeValue());
    const CompileTimeValue opCompileTimeValue = operands.at(i)->getCompileTimeValue();
    result.longValue ^= opCompileTimeValue.longValue;
  }

  return result;
}

bool BitwiseAndExprNode::hasCompileTimeValue() const {
  return std::ranges::all_of(operands, [](const EqualityExprNode *node) { return node->hasCompileTimeValue(); });
}

CompileTimeValue BitwiseAndExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();

  CompileTimeValue result = operands.front()->getCompileTimeValue();
  for (size_t i = 1; i < operands.size(); i++) {
    assert(operands.at(i)->hasCompileTimeValue());
    const CompileTimeValue opCompileTimeValue = operands.at(i)->getCompileTimeValue();
    result.longValue &= opCompileTimeValue.longValue;
  }

  return result;
}

bool EqualityExprNode::hasCompileTimeValue() const {
  return std::ranges::all_of(operands, [](const RelationalExprNode *node) { return node->hasCompileTimeValue(); });
}

CompileTimeValue EqualityExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();

  const CompileTimeValue op0Value = operands.at(0)->getCompileTimeValue();
  const CompileTimeValue op1Value = operands.at(1)->getCompileTimeValue();
  if (op == OP_EQUAL)
    return CompileTimeValue{.boolValue = op0Value.longValue == op1Value.longValue};
  if (op == OP_NOT_EQUAL)
    return CompileTimeValue{.boolValue = op0Value.longValue != op1Value.longValue};

  throw CompilerError(UNHANDLED_BRANCH, "EqualityExprNode::getCompileTimeValue()");
}

bool RelationalExprNode::hasCompileTimeValue() const {
  return std::ranges::all_of(operands, [](const ShiftExprNode *node) { return node->hasCompileTimeValue(); });
}

CompileTimeValue RelationalExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();

  const CompileTimeValue op0Value = operands.at(0)->getCompileTimeValue();
  const CompileTimeValue op1Value = operands.at(1)->getCompileTimeValue();
  if (op == OP_LESS)
    return CompileTimeValue{.boolValue = op0Value.longValue < op1Value.longValue};
  if (op == OP_GREATER)
    return CompileTimeValue{.boolValue = op0Value.longValue > op1Value.longValue};
  if (op == OP_LESS_EQUAL)
    return CompileTimeValue{.boolValue = op0Value.longValue <= op1Value.longValue};
  if (op == OP_GREATER_EQUAL)
    return CompileTimeValue{.boolValue = op0Value.longValue >= op1Value.longValue};

  throw CompilerError(UNHANDLED_BRANCH, "RelationalExprNode::getCompileTimeValue()");
}

bool ShiftExprNode::hasCompileTimeValue() const {
  return std::ranges::all_of(operands, [](const AdditiveExprNode *node) { return node->hasCompileTimeValue(); });
}

CompileTimeValue ShiftExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();

  const CompileTimeValue op0Value = operands.at(0)->getCompileTimeValue();
  const CompileTimeValue op1Value = operands.at(1)->getCompileTimeValue();
  if (op == OP_SHIFT_LEFT)
    return CompileTimeValue{.longValue = op0Value.longValue << op1Value.longValue};
  if (op == OP_SHIFT_RIGHT)
    return CompileTimeValue{.longValue = op0Value.longValue >> op1Value.longValue};

  throw CompilerError(UNHANDLED_BRANCH, "ShiftExprNode::getCompileTimeValue()");
}

bool AdditiveExprNode::hasCompileTimeValue() const {
  return std::ranges::all_of(operands, [](const MultiplicativeExprNode *node) { return node->hasCompileTimeValue(); });
}

CompileTimeValue AdditiveExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();

  CompileTimeValue result = operands.front()->getCompileTimeValue();
  OpQueue opQueueCopy = opQueue;
  for (size_t i = 1; i < operands.size(); i++) {
    assert(operands.at(i)->hasCompileTimeValue());
    const CompileTimeValue opCompileTimeValue = operands.at(i)->getCompileTimeValue();
    const AdditiveOp op = opQueueCopy.front().first;
    opQueueCopy.pop();
    if (op == OP_PLUS)
      result.longValue += opCompileTimeValue.longValue;
    else if (op == OP_MINUS)
      result.longValue -= opCompileTimeValue.longValue;
    else
      throw CompilerError(UNHANDLED_BRANCH, "AdditiveExprNode::getCompileTimeValue()");
  }
  return result;
}

bool MultiplicativeExprNode::hasCompileTimeValue() const {
  return std::ranges::all_of(operands, [](const CastExprNode *node) { return node->hasCompileTimeValue(); });
}

CompileTimeValue MultiplicativeExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();

  CompileTimeValue result = operands.front()->getCompileTimeValue();
  OpQueue opQueueCopy = opQueue;
  for (size_t i = 1; i < operands.size(); i++) {
    assert(operands.at(i)->hasCompileTimeValue());
    const CompileTimeValue opCompileTimeValue = operands.at(i)->getCompileTimeValue();
    const MultiplicativeOp op = opQueueCopy.front().first;
    opQueueCopy.pop();
    if (op == OP_MUL) {
      result.longValue *= opCompileTimeValue.longValue;
    } else if (op == OP_DIV) {
      if (opCompileTimeValue.longValue == 0)
        throw SemanticError(operands.at(i), DIVISION_BY_ZERO, "Dividing by zero is not allowed.");
      result.longValue /= opCompileTimeValue.longValue;
    } else if (op == OP_REM) {
      result.longValue %= opCompileTimeValue.longValue;
    } else {
      throw CompilerError(UNHANDLED_BRANCH, "MultiplicativeExprNode::getCompileTimeValue()");
    }
  }
  return result;
}

bool CastExprNode::hasCompileTimeValue() const { return prefixUnaryExpr->hasCompileTimeValue(); }

CompileTimeValue CastExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();
  return prefixUnaryExpr->getCompileTimeValue();
}

bool PrefixUnaryExprNode::hasCompileTimeValue() const { // NOLINT(*-no-recursion)
  if (postfixUnaryExpr)
    return postfixUnaryExpr->hasCompileTimeValue();

  const bool isOperatorSupported =
      op == OP_NONE || op == OP_MINUS || op == OP_PLUS_PLUS || op == OP_MINUS_MINUS || op == OP_NOT || op == OP_BITWISE_NOT;
  return isOperatorSupported && prefixUnaryExpr->hasCompileTimeValue();
}

CompileTimeValue PrefixUnaryExprNode::getCompileTimeValue() const { // NOLINT(*-no-recursion)
  if (postfixUnaryExpr)
    return postfixUnaryExpr->getCompileTimeValue();

  CompileTimeValue opValue = prefixUnaryExpr->getCompileTimeValue();
  if (op == OP_MINUS)
    return CompileTimeValue{.longValue = -opValue.longValue};
  if (op == OP_PLUS_PLUS)
    return CompileTimeValue{.longValue = ++opValue.longValue};
  if (op == OP_MINUS_MINUS)
    return CompileTimeValue{.longValue = --opValue.longValue};
  if (op == OP_NOT)
    return CompileTimeValue{.boolValue = !opValue.boolValue};
  if (op == OP_BITWISE_NOT)
    return CompileTimeValue{.longValue = ~opValue.longValue};

  throw CompilerError(UNHANDLED_BRANCH, "PrefixUnaryExprNode::getCompileTimeValue()");
}

bool PostfixUnaryExprNode::hasCompileTimeValue() const { // NOLINT(*-no-recursion)
  if (atomicExpr)
    return atomicExpr->hasCompileTimeValue();

  const bool isOperatorSupported = op == OP_NONE || op == OP_PLUS_PLUS || op == OP_MINUS_MINUS;
  return isOperatorSupported && postfixUnaryExpr->hasCompileTimeValue();
}

CompileTimeValue PostfixUnaryExprNode::getCompileTimeValue() const { // NOLINT(*-no-recursion)
  if (atomicExpr)
    return atomicExpr->getCompileTimeValue();

  CompileTimeValue opValue = postfixUnaryExpr->getCompileTimeValue();
  if (op == OP_PLUS_PLUS)
    return CompileTimeValue{.longValue = opValue.longValue++};
  if (op == OP_MINUS_MINUS)
    return CompileTimeValue{.longValue = opValue.longValue--};

  throw CompilerError(UNHANDLED_BRANCH, "PostfixUnaryExprNode::getCompileTimeValue()");
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
    if (node->children.size() > 1)
      return true;
    node = node->parent;
  }
  // Also check the condition of the assign expression
  return node->children.size() > 1 || !node->parent->isStmtLstNode();
}

bool LambdaFuncNode::returnsOnAllControlPaths(bool *overrideUnreachable) const {
  return body->returnsOnAllControlPaths(overrideUnreachable);
}

bool LambdaProcNode::returnsOnAllControlPaths(bool *overrideUnreachable) const {
  return body->returnsOnAllControlPaths(overrideUnreachable);
}

void DataTypeNode::setFieldTypeRecursive() { // NOLINT(*-no-recursion)
  // Set the current node to field type
  isFieldType = true;
  // Do the same for all template nodes
  const CustomDataTypeNode *customType = baseDataType->customDataType();
  if (customType != nullptr && customType->templateTypeLst())
    for (DataTypeNode *templateNode : customType->templateTypeLst()->dataTypes())
      templateNode->setFieldTypeRecursive();
}

} // namespace spice::compiler