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
  if (size_t offset = inputStream->getText(extSourceInterval).find('\n'); offset != std::string::npos)
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

bool MainFctDefNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const {
  return body()->returnsOnAllControlPaths(doSetPredecessorsUnreachable);
}

bool FctDefBaseNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const {
  return body()->returnsOnAllControlPaths(doSetPredecessorsUnreachable);
}

bool ForLoopNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const {
  return condAssign->hasCompileTimeValue() && condAssign->getCompileTimeValue().boolValue;
}

bool WhileLoopNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const {
  return condition->hasCompileTimeValue() && condition->getCompileTimeValue().boolValue;
}

bool DoWhileLoopNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const {
  const AssignExprNode *cond = condition();
  return cond->hasCompileTimeValue() && cond->getCompileTimeValue().boolValue;
}

bool IfStmtNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const { // NOLINT(misc-no-recursion)
  // An if statement returns on all control paths, if then and else block return on all control paths
  const AssignExprNode *cond = condition();
  if (!cond->hasCompileTimeValue())
    return thenBody()->returnsOnAllControlPaths(doSetPredecessorsUnreachable) && elseStmt() != nullptr &&
           elseStmt()->returnsOnAllControlPaths(doSetPredecessorsUnreachable);

  // If the condition always evaluates to 'true' only the then block must return and vice versa
  const CompileTimeValue &compileTimeValue = cond->getCompileTimeValue();
  if (compileTimeValue.boolValue)
    return thenBody()->returnsOnAllControlPaths(doSetPredecessorsUnreachable);
  return elseStmt() != nullptr && elseStmt()->returnsOnAllControlPaths(doSetPredecessorsUnreachable);
}

bool ElseStmtNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const { // NOLINT(misc-no-recursion)
  return isElseIf ? ifStmt()->returnsOnAllControlPaths(doSetPredecessorsUnreachable)
                  : body()->returnsOnAllControlPaths(doSetPredecessorsUnreachable);
}

bool SwitchStmtNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const {
  const std::vector<CaseBranchNode *> caseNodes = caseBranches();
  const DefaultBranchNode *defaultBranchNode = defaultBranch();

  const bool allCaseBranchesReturn = std::ranges::all_of(
      caseNodes, [=](CaseBranchNode *node) { return node->returnsOnAllControlPaths(doSetPredecessorsUnreachable); });
  const bool defaultBranchReturns =
      defaultBranchNode && defaultBranchNode->returnsOnAllControlPaths(doSetPredecessorsUnreachable);

  return allCaseBranchesReturn && defaultBranchReturns;
}

bool CaseBranchNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const {
  return body()->returnsOnAllControlPaths(doSetPredecessorsUnreachable);
}

bool DefaultBranchNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const {
  return body()->returnsOnAllControlPaths(doSetPredecessorsUnreachable);
}

bool StmtLstNode::returnsOnAllControlPaths(bool *) const {
  // An empty statement list does not return at all
  if (children.empty())
    return false;
  // A statement list returns on all control paths, if the one direct child statement returns on all control paths
  bool returns = false;
  bool doSetPredecessorsUnreachable = true;
  for (ASTNode *child : children) {
    if (!child)
      continue;
    if (returns) {
      // Prevent marking as unreachable if doSetPredecessorsUnreachable is set to false
      child->unreachable = doSetPredecessorsUnreachable;
    } else if (child->returnsOnAllControlPaths(&doSetPredecessorsUnreachable)) {
      returns = true;
    }
  }
  return returns;
}

