// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <vector>
#include <tuple>

#include <symbol/SymbolTable.h>

class ScopePath {
public:
    explicit ScopePath() {};
    void pushFragment(const std::string& fragment, SymbolTable* symbolTable);
    void clear();
    SymbolTable* getCurrentScope();
    std::string getScopeName();
private:
    std::vector<std::pair<std::string, SymbolTable*>> fragments;
};