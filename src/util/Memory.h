// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <atomic>
#include <cassert>
#include <cstdlib>
#include <new>

void *operator new(std::size_t n) noexcept(false) { return malloc(n); }

void operator delete(void *ptr) noexcept { free(ptr); }