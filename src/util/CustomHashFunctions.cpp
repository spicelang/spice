// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "CustomHashFunctions.h"

#include <numeric>

namespace spice::compiler {

uint64_t mix64(uint64_t x) {
  x ^= x >> 30;
  x *= 0xbf58476d1ce4e5b9ULL;
  x ^= x >> 27;
  x *= 0x94d049bb133111ebULL;
  x ^= x >> 31;
  return x;
}

uint64_t hashCombine64(uint64_t seed, uint64_t v) { return mix64(seed ^ mix64(v + 0x9e3779b97f4a7c15ULL)); }

} // namespace spice::compiler

namespace std {

size_t hash<spice::compiler::TypeChainElement>::operator()(const spice::compiler::TypeChainElement &tce) const noexcept {
  uint64_t acc = spice::compiler::HASH_INIT_VAL;
  acc = spice::compiler::hashCombine64(acc, std::hash<spice::compiler::SuperType>{}(tce.superType));
  acc = spice::compiler::hashCombine64(acc, std::hash<std::string>{}(tce.subType));
  acc = spice::compiler::hashCombine64(acc, std::hash<uint64_t>{}(tce.typeId));
  acc = spice::compiler::hashCombine64(acc, std::hash<spice::compiler::Scope *>{}(tce.data.bodyScope));
  for (const auto &templateType : tce.templateTypes)
    acc = spice::compiler::hashCombine64(acc, std::hash<spice::compiler::QualType>{}(templateType));
  for (const auto &paramType : tce.paramTypes)
    acc = spice::compiler::hashCombine64(acc, std::hash<spice::compiler::QualType>{}(paramType));
  return acc;
}

size_t hash<spice::compiler::Type>::operator()(const spice::compiler::Type &t) const noexcept {
  uint64_t acc = spice::compiler::HASH_INIT_VAL;
  for (const auto &typeChainElement : t.typeChain)
    acc = spice::compiler::hashCombine64(acc, std::hash<spice::compiler::TypeChainElement>{}(typeChainElement));
  return acc;
}

size_t hash<spice::compiler::TypeQualifiers>::operator()(const spice::compiler::TypeQualifiers &qualifiers) const noexcept {
  uint8_t bits = 0;
  bits |= qualifiers.isConst << 0;
  bits |= qualifiers.isSigned << 1;
  bits |= qualifiers.isUnsigned << 2;
  bits |= qualifiers.isHeap << 3;
  bits |= qualifiers.isPublic << 4;
  bits |= qualifiers.isInline << 5;
  bits |= qualifiers.isComposition << 6;
  return spice::compiler::hashCombine64(spice::compiler::HASH_INIT_VAL, std::hash<uint8_t>{}(bits));
}

size_t hash<spice::compiler::QualType>::operator()(const spice::compiler::QualType &qualType) const noexcept {
  uint64_t acc = spice::compiler::HASH_INIT_VAL;
  acc = spice::compiler::hashCombine64(acc, std::hash<const spice::compiler::Type *>{}(qualType.getType()));
  spice::compiler::TypeQualifiers qualifiers = qualType.getQualifiers();
  qualifiers.isPublic = false; // Ignore the public qualifier for hashing
  acc = spice::compiler::hashCombine64(acc, std::hash<spice::compiler::TypeQualifiers>{}(qualifiers));
  return acc;
}

} // namespace std
