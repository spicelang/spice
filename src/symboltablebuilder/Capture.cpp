// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "Capture.h"

#include <symboltablebuilder/TypeChain.h>

namespace spice::compiler {

Capture::Capture(SymbolTableEntry *entry) : capturedSymbol(entry) {
  // Set the capture mode depending on the symbol type
  // All types with guaranteed size <= 64 bit are captured by value, all others by reference.
  captureMode = entry->getQualType().isOneOf({TY_STRUCT, TY_INTERFACE}) ? BY_REFERENCE : BY_VALUE;
}

/**
 * Retrieve the name of the capture
 *
 * @return Capture name or symbol name if no capture name was set
 */
std::string Capture::getName() const { return capturedSymbol->name; }

/**
 * Set the access type of this capture.
 * Possible values are READ_ONLY and READ_WRITE
 *
 * @param captureAccessType Capture access type
 */
void Capture::setAccessType(CaptureAccessType captureAccessType) {
  accessType = captureAccessType;
  // If we write to the captured symbol, we need to set the symbol to be a reference
  if (captureAccessType == READ_WRITE)
    captureMode = BY_REFERENCE;
}

/**
 * Retrieve the mode of this capture.
 * Possible values are BY_VALUE and BY_REFERENCE
 *
 * @return Capture mode
 */
CapturePassMode Capture::getMode() const { return captureMode; }

/**
 * Stringify the current capture to a human-readable form. Used to dump whole symbol tables with their contents.
 *
 * Example:
 * {
 *   "name": "testIdentifier",
 *   "accessType": "READ_ONLY",
 *   "mode": "BY_VALUE"
 * }
 *
 * @return Capture as a JSON object
 */
nlohmann::ordered_json Capture::toJSON() const {
  nlohmann::json result;
  result["name"] = capturedSymbol->name;
  result["accessType"] = accessType == READ_ONLY ? "READ_ONLY" : "READ_WRITE";
  result["mode"] = captureMode == BY_VALUE ? "BY_VALUE" : "BY_REFERENCE";
  return result;
}

} // namespace spice::compiler