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
 * Retrieve the mode of this capture
 *
 * @return Capture mode (READ_ONLY or READ_WRITE)
 */
CaptureMode Capture::getCaptureMode() const {
    return mode;
}

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
nlohmann::ordered_json Capture::toJSON() const {
    return capturedEntry->toJSON();
}