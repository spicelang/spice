// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <functional>
#include <utility>

namespace spice::compiler {

/**
 * RAII helper that can run a lambda at the end of the scope
 **/
struct DeferredLogic {
  // Constructors
  explicit DeferredLogic(std::function<void()> deferredFunc, const bool triggerOnDestruct = true)
      : deferredFunc(std::move(deferredFunc)), triggerOnDestruct(triggerOnDestruct) {}

  // Destructor
  ~DeferredLogic() {
    if (triggerOnDestruct && !alreadyExecuted)
      deferredFunc();
  }

  // Public methods
  void execute() {
    deferredFunc();
    alreadyExecuted = true;
  }

private:
  // Private members
  std::function<void()> deferredFunc;
  bool triggerOnDestruct;
  bool alreadyExecuted = false;
};

} // namespace spice::compiler