std::vector<const CompileTimeValue *> AttrLstNode::getAttrValuesByName(const std::string &key) const {
  assert(ATTR_CONFIGS.contains(key));
  const std::vector<AttrNode *> attrNodes = attributes();

  std::vector<const CompileTimeValue *> attributeValues;
  for (AttrNode *attrNode : attrNodes) {
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
  return std::ranges::any_of(attrs, [&](AttrNode *attr) { return attr->key == key; });
}

const CompileTimeValue *AttrNode::getValue() const { return value() ? &value()->compileTimeValue : nullptr; }

bool FallthroughStmtNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const { return true; }

bool AssignExprNode::returnsOnAllControlPaths(bool *doSetPredecessorsUnreachable) const {
  if (op == OP_NONE) {
    return children.front()->returnsOnAllControlPaths(doSetPredecessorsUnreachable);
  } else {
    bool returns = op == OP_ASSIGN && lhs()->postfixUnaryExpr() && lhs()->postfixUnaryExpr()->atomicExpr() &&
                   lhs()->postfixUnaryExpr()->atomicExpr()->fqIdentifier == RETURN_VARIABLE_NAME;

    // If we assign the result variable, we technically return from the function, but at the end of the function.
    // Therefore, the following code is not unreachable, but will be executed in any case.
    if (returns)
      *doSetPredecessorsUnreachable = false;

    return returns;
  }
}

bool TernaryExprNode::hasCompileTimeValue() const {
  return std::ranges::all_of(operands(), [](const LogicalOrExprNode *node) { return node->hasCompileTimeValue(); });
}

CompileTimeValue GlobalVarDefNode::getCompileTimeValue() const { return constant()->getCompileTimeValue(); }

CompileTimeValue TernaryExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();

  const std::vector<LogicalOrExprNode *> &ops = operands();

  // If the condition has no compile time value, we do not need to evaluate the true and false values
  const LogicalOrExprNode *condition = ops[0];
  if (!condition->hasCompileTimeValue())
    return {};

  // Check if condition always evaluates to 'true'
  if (condition->getCompileTimeValue().boolValue) {
    const LogicalOrExprNode *trueValue = isShortened ? condition : ops[1];
    return trueValue->getCompileTimeValue();
  } else {
    const LogicalOrExprNode *falseValue = isShortened ? operands()[1] : ops[2];
    return falseValue->getCompileTimeValue();
  }
}

bool LogicalOrExprNode::hasCompileTimeValue() const {
  return std::ranges::all_of(operands(), [](const LogicalAndExprNode *node) { return node->hasCompileTimeValue(); });
}

CompileTimeValue LogicalOrExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();

  // Check if one expression evaluates to 'true'
  for (const LogicalAndExprNode *op : operands()) {
    assert(op->hasCompileTimeValue());
    // If one operand evaluates to 'true' the whole expression is 'true'
    CompileTimeValue opCompileTimeValue = op->getCompileTimeValue();
    if (opCompileTimeValue.boolValue)
      return CompileTimeValue{.boolValue = true};
  }

  // Return 'false'
  return CompileTimeValue{.boolValue = false};
}

bool LogicalAndExprNode::hasCompileTimeValue() const {
  return std::ranges::all_of(operands(), [](const BitwiseOrExprNode *node) { return node->hasCompileTimeValue(); });
}

CompileTimeValue LogicalAndExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();

  // Check if all expressions evaluate to 'true'
  for (const BitwiseOrExprNode *op : operands()) {
    assert(op->hasCompileTimeValue());
    // If one operand evaluates to 'false' the whole expression is 'false'
    CompileTimeValue opCompileTimeValue = op->getCompileTimeValue();
    if (!opCompileTimeValue.boolValue)
      return CompileTimeValue{.boolValue = false};
  }

  // Return 'false'
  return CompileTimeValue{.boolValue = false};
}

bool BitwiseOrExprNode::hasCompileTimeValue() const {
  return std::ranges::all_of(operands(), [](const BitwiseXorExprNode *node) { return node->hasCompileTimeValue(); });
}

CompileTimeValue BitwiseOrExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();

  const std::vector<BitwiseXorExprNode *> ops = operands();
  CompileTimeValue result = ops.front()->getCompileTimeValue();
  for (size_t i = 1; i < ops.size(); i++) {
    assert(ops.at(i)->hasCompileTimeValue());
    CompileTimeValue opCompileTimeValue = ops.at(i)->getCompileTimeValue();
    result.longValue |= opCompileTimeValue.longValue;
  }

  return result;
}

