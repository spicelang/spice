// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include <ast/ASTNodes.h>

#include <symboltablebuilder/SymbolTableBuilder.h>

bool FctDefNode::returnsOnAllControlPaths() const { return body()->returnsOnAllControlPaths(); }

bool ForLoopNode::returnsOnAllControlPaths() const {
  const AssignExprNode *cond = condAssign();
  return cond->hasCompileTimeValue() && cond->getCompileTimeValue().boolValue;
}

bool WhileLoopNode::returnsOnAllControlPaths() const {
  const AssignExprNode *cond = condition();
  return cond->hasCompileTimeValue() && cond->getCompileTimeValue().boolValue;
}

bool IfStmtNode::returnsOnAllControlPaths() const {
  // An if statement returns on all control paths, if then and else block return on all control paths
  const AssignExprNode *cond = condition();
  if (!cond->hasCompileTimeValue())
    return thenBody()->returnsOnAllControlPaths() && elseStmt() != nullptr && elseStmt()->returnsOnAllControlPaths();

  // If the condition always evaluates to 'true' only the then block must return and vice versa
  const CompileTimeValue &compileTimeValue = cond->getCompileTimeValue();
  if (compileTimeValue.boolValue)
    return thenBody()->returnsOnAllControlPaths();
  return elseStmt() != nullptr && elseStmt()->returnsOnAllControlPaths();
}

bool ElseStmtNode::returnsOnAllControlPaths() const {
  return isElseIf ? ifStmt()->returnsOnAllControlPaths() : body()->returnsOnAllControlPaths();
}

bool ThreadDefNode::returnsOnAllControlPaths() const { return false; }

bool StmtLstNode::returnsOnAllControlPaths() const {
  // An empty statement list does not return at all
  if (children.empty())
    return false;
  // A statement list returns on all control paths, if the one statement returns on all control paths
  for (auto it = children.rbegin(); it != children.rend(); ++it) {
    const ASTNode *astNode = *it;
    if (astNode->returnsOnAllControlPaths())
      return true;
  }
  return false;
}

bool AssignExprNode::returnsOnAllControlPaths() const {
  return hasOperator && op == OP_ASSIGN && lhs()->postfixUnaryExpr()->atomicExpr()->identifier == RETURN_VARIABLE_NAME;
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