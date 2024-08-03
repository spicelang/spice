// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

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

class TypeSpecifiers {
public:
  // Constructors
  TypeSpecifiers() = default;
  TypeSpecifiers(bool isConst, bool isSigned, bool isUnsigned, bool isHeap)
      : isConst(isConst), isSigned(isSigned), isUnsigned(isUnsigned), isHeap(isHeap) {}

  // Public static methods
  static TypeSpecifiers of(uint16_t superType);

  // Public methods
  [[nodiscard]] TypeSpecifiers merge(const TypeSpecifiers &other) const;
  [[nodiscard]] bool match(TypeSpecifiers other, bool allowConstify) const;
  void eraseWithMask(const TypeSpecifiers &mask);

  // Overloaded operators
  friend bool operator==(const TypeSpecifiers &lhs, const TypeSpecifiers &rhs);

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