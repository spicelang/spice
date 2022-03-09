// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <utility>

#include "SymbolTableEntry.h"

class Capture {
public:
    // Constructors
    explicit Capture(std::string name, SymbolTableEntry* capturedEntry): name(std::move(name)), capturedEntry(capturedEntry) {};

    // Public methods
    SymbolTableEntry* getEntry();
    void setAddress(llvm::Value*);
    nlohmann::ordered_json toJSON();
private:
    // Members
    std::string name;
    SymbolTableEntry* capturedEntry;
    llvm::Value* captureAddress = nullptr;
};