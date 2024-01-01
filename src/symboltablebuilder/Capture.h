// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <utility>

#include "SymbolTableEntry.h"

#include "../../lib/json/json.hpp"

namespace spice::compiler {

enum CaptureMode : uint8_t { BY_VALUE, BY_REFERENCE };
enum CaptureType : uint8_t { READ_ONLY, READ_WRITE };

class Capture {
public:
  // Constructors
  explicit Capture(SymbolTableEntry *entry, std::string name = "") : capturedEntry(entry), name(std::move(name)){};

  // Public methods
  [[nodiscard]] std::string getName() const;
  void setCaptureType(CaptureType captureType);
  [[nodiscard]] nlohmann::ordered_json toJSON() const;

  // Public members
  SymbolTableEntry *capturedEntry;

private:
  // Members
  CaptureType type = READ_ONLY;
  std::string name;
};

} // namespace spice::compiler