bool BitwiseXorExprNode::hasCompileTimeValue() const {
  return std::ranges::all_of(operands(), [](const BitwiseAndExprNode *node) { return node->hasCompileTimeValue(); });
}

CompileTimeValue BitwiseXorExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();

  const std::vector<BitwiseAndExprNode *> ops = operands();
  CompileTimeValue result = ops.front()->getCompileTimeValue();
  for (size_t i = 1; i < ops.size(); i++) {
    assert(ops.at(i)->hasCompileTimeValue());
    CompileTimeValue opCompileTimeValue = ops.at(i)->getCompileTimeValue();
    result.longValue ^= opCompileTimeValue.longValue;
  }

  return result;
}

bool BitwiseAndExprNode::hasCompileTimeValue() const {
  return std::ranges::all_of(operands(), [](const EqualityExprNode *node) { return node->hasCompileTimeValue(); });
}

CompileTimeValue BitwiseAndExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();

  const std::vector<EqualityExprNode *> ops = operands();
  CompileTimeValue result = ops.front()->getCompileTimeValue();
  for (size_t i = 1; i < ops.size(); i++) {
    assert(ops.at(i)->hasCompileTimeValue());
    CompileTimeValue opCompileTimeValue = ops.at(i)->getCompileTimeValue();
    result.longValue &= opCompileTimeValue.longValue;
  }

  return result;
}

bool EqualityExprNode::hasCompileTimeValue() const {
  return std::ranges::all_of(operands(), [](const RelationalExprNode *node) { return node->hasCompileTimeValue(); });
}

CompileTimeValue EqualityExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();

  const std::vector<RelationalExprNode *> ops = operands();
  CompileTimeValue op0Value = ops.at(0)->getCompileTimeValue();
  CompileTimeValue op1Value = ops.at(1)->getCompileTimeValue();
  if (op == OP_EQUAL)
    return CompileTimeValue{.boolValue = op0Value.longValue == op1Value.longValue};
  if (op == OP_NOT_EQUAL)
    return CompileTimeValue{.boolValue = op0Value.longValue != op1Value.longValue};

  throw CompilerError(UNHANDLED_BRANCH, "EqualityExprNode::getCompileTimeValue()");
}

bool RelationalExprNode::hasCompileTimeValue() const {
  return std::ranges::all_of(operands(), [](const ShiftExprNode *node) { return node->hasCompileTimeValue(); });
}

CompileTimeValue RelationalExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();

  const std::vector<ShiftExprNode *> ops = operands();
  CompileTimeValue op0Value = ops.at(0)->getCompileTimeValue();
  CompileTimeValue op1Value = ops.at(1)->getCompileTimeValue();
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
  return std::ranges::all_of(operands(), [](const AdditiveExprNode *node) { return node->hasCompileTimeValue(); });
}

CompileTimeValue ShiftExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();

  const std::vector<AdditiveExprNode *> ops = operands();
  CompileTimeValue op0Value = ops.at(0)->getCompileTimeValue();
  CompileTimeValue op1Value = ops.at(1)->getCompileTimeValue();
  if (op == OP_SHIFT_LEFT)
    return CompileTimeValue{.longValue = op0Value.longValue << op1Value.longValue};
  if (op == OP_SHIFT_RIGHT)
    return CompileTimeValue{.longValue = op0Value.longValue >> op1Value.longValue};

  throw CompilerError(UNHANDLED_BRANCH, "ShiftExprNode::getCompileTimeValue()");
}

bool AdditiveExprNode::hasCompileTimeValue() const {
  return std::ranges::all_of(operands(), [](const MultiplicativeExprNode *node) { return node->hasCompileTimeValue(); });
}

