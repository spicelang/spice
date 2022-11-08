// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <chrono>
#include <iostream>

struct Timer {
public:
  Timer() { start = std::chrono::high_resolution_clock::now(); }
  ~Timer() {
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    double durationSeconds = duration.count() * 1000000;
    std::cout << durationSeconds << std::endl;
  }

private:
  std::chrono::time_point<std::chrono::system_clock> start;
};