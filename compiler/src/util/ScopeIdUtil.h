// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>

#include <SpiceParser.h>

/**
 * Util class to generate scope identifiers for different AST nodes
 */
class ScopeIdUtil {
public:
    static std::string getScopeId(SpiceParser::MainFunctionDefContext*);

    static std::string getScopeId(SpiceParser::FunctionDefContext*);

    static std::string getScopeId(SpiceParser::StructDefContext*);

    static std::string getScopeId(SpiceParser::ProcedureDefContext*);

    static std::string getScopeId(SpiceParser::ForLoopContext*);

    static std::string getScopeId(SpiceParser::ForeachLoopContext*);

    static std::string getScopeId(SpiceParser::WhileLoopContext*);

    static std::string getScopeId(SpiceParser::IfStmtContext*);

    static std::string getScopeId(SpiceParser::ElseStmtContext*);
};