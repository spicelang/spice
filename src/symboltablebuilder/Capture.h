// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include "SymbolTableEntry.h"

#include "../../lib/json/json.hpp"

namespace spice::compiler {

enum CapturePassMode : uint8_t {
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
  [[nodiscard]] CapturePassMode getMode() const;
  [[nodiscard]] nlohmann::ordered_json toJSON() const;

  // Public members
  SymbolTableEntry *capturedSymbol;

private:
  // Members
  CaptureAccessType accessType = READ_ONLY;
  CapturePassMode captureMode = BY_VALUE;
};

} // namespace spice::compiler