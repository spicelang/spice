// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <utility>

#include "SymbolTableEntry.h"

#include "../../lib/json/json.hpp"

namespace spice::compiler {

enum CaptureMode : uint8_t {
  BY_VALUE,
  BY_REFERENCE,
};

enum CaptureAccessType : uint8_t {
  READ_ONLY,
  READ_WRITE,
};

class Capture {
public:
  // Constructors
  explicit Capture(SymbolTableEntry *entry);

  // Public methods
  [[nodiscard]] std::string getName() const;
  void setAccessType(CaptureAccessType captureAccessType);
  [[nodiscard]] CaptureAccessType getAccessType() const;
  void setMode(CaptureMode captureMode);
  [[nodiscard]] CaptureMode getMode() const;
  [[nodiscard]] nlohmann::ordered_json toJSON() const;

  // Public members
  SymbolTableEntry *capturedEntry;

private:
  // Members
  CaptureAccessType accessType = READ_ONLY;
  CaptureMode captureMode = BY_VALUE;
};

} // namespace spice::compiler