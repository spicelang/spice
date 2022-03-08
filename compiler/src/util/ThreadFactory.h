// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

/**
 * Factory class for managing program-wide organized threads and their ids
 */
class ThreadFactory {
public:
    // Constructors
    static ThreadFactory* getInstance();

    // Public methods
    int getNextThreadId();
private:
    // Private constructors
    explicit ThreadFactory() = default;

    // Members
    static ThreadFactory* instance;
    int nextThreadId = -1;
};
