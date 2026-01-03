// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <symboltablebuilder/QualType.h>
#include <symboltablebuilder/Type.h>
#include <symboltablebuilder/TypeChain.h>

namespace spice::compiler {

uint64_t hashMix(uint64_t hash) noexcept;

void hashCombine64(uint64_t &hash, uint64_t value) noexcept;

template <typename T> uint64_t hashValue(const T &value) noexcept { return static_cast<uint64_t>(std::hash<T>{}(value)); }

template <typename T>
uint64_t hashPointer(const T *value) noexcept {
  return reinterpret_cast<uint64_t>(value) >> 3;
}

template <typename T> uint64_t hashVector(const std::vector<T> &vec) noexcept {
  uint64_t hash = 0;
  for (const T &item : vec)
    hashCombine64(hash, hashValue(item));
  return hash;
}

} // namespace spice::compiler

namespace std {

// Implement hash functionality for the TypeChainElement struct
template <> struct hash<spice::compiler::TypeChainElement> {
  size_t operator()(const spice::compiler::TypeChainElement &tce) const noexcept;
};

// Implement hash functionality for the Type class
template <> struct hash<spice::compiler::Type> {
  size_t operator()(const spice::compiler::Type &t) const noexcept;
};

// Implement hash functionality for the TypeQualifiers class
template <> struct hash<spice::compiler::TypeQualifiers> {
  size_t operator()(const spice::compiler::TypeQualifiers &qualifiers) const noexcept;
};

// Implement hash functionality for the QualType class
template <> struct hash<spice::compiler::QualType> {
  size_t operator()(const spice::compiler::QualType &qualType) const noexcept;
};

} // namespace std
