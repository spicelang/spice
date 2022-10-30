// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "Lifecycle.h"

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
LifecycleState Lifecycle::getCurrentState() const { return events.back().state; }

/**
 * Check if the symbol is declared
 *
 * @return Declared or not
 */
bool Lifecycle::isDeclared() const { return events.back().state == DECLARED; }

/**
 * Check if the symbol is initialized
 *
 * @return Initialized or not
 */
bool Lifecycle::isInitialized() const { return events.back().state == INITIALIZED; }

/**
 * Check if the value of the symbol was moved away
 *
 * @return Moved or not
 */
bool Lifecycle::isMoved() const { return events.back().state == MOVED; }

/**
 * Check if the value of the symbol has returned after being moved away
 *
 * @return Returned or not
 */
bool Lifecycle::isReturned() const { return events.back().state == RETURNED; }

/**
 * Check if the symbol is dead
 *
 * @return Dead or not
 */
bool Lifecycle::isDead() const { return events.back().state == DEAD; }

/**
 * Check if the symbol is borrowable
 *
 * @return Borrowable or not
 */
bool Lifecycle::isBorrowable() const { return isInitialized() || isReturned(); }

/**
 * Retrieve the last occurred move event. Is used to find the side effector of a borrow failure.
 *
 * @return Last move event
 */
const LifecycleEvent *Lifecycle::getLastMoveEvent() const {
  for (auto it = events.rbegin(); it != events.rend(); it++) {
    if (it->state == MOVED)
      return &(*it);
  }
  return nullptr;
}
