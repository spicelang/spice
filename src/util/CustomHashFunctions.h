// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#pragma once

#include <symboltablebuilder/QualType.h>
#include <symboltablebuilder/Type.h>
#include <symboltablebuilder/TypeChain.h>

namespace spice::compiler {

constexpr uint64_t HASH_INIT_VAL = 0x243F6A8885A308D3ull;

uint64_t hashCombine64(uint64_t seed, uint64_t v);

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
