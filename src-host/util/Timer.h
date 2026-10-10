// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <chrono>

namespace spice::compiler {

class Timer {
public:
  // Constructors
  explicit Timer(uint64_t *const timerOutput = nullptr) : timerOutput(timerOutput) {}

  // Public methods
  void start() {
    if (timerOutput)
      *timerOutput = 0;
    resume();
  }

  void stop() { pause(); }

  void resume() { timeStart = std::chrono::high_resolution_clock::now(); }

  void pause() {
    timeStop = std::chrono::high_resolution_clock::now();
    if (timerOutput)
      *timerOutput += getDurationMilliseconds();
  }

  [[nodiscard]] uint64_t getDurationMilliseconds() const {
    const auto duration = timeStop - timeStart;
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  }

private:
  uint64_t *const timerOutput;
  std::chrono::time_point<std::chrono::high_resolution_clock> timeStart;
  std::chrono::time_point<std::chrono::high_resolution_clock> timeStop;
};

} // namespace spice::compiler