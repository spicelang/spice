// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <vector>

namespace spice::compiler {

// Forward declarations
class ASTNode;

enum LifecycleState { DECLARED, INITIALIZED, MOVED, RETURNED, DEAD };

/**
 * A lifecycle event represents one change of a symbol in time.
 * It keeps track of the state of the symbol at that point of time and the event issuer AST node.
 */
struct LifecycleEvent {
  LifecycleState state;
  ASTNode *issuer;
};

/**
 * A lifecycle represents a collection of timely events that occur for a symbol.
 *
 * Usually a lifecycle looks e.g. like this:
 * - dead
 * - declared
 * - initialized
 * - moved
 * - returned
 * - moved
 * - returned
 * - dead
 */
class Lifecycle {
public:
  // Public methods
  void addEvent(const LifecycleEvent &event);
  [[nodiscard]] LifecycleState getCurrentState() const;
  [[nodiscard]] bool isDeclared() const;
  [[nodiscard]] bool isInitialized() const;
  [[nodiscard]] bool isMoved() const;
  [[nodiscard]] bool isReturned() const;
  [[nodiscard]] bool isDead() const;
  [[nodiscard]] bool isBorrowable() const;
  [[nodiscard]] const LifecycleEvent *getLastMoveEvent() const;

private:
  // Private members
  std::vector<LifecycleEvent> events;
};

} // namespace spice::compiler