// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "Capture.h"

/**
 * Retrieve the captured symbol table entry
 *
 * @return Captured entry
 */
SymbolTableEntry* Capture::getEntry() const {
    return capturedEntry;
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
nlohmann::ordered_json Capture::toJSON() const {
    return capturedEntry->toJSON();
}