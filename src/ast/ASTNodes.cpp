// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include <ast/ASTNodes.h>

#include <symboltablebuilder/SymbolTableBuilder.h>

namespace spice::compiler {

bool MainFctDefNode::returnsOnAllControlPaths(bool *overrideUnreachable) const {
  return body()->returnsOnAllControlPaths(overrideUnreachable);
}

bool FctDefNode::returnsOnAllControlPaths(bool *overrideUnreachable) const {
  return body()->returnsOnAllControlPaths(overrideUnreachable);
}

bool ProcDefNode::returnsOnAllControlPaths(bool *overrideUnreachable) const {
  return body()->returnsOnAllControlPaths(overrideUnreachable);
}

bool ForLoopNode::returnsOnAllControlPaths(bool *overrideUnreachable) const {
  const AssignExprNode *cond = condAssign();
  return cond->hasCompileTimeValue() && cond->getCompileTimeValue().boolValue;
}

bool WhileLoopNode::returnsOnAllControlPaths(bool *overrideUnreachable) const {
  const AssignExprNode *cond = condition();
  return cond->hasCompileTimeValue() && cond->getCompileTimeValue().boolValue;
}

bool DoWhileLoopNode::returnsOnAllControlPaths(bool *overrideUnreachable) const {
  const AssignExprNode *cond = condition();
  return cond->hasCompileTimeValue() && cond->getCompileTimeValue().boolValue;
}

bool IfStmtNode::returnsOnAllControlPaths(bool *overrideUnreachable) const { // NOLINT(misc-no-recursion)
  // An if statement returns on all control paths, if then and else block return on all control paths
  const AssignExprNode *cond = condition();
  if (!cond->hasCompileTimeValue())
    return thenBody()->returnsOnAllControlPaths(overrideUnreachable) && elseStmt() != nullptr &&
           elseStmt()->returnsOnAllControlPaths(overrideUnreachable);

  // If the condition always evaluates to 'true' only the then block must return and vice versa
  const CompileTimeValue &compileTimeValue = cond->getCompileTimeValue();
  if (compileTimeValue.boolValue)
    return thenBody()->returnsOnAllControlPaths(overrideUnreachable);
  return elseStmt() != nullptr && elseStmt()->returnsOnAllControlPaths(overrideUnreachable);
}

bool ElseStmtNode::returnsOnAllControlPaths(bool *overrideUnreachable) const { // NOLINT(misc-no-recursion)
  return isElseIf ? ifStmt()->returnsOnAllControlPaths(overrideUnreachable)
                  : body()->returnsOnAllControlPaths(overrideUnreachable);
}

bool ThreadDefNode::returnsOnAllControlPaths(bool *overrideUnreachable) const { return false; }

bool StmtLstNode::returnsOnAllControlPaths(bool *) const {
  // An empty statement list does not return at all
  if (children.empty())
    return false;
  // A statement list returns on all control paths, if the one statement returns on all control paths
  bool returns = false;
  bool overrideUnreachable = false;
  for (ASTNode *child : children) {
    if (!child)
      continue;
    if (returns) {
      child->unreachable = !overrideUnreachable;
    } else if (child->returnsOnAllControlPaths(&overrideUnreachable)) {
      returns = true;
    }
  }
  return returns;
}

bool AssertStmtNode::returnsOnAllControlPaths(bool *overrideUnreachable) const {
  const bool returns = hasCompileTimeValue() && !compileTimeValue.boolValue;
  *overrideUnreachable |= returns;
  return returns;
}

bool AssignExprNode::returnsOnAllControlPaths(bool *overrideUnreachable) const {
  const bool returns = op == OP_ASSIGN && lhs()->postfixUnaryExpr() && lhs()->postfixUnaryExpr()->atomicExpr() &&
                       lhs()->postfixUnaryExpr()->atomicExpr()->fqIdentifier == RETURN_VARIABLE_NAME;
  *overrideUnreachable |= returns;
  return returns;
}

const CompileTimeValue &TernaryExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();

  const std::vector<LogicalOrExprNode *> &ops = operands();

  // If the condition has no compile time value, we do not need to evaluate the true and false values
  const LogicalOrExprNode *condition = ops[0];
  if (!condition->hasCompileTimeValue())
    return compileTimeValue;

  // Check if condition always evaluates to 'true'
  if (condition->getCompileTimeValue().boolValue) {
    const LogicalOrExprNode *trueValue = isShortened ? condition : ops[1];
    return trueValue->getCompileTimeValue();
  } else {
    const LogicalOrExprNode *falseValue = isShortened ? operands()[1] : ops[2];
    return falseValue->getCompileTimeValue();
  }
}

const CompileTimeValue &LogicalOrExprNode::getCompileTimeValue() const {
  if (children.size() == 1)
    return children.front()->getCompileTimeValue();

  // Check if one expression evaluates to 'true'
  const std::vector<LogicalAndExprNode *> &ops = operands();
  for (const LogicalAndExprNode *op : ops) {
    // If one of the operands does not have a value, the whole expression has no value
    if (!op->hasCompileTimeValue())
      return compileTimeValue;
    // If one operand evaluates to 'true' the whole expression is 'true'
    const CompileTimeValue &opCompileTimeValue = op->getCompileTimeValue();
    if (opCompileTimeValue.boolValue)
      return opCompileTimeValue;
  }
  // Return 'false'
  return ops.front()->getCompileTimeValue();
}

/**
 * Check if right above the closest assign expression ancestor is a statement node
 *
 * @return Has return value receiver or not
 */
bool FunctionCallNode::hasReturnValueReceiver() const {
  ASTNode *node = parent;
  while (!node->isAssignExpr()) {
    if (node->children.size() > 1)
      return true;
    node = node->parent;
  }
  return !node->parent->isStmtNode();
}

} // namespace spice::compiler