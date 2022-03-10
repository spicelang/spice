// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

/**
 * Factory class for managing program-wide organized threads and their ids
 */
class ThreadFactory {
public:
    // Constructors
    static ThreadFactory* getInstance();
    static void dropInstance();

    // Public methods
    int getNextFunctionSuffix();
private:
    // Private constructors
    explicit ThreadFactory() = default;

    // Members
    static ThreadFactory* instance;
    int nextFctSuffix = 0;
};
