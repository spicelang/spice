// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include "CustomHashFunctions.h"

#include <numeric>

namespace std {

size_t hash<spice::compiler::TypeChainElement>::operator()(const spice::compiler::TypeChainElement &tce) const noexcept {
  // Hasher for QualTypeList
  constexpr auto pred = [](const size_t acc, const spice::compiler::QualType &val) {
    // Combine the previous hash value with the current element's hash, adjusted by a prime number to reduce collisions
    return acc * 31 + std::hash<spice::compiler::QualType>{}(val);
  };
  // Hash all fields
  const size_t hashSuperType = std::hash<spice::compiler::SuperType>{}(tce.superType);
  const size_t hashSubType = std::hash<std::string>{}(tce.subType) << 1;
  const size_t hashTypeId = std::hash<uint64_t>{}(tce.typeId) << 2;
  const size_t hashData = std::hash<spice::compiler::Scope *>{}(tce.data.bodyScope) << 3;
  const size_t hashTemplateTypes = accumulate(tce.templateTypes.begin(), tce.templateTypes.end(), 0u, pred) << 4;
  const size_t hashParamTypes = accumulate(tce.paramTypes.begin(), tce.paramTypes.end(), 0u, pred) << 5;
  return hashSuperType ^ hashSubType ^ hashTypeId ^ hashData ^ hashTemplateTypes ^ hashParamTypes;
}

size_t hash<spice::compiler::Type>::operator()(const spice::compiler::Type &t) const noexcept {
  const auto pred = [](const size_t acc, const spice::compiler::TypeChainElement &val) {
    // Combine the previous hash value with the current element's hash, adjusted by a prime number to reduce collisions
    return acc * 31 + std::hash<spice::compiler::TypeChainElement>{}(val);
  };
  return accumulate(t.typeChain.begin(), t.typeChain.end(), 0u, pred);
}

size_t hash<spice::compiler::TypeQualifiers>::operator()(const spice::compiler::TypeQualifiers &qualifiers) const noexcept {
  const size_t hashConst = std::hash<bool>{}(qualifiers.isConst);
  const size_t hashSigned = std::hash<bool>{}(qualifiers.isSigned) << 1;
  const size_t hashUnsigned = std::hash<bool>{}(qualifiers.isUnsigned) << 2;
  const size_t hashHeap = std::hash<bool>{}(qualifiers.isHeap) << 3;
  const size_t hashPublic = std::hash<bool>{}(qualifiers.isPublic) << 4;
  const size_t hashInline = std::hash<bool>{}(qualifiers.isInline) << 5;
  const size_t hashComposition = std::hash<bool>{}(qualifiers.isComposition) << 6;
  return hashConst ^ hashSigned ^ hashUnsigned ^ hashHeap ^ hashPublic ^ hashInline ^ hashComposition;
}

size_t hash<spice::compiler::QualType>::operator()(const spice::compiler::QualType &qualType) const noexcept {
  const size_t hashType = std::hash<const spice::compiler::Type *>{}(qualType.getType());
  spice::compiler::TypeQualifiers qualifiers = qualType.getQualifiers();
  qualifiers.isPublic = false; // Ignore the public qualifier for hashing
  const size_t hashQualifiers = std::hash<spice::compiler::TypeQualifiers>{}(qualifiers) << 1;
  return hashType ^ hashQualifiers;
}

} // namespace std