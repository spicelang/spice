// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <utility>

#include <symbol/SymbolTableEntry.h>

#include "../../lib/json/json.hpp"

enum CaptureMode { READ_ONLY, READ_WRITE };

class Capture {
public:
  // Constructors
  explicit Capture(SymbolTableEntry *entry, std::string name = "", SymbolState state = INITIALIZED)
      : capturedEntry(entry), name(std::move(name)), state(state){};

  // Public methods
  [[nodiscard]] std::string getName() const;
  [[nodiscard]] SymbolState getState() const;
  void setCaptureMode(CaptureMode captureMode);
  [[nodiscard]] nlohmann::ordered_json toJSON() const;

  // Public members
  SymbolTableEntry *capturedEntry;

private:
  // Members
  CaptureMode mode = READ_ONLY;
  std::string name;
  SymbolState state;
};