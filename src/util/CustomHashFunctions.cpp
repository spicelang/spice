// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "CustomHashFunctions.h"

#include <numeric>

namespace spice::compiler {

uint64_t hash_combine64(uint64_t seed, uint64_t v) { return seed ^ v + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2); }

} // namespace spice::compiler

namespace std {

size_t hash<spice::compiler::TypeChainElement>::operator()(const spice::compiler::TypeChainElement &tce) const noexcept {
  uint64_t acc = spice::compiler::HASH_INIT_VAL;
  acc = spice::compiler::hash_combine64(acc, std::hash<spice::compiler::SuperType>{}(tce.superType));
  acc = spice::compiler::hash_combine64(acc, std::hash<std::string>{}(tce.subType));
  acc = spice::compiler::hash_combine64(acc, std::hash<uint64_t>{}(tce.typeId));
  acc = spice::compiler::hash_combine64(acc, std::hash<spice::compiler::Scope *>{}(tce.data.bodyScope));
  for (const auto &templateType : tce.templateTypes)
    acc = spice::compiler::hash_combine64(acc, std::hash<spice::compiler::QualType>{}(templateType));
  for (const auto &paramType : tce.paramTypes)
    acc = spice::compiler::hash_combine64(acc, std::hash<spice::compiler::QualType>{}(paramType));
  return acc;
}

size_t hash<spice::compiler::Type>::operator()(const spice::compiler::Type &t) const noexcept {
  uint64_t acc = spice::compiler::HASH_INIT_VAL;
  for (const auto &typeChainElement : t.typeChain)
    acc = spice::compiler::hash_combine64(acc, std::hash<spice::compiler::TypeChainElement>{}(typeChainElement));
  return acc;
}

size_t hash<spice::compiler::TypeQualifiers>::operator()(const spice::compiler::TypeQualifiers &qualifiers) const noexcept {
  uint64_t acc = spice::compiler::HASH_INIT_VAL;
  acc = spice::compiler::hash_combine64(acc, std::hash<bool>{}(qualifiers.isConst));
  acc = spice::compiler::hash_combine64(acc, std::hash<bool>{}(qualifiers.isSigned));
  acc = spice::compiler::hash_combine64(acc, std::hash<bool>{}(qualifiers.isUnsigned));
  acc = spice::compiler::hash_combine64(acc, std::hash<bool>{}(qualifiers.isHeap));
  acc = spice::compiler::hash_combine64(acc, std::hash<bool>{}(qualifiers.isPublic));
  acc = spice::compiler::hash_combine64(acc, std::hash<bool>{}(qualifiers.isInline));
  acc = spice::compiler::hash_combine64(acc, std::hash<bool>{}(qualifiers.isComposition));
  return acc;
}

size_t hash<spice::compiler::QualType>::operator()(const spice::compiler::QualType &qualType) const noexcept {
  uint64_t acc = spice::compiler::HASH_INIT_VAL;
  acc = spice::compiler::hash_combine64(acc, std::hash<const spice::compiler::Type *>{}(qualType.getType()));
  spice::compiler::TypeQualifiers qualifiers = qualType.getQualifiers();
  qualifiers.isPublic = false; // Ignore the public qualifier for hashing
  acc = spice::compiler::hash_combine64(acc, std::hash<spice::compiler::TypeQualifiers>{}(qualifiers));
  return acc;
}

} // namespace std
