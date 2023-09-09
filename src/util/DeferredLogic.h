// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <functional>
#include <utility>

namespace spice::compiler {

struct DeferredLogic {
public:
  // Constructors
  explicit DeferredLogic(std::function<void()> deferredFunc) : deferredFunc(std::move(deferredFunc)) {}

  // Destructor
  ~DeferredLogic() {
    if (!alreadyExecuted)
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
  bool alreadyExecuted = false;
};

} // namespace spice::compiler