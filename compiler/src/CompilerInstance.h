// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include <string>

#include <SpiceLexer.h>
#include <SpiceParser.h>

#include <util/FileUtil.h>
#include <analyzer/SymbolTable.h>
#include <analyzer/AnalyzerVisitor.h>
#include <generator/GeneratorVisitor.h>

/**
 * Represents the compilation process of a single source file
 */
class CompilerInstance {
public:
    static SymbolTable* CompileSourceFile(std::string&, const std::string&, const std::string&, const std::string&,
                                          const std::string&, bool, int, bool, bool);
};