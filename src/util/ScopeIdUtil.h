// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>

#include <SpiceParser.h>

/**
 * Util class to generate scope identifiers for different AST nodes
 */
class ScopeIdUtil {
public:
    static std::string getScopeId(SpiceParser::MainFunctionDefContext* ctx);
    static std::string getScopeId(SpiceParser::FunctionDefContext* ctx);
    static std::string getScopeId(SpiceParser::StructDefContext* ctx);
    static std::string getScopeId(SpiceParser::ThreadDefContext* ctx);
    static std::string getScopeId(SpiceParser::ProcedureDefContext* ctx);
    static std::string getScopeId(SpiceParser::ForLoopContext* ctx);
    static std::string getScopeId(SpiceParser::ForeachLoopContext* ctx);
    static std::string getScopeId(SpiceParser::WhileLoopContext* ctx);
    static std::string getScopeId(SpiceParser::IfStmtContext* ctx);
    static std::string getScopeId(SpiceParser::ElseStmtContext* ctx);
};