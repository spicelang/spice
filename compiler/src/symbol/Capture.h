// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <utility>

#include "SymbolTableEntry.h"

class Capture {
public:
    // Constructors
    explicit Capture(SymbolTableEntry* entry): capturedEntry(entry) {};

    // Public methods
    SymbolTableEntry* getEntry();
    nlohmann::ordered_json toJSON();

private:
    // Members
    SymbolTableEntry* capturedEntry;
};