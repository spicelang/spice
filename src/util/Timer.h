// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <chrono>
#include <iostream>

struct Timer {
public:
  // Constructors
  Timer() = default;

  // Public methods
  void start() { timeStart = std::chrono::high_resolution_clock::now(); }
  void stop() { timeStop = std::chrono::high_resolution_clock::now(); }
  [[nodiscard]] long long getDurationMilliseconds() const {
    std::chrono::duration<double> duration = timeStop - timeStart;
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  }

private:
  std::chrono::time_point<std::chrono::system_clock> timeStart;
  std::chrono::time_point<std::chrono::system_clock> timeStop;
};