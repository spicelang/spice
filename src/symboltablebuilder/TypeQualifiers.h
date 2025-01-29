// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <cstdint>

namespace spice::compiler {

// Bit indices from right to left
static constexpr uint8_t BIT_INDEX_COMPOSITION = 0;
static constexpr uint8_t BIT_INDEX_INLINE = 1;
static constexpr uint8_t BIT_INDEX_PUBLIC = 2;
static constexpr uint8_t BIT_INDEX_HEAP = 3;
static constexpr uint8_t BIT_INDEX_UNSIGNED = 4;
static constexpr uint8_t BIT_INDEX_SIGNED = 5;
static constexpr uint8_t BIT_INDEX_CONST = 6;
static constexpr uint8_t BIT_INDEX_MAX = BIT_INDEX_CONST; // Please adjust if something changes above

class TypeQualifiers {
public:
  // Constructors
  TypeQualifiers() = default;
  TypeQualifiers(bool isConst, bool isSigned, bool isUnsigned) : isConst(isConst), isSigned(isSigned), isUnsigned(isUnsigned) {}

  // Public static methods
  static TypeQualifiers of(uint16_t superType);

  // Public methods
  [[nodiscard]] TypeQualifiers merge(const TypeQualifiers &other) const;
  [[nodiscard]] bool match(TypeQualifiers other, bool allowConstify) const;
  void eraseWithMask(const TypeQualifiers &mask);

  // Overloaded operators
  friend bool operator==(const TypeQualifiers &lhs, const TypeQualifiers &rhs);

  // public members
  // Note: Please adjust bit indices above, if something changes here
  bool isConst : 1 = false;
  bool isSigned : 1 = false;
  bool isUnsigned : 1 = false;
  bool isHeap : 1 = false;
  bool isPublic : 1 = false;
  bool isInline : 1 = false;
  bool isComposition : 1 = false;

private:
  // Private methods
  [[nodiscard]] bool getBit(uint8_t index) const;
  bool setBit(uint8_t index, bool value);
};

} // namespace spice::compiler