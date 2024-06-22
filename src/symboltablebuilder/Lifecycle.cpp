// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "Lifecycle.h"

namespace spice::compiler {

/**
 * Add an event to the lifecycle
 *
 * @param event New lifecycle event
 */
void Lifecycle::addEvent(const LifecycleEvent &event) { events.push_back(event); }

/**
 * Retrieve the current state of a symbol
 *
 * @return Current state
 */
LifecycleState Lifecycle::getCurrentState() const { return events.empty() ? DEAD : events.back().state; }

/**
 * Retrieve the name of the  current state of a symbol
 *
 * @return Current state name
 */
const char *Lifecycle::getCurrentStateName() const {
  switch (getCurrentState()) {
  case DECLARED:
    return "declared";
  case INITIALIZED:
    return "initialized";
  case MOVED:
    return "moved";
  default:
    return "dead";
  }
}

/**
 * Check if the symbol is dead
 *
 * @return Dead or not
 */
bool Lifecycle::isDead() const { return getCurrentState() == DEAD; }

/**
 * Check if the symbol is declared
 *
 * @return Declared or not
 */
bool Lifecycle::isDeclared() const { return getCurrentState() == DECLARED; }

/**
 * Check if the symbol is initialized
 *
 * @return Initialized or not
 */
bool Lifecycle::isInitialized() const { return getCurrentState() == INITIALIZED; }

/**
 * Check if the symbol was moved
 *
 * @return Moved or not
 */
bool Lifecycle::wasMoved() const { return getCurrentState() == MOVED; }

/**
 * Check if the symbol is in an owning state
 *
 * @return Owning state or not
 */
bool Lifecycle::isInOwningState() const { return isDeclared() || isInitialized(); }

} // namespace spice::compiler