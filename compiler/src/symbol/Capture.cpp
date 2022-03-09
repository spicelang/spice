// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "Capture.h"

/**
 * Retrieve captured symbol table entry.
 *
 * @return Symbol table entry
 */
SymbolTableEntry* Capture::getEntry() {
    return capturedEntry;
}

/**
 * Retrieve capture address. This is the address which is available in the nested function.
 *
 * @param address Memory address
 */
void Capture::setAddress(llvm::Value* address) {
    captureAddress = address;
}

/**
 * Stringify the current capture to a human-readable form. Used to dump whole symbol tables with their contents.
 *
 * Example:
 * {
 *   "name": "testIden"
 * }
 *
 * @return Capture as a JSON object
 */
nlohmann::ordered_json Capture::toJSON() {
    nlohmann::json result;
    result["name"] = name;
    return result;
}
