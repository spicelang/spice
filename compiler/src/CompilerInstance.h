// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>

#include <symbol/SymbolTable.h>

/**
 * Represents the compilation process of a single source file to a LLVM module / object file
 */
class CompilerInstance {
public:
    static SymbolTable* CompileSourceFile(std::string&, const std::string&, const std::string&, const std::string&,
                                          const std::string&, bool, int, bool, bool);
};
