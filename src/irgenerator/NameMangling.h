// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <model/GenericType.h>
#include <symboltablebuilder/Type.h>

namespace spice::compiler {

// Forward declarations
class Function;
class StructBase;
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
 * - PKc: string (const char*)
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
  [[nodiscard]] static std::string mangleTypeInfoName(const StructBase *structBase);
  [[nodiscard]] static std::string mangleTypeInfoValue(const std::string &value);
  [[nodiscard]] static std::string mangleTypeInfo(const StructBase *structBase);
  [[nodiscard]] static std::string mangleVTable(const StructBase *structBase);
  [[nodiscard]] static std::string mangleVTable(const std::string &typeName);

private:
  // Private methods
  static void mangleName(std::stringstream &out, const std::string &name, bool &nestedType);
  static void mangleType(std::stringstream &out, QualType qualType);
  static void mangleTypeChainElement(std::stringstream &out, const TypeChainElement &chainElement, bool signedness);
};

} // namespace spice::compiler