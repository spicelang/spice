// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <symboltablebuilder/QualType.h>
#include <symboltablebuilder/Type.h>
#include <symboltablebuilder/TypeChain.h>

namespace std {

// Implement hash functionality for the TypeChainElement struct
template <> struct hash<spice::compiler::TypeChainElement> {
  size_t operator()(const spice::compiler::TypeChainElement &tce) const;
};

// Implement hash functionality for the Type class
template <> struct hash<spice::compiler::Type> {
  size_t operator()(const spice::compiler::Type &t) const;
};

// Implement hash functionality for the TypeSpecifiers class
template <> struct hash<spice::compiler::TypeSpecifiers> {
  size_t operator()(const spice::compiler::TypeSpecifiers &specifiers) const;
};

// Implement hash functionality for the QualType class
template <> struct hash<spice::compiler::QualType> {
  size_t operator()(const spice::compiler::QualType &qualType) const;
};

} // namespace std