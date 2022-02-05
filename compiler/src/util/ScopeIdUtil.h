// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>

#include <SpiceParser.h>

/**
 * Util class to generate scope identifiers for different AST nodes
 */
class ScopeIdUtil {
public:
    static std::string getScopeId(SpiceParser::MainFunctionDefContext* context);

    static std::string getScopeId(SpiceParser::FunctionDefContext* context);

    static std::string getScopeId(SpiceParser::StructDefContext* context);

    static std::string getScopeId(SpiceParser::ProcedureDefContext* context);

    static std::string getScopeId(SpiceParser::ForLoopContext* context);

    static std::string getScopeId(SpiceParser::ForeachLoopContext* context);

    static std::string getScopeId(SpiceParser::WhileLoopContext* context);

    static std::string getScopeId(SpiceParser::IfStmtContext* context);

    static std::string getScopeId(SpiceParser::ElseStmtContext* context);
};