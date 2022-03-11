// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "ThreadFactory.h"

/**
 * Retrieve the next free function suffix
 *
 * @return Free function suffix
 */
int ThreadFactory::getNextFunctionSuffix() {
    return nextFctSuffix++;
}
