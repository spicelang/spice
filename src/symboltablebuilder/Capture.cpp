// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "Capture.h"

namespace spice::compiler {

/**
 * Retrieve the name of the capture
 *
 * @return Capture name or symbol name if no capture name was set
 */
std::string Capture::getName() const { return name.empty() ? capturedEntry->name : name; }

/**
 * Set the access type of this capture.
 * Possible values are READ_ONLY and READ_WRITE
 *
 * @param captureAccessType Capture access type
 */
void Capture::setAccessType(CaptureAccessType captureAccessType) {
  accessType = captureAccessType;
  // Set the captured symbol table entry to volatile if appropriate
  capturedEntry->isVolatile = captureAccessType == READ_WRITE;
}

/**
 * Retrieve the access type of this capture.
 * Possible values are READ_ONLY and READ_WRITE
 *
 * @return Capture access type
 */
CaptureAccessType Capture::getAccessType() const { return accessType; }

/**
 * Set the mode of this capture.
 * Possible values are BY_VALUE and BY_REFERENCE
 *
 * @param captureMode Capture mode
 */
void Capture::setMode(CaptureMode captureMode) { captureType = captureMode; }

/**
 * Retrieve the mode of this capture.
 * Possible values are BY_VALUE and BY_REFERENCE
 *
 * @return Capture mode
 */
CaptureMode Capture::getMode() const { return captureType; }

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