// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "ThreadFactory.h"

// Instance of the module registry
ThreadFactory* ThreadFactory::instance = nullptr;

/**
 * Retrieve an instance of the thread factory.
 *
 * @return Instance of the thread factory
 */
ThreadFactory* ThreadFactory::getInstance() {
    if (!instance) instance = new ThreadFactory();
    return instance;
}

/**
 * Retrieve the next free thread id
 *
 * @return Free tid
 */
int ThreadFactory::getNextThreadId() {
    return nextThreadId--;
}
