// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <utility>

#include "SymbolTableEntry.h"

class Capture {
public:
    // Constructors
    explicit Capture(SymbolTableEntry* entry): capturedEntry(entry) {};

    // Public methods
    [[nodiscard]] SymbolTableEntry* getEntry() const;
    [[nodiscard]] nlohmann::ordered_json toJSON() const;

private:
    // Members
    SymbolTableEntry* capturedEntry;
};