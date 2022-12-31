// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "Capture.h"

namespace spice::compiler {

/**
 * Retrieve the name of the capture
 *
 * @return Capture name or symbol name if no capture name was set
 */
std::string Capture::getName() const { return name.empty() ? capturedEntry->name : name; }

/**
 * Set the mode of this capture.
 * Possible values are READ_ONLY and READ_WRITE
 *
 * @param captureMode Capture mode
 */
void Capture::setCaptureMode(CaptureMode captureMode) {
  mode = captureMode;
  // Set the captured symbol table entry to volatile if appropriate
  capturedEntry->isVolatile = captureMode == READ_WRITE;
}

/**
 * Stringify the current capture to a human-readable form. Used to dump whole symbol tables with their contents.
 *
 * Example:
 * {
 *   "name": "testIdentifier"
 * }
 *
 * @return Capture as a JSON object
 */
nlohmann::ordered_json Capture::toJSON() const { return capturedEntry->toJSON(); }

} // namespace spice::compiler