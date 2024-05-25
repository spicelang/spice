// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace spice::compiler {

// Forward declarations
class ASTNode;

enum LifecycleState : uint8_t {
  DEAD,
  DECLARED,
  INITIALIZED,
};

/**
 * A lifecycle event represents one change of a symbol in time.
 * It keeps track of the state of the symbol at that point of time and the event issuer AST node.
 */
struct LifecycleEvent {
  LifecycleState state;
  const ASTNode *issuer;
};

/**
 * A lifecycle represents a collection of timely events that occur for a symbol.
 *
 * Usually a lifecycle looks e.g. like this:
 * - dead
 * - declared
 * - initialized
 * - dead
 */
class Lifecycle {
public:
  // Public methods
  void addEvent(const LifecycleEvent &event);
  [[nodiscard]] LifecycleState getCurrentState() const;
  [[nodiscard]] const char *getCurrentStateName() const;
  [[nodiscard]] [[maybe_unused]] bool isDead() const;
  [[nodiscard]] [[maybe_unused]] bool isDeclared() const;
  [[nodiscard]] bool isInitialized() const;

private:
  // Private members
  std::vector<LifecycleEvent> events;
};

} // namespace spice::compiler