// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "Capture.h"

/**
 * Retrieve the name of the capture
 *
 * @return Capture name or symbol name if no capture name was set
 */
std::string Capture::getName() const { return name.empty() ? capturedEntry->getName() : name; }

/**
 * Retrieve the captured symbol table entry
 *
 * @return Captured entry
 */
SymbolTableEntry *Capture::getEntry() const { return capturedEntry; }

/**
 * Retrieve the state of the capture
 *
 * @return Capture state
 */
SymbolState Capture::getState() const { return state; }

/**
 * Set the mode of this capture.
 * Possible values are READ_ONLY and READ_WRITE
 *
 * @param captureMode Capture mode
 */
void Capture::setCaptureMode(CaptureMode captureMode) {
  mode = captureMode;
  // Set the captured symbol table entry to volatile if appropriate
  capturedEntry->setVolatile(captureMode == READ_WRITE);
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
nlohmann::ordered_json Capture::toJSON() const { return capturedEntry->toJSON(); }