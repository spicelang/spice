// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "CustomHashFunctions.h"

#include <numeric>

namespace spice::compiler {

uint64_t hashMix(uint64_t hash) noexcept {
  hash += 0x9e3779b97f4a7c15ull;
  hash = (hash ^ (hash >> 30)) * 0xbf58476d1ce4e5b9ull;
  hash = (hash ^ (hash >> 27)) * 0x94d049bb133111ebull;
  hash ^= (hash >> 31);
  return hash;
}

void hashCombine64(uint64_t &hash, uint64_t value) noexcept {
  hash ^= hashMix(value + 0x9e3779b97f4a7c15ull + (hash << 6) + (hash >> 2));
}

} // namespace spice::compiler

namespace std {

size_t hash<spice::compiler::TypeChainElement>::operator()(const spice::compiler::TypeChainElement &tce) const noexcept {
  using namespace spice::compiler;
  uint64_t hash = 0;

  hashCombine64(hash, tce.superType);
  hashCombine64(hash, std::hash<std::string>{}(tce.subType));
  hashCombine64(hash, tce.typeId);

  switch (tce.superType) {
  case TY_ARRAY:
    hashCombine64(hash, tce.data.arraySize);
    break;
  case TY_FUNCTION:
  case TY_PROCEDURE:
    hashCombine64(hash, tce.data.hasCaptures ? 0xF00D1234ULL : 0xBAD0C0DEULL);
    break;
  case TY_STRUCT:
  case TY_INTERFACE:
  case TY_ENUM:
    // Stable hash based on pointer identity, but randomized for safety
    hashCombine64(hash, hashPointer(tce.data.bodyScope));
    break;
  default:
    break;
  }

  hashCombine64(hash, hashVector(tce.templateTypes));
  hashCombine64(hash, hashVector(tce.paramTypes));

  return hashMix(hash);
}

size_t hash<spice::compiler::Type>::operator()(const spice::compiler::Type &t) const noexcept {
  using namespace spice::compiler;
  uint64_t hash = 0;
  hashCombine64(hash, hashVector(t.typeChain));
  return hashMix(hash);
}

size_t hash<spice::compiler::TypeQualifiers>::operator()(const spice::compiler::TypeQualifiers &qualifiers) const noexcept {
  using namespace spice::compiler;
  const uint8_t bits = (qualifiers.isConst << 0) | (qualifiers.isSigned << 1) | (qualifiers.isUnsigned << 2) |
                       (qualifiers.isHeap << 3) | (qualifiers.isPublic << 4) | (qualifiers.isInline << 5) |
                       (qualifiers.isComposition << 6);
  return hashMix(bits);
}

size_t hash<spice::compiler::QualType>::operator()(const spice::compiler::QualType &qualType) const noexcept {
  using namespace spice::compiler;
  uint64_t seed = 0;

  // Hash type pointer content if possible
  hashCombine64(seed, std::hash<Type>{}(*qualType.getType()));

  TypeQualifiers qualifiers = qualType.getQualifiers();
  qualifiers.isPublic = false; // Ignore the public qualifier for hashing
  hashCombine64(seed, std::hash<TypeQualifiers>{}(qualifiers));
  return hashMix(seed);
}

} // namespace std
