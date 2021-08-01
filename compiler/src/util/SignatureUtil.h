// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include <SpiceParser.h>
#include <analyzer/SymbolTableEntry.h>

class SignatureUtil {
public:
    static std::string getSignature(const std::string&, const std::vector<SymbolType>&);
};