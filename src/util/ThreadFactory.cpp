// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "ThreadFactory.h"

/**
 * Retrieve the next free function suffix
 *
 * @return Free function suffix
 */
int ThreadFactory::getNextFunctionSuffix() { return nextFctSuffix++; }

/**
 * Check if the program uses threads and we need to pass the -pthread flag to the linker
 *
 * @return Using threads or not
 */
bool ThreadFactory::isUsingThreads() const { return nextFctSuffix > 0; }