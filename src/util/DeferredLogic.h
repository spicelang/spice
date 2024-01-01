// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <functional>
#include <utility>

namespace spice::compiler {

struct DeferredLogic {
public:
  // Constructors
  explicit DeferredLogic(std::function<void()> deferredFunc, bool triggerOnDestruct = true)
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