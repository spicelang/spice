// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <string>

#include <symboltablebuilder/SymbolType.h>

namespace spice::compiler {

// Forward declarations
class Function;
class Struct;
class Interface;

/**
 * Helper for name mangling.
 * This should be mostly compatible with the C++ Itanium ABI name mangling scheme.
 *
 * Names:
 * - P: pointer
 * - R: reference
 * - A: array
 * - d: double
 * - i: int
 * - j: unsigned int
 * - s: short
 * - t: unsigned short
 * - l: long
 * - m: unsigned long
 * - a: byte
 * - h: unsigned byte / unsigned char
 * - c: char
 * - Pc: string (char*)
 * - b: bool
 * - v: void
 * - K: const
 * - C: capturing
 *
 * Reserved:
 * - I
 * - E
 */
class NameMangling {
public:
  // Public methods
  [[nodiscard]] static std::string mangleFunction(const Function &spiceFunc);
  [[nodiscard]] static std::string mangleStruct(const Struct &spiceStruct);
  [[nodiscard]] static std::string mangleInterface(const Interface &spiceInterface);
  [[nodiscard]] static std::string mangleTypeInfoName(std::string &typeName);
  [[nodiscard]] static std::string mangleTypeInfo(const std::string &typeName);
  [[nodiscard]] static std::string mangleVTable(const std::string &typeName);

private:
  // Typedefs
  using TypeChainElement = SymbolType::TypeChainElement;

  // Private methods
  static void mangleType(std::stringstream &out, const SymbolType &type);
  static void mangleTypeChainElement(std::stringstream &out, const TypeChainElement &chainElement, bool signedness);
};

} // namespace spice::compiler