CompileTimeValue AdditiveExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();

  const std::vector<MultiplicativeExprNode *> ops = operands();
  CompileTimeValue result = ops.front()->getCompileTimeValue();
  OpQueue opQueueCopy = opQueue;
  for (size_t i = 1; i < ops.size(); i++) {
    assert(ops.at(i)->hasCompileTimeValue());
    CompileTimeValue opCompileTimeValue = ops.at(i)->getCompileTimeValue();
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
  return std::ranges::all_of(operands(), [](const CastExprNode *node) { return node->hasCompileTimeValue(); });
}

CompileTimeValue MultiplicativeExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();

  const std::vector<CastExprNode *> ops = operands();
  CompileTimeValue result = ops.front()->getCompileTimeValue();
  OpQueue opQueueCopy = opQueue;
  for (size_t i = 1; i < ops.size(); i++) {
    assert(ops.at(i)->hasCompileTimeValue());
    CompileTimeValue opCompileTimeValue = ops.at(i)->getCompileTimeValue();
    const MultiplicativeOp op = opQueueCopy.front().first;
    opQueueCopy.pop();
    if (op == OP_MUL) {
      result.longValue *= opCompileTimeValue.longValue;
    } else if (op == OP_DIV) {
      if (opCompileTimeValue.longValue == 0)
        throw SemanticError(ops.at(i), DIVISION_BY_ZERO, "Dividing by zero is not allowed.");
      result.longValue /= opCompileTimeValue.longValue;
    } else if (op == OP_REM) {
      result.longValue %= opCompileTimeValue.longValue;
    } else {
      throw CompilerError(UNHANDLED_BRANCH, "MultiplicativeExprNode::getCompileTimeValue()");
    }
  }
  return result;
}

bool CastExprNode::hasCompileTimeValue() const { return prefixUnaryExpr()->hasCompileTimeValue(); }

CompileTimeValue CastExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();

  return prefixUnaryExpr()->getCompileTimeValue();
}

bool PrefixUnaryExprNode::hasCompileTimeValue() const { // NOLINT(*-no-recursion)
  if (postfixUnaryExpr())
    return postfixUnaryExpr()->hasCompileTimeValue();

  bool isOperatorSupported =
      op == OP_NONE || op == OP_MINUS || op == OP_PLUS_PLUS || op == OP_MINUS_MINUS || op == OP_NOT || op == OP_BITWISE_NOT;
  return isOperatorSupported && prefixUnary()->hasCompileTimeValue();
}

CompileTimeValue PrefixUnaryExprNode::getCompileTimeValue() const { // NOLINT(*-no-recursion)
  if (postfixUnaryExpr())
    return postfixUnaryExpr()->getCompileTimeValue();

  CompileTimeValue opValue = prefixUnary()->getCompileTimeValue();
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
  if (atomicExpr())
    return atomicExpr()->hasCompileTimeValue();

  bool isOperatorSupported = op == OP_NONE || op == OP_PLUS_PLUS || op == OP_MINUS_MINUS;
  return isOperatorSupported && postfixUnaryExpr()->hasCompileTimeValue();
}

CompileTimeValue PostfixUnaryExprNode::getCompileTimeValue() const { // NOLINT(*-no-recursion)
  if (atomicExpr())
    return atomicExpr()->getCompileTimeValue();

  CompileTimeValue opValue = postfixUnaryExpr()->getCompileTimeValue();
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
  ASTNode *node = parent;
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
  return body()->returnsOnAllControlPaths(overrideUnreachable);
}

bool LambdaProcNode::returnsOnAllControlPaths(bool *overrideUnreachable) const {
  return body()->returnsOnAllControlPaths(overrideUnreachable);
}

void DataTypeNode::setFieldTypeRecursive() { // NOLINT(*-no-recursion)
  // Set the current node to field type
  isFieldType = true;
  // Do the same for all template nodes
  BaseDataTypeNode *baseType = baseDataType();
  CustomDataTypeNode *customType = baseType->customDataType();
  if (customType != nullptr && customType->templateTypeLst())
    for (DataTypeNode *templateNode : customType->templateTypeLst()->dataTypes())
      templateNode->setFieldTypeRecursive();
}

} // namespace spice::compiler