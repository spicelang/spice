// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <vector>
#include <stack>

#include <exception/ErrorFactory.h>

enum SymbolSuperType {
    TY_INVALID,
    TY_DOUBLE,
    TY_INT,
    TY_SHORT,
    TY_LONG,
    TY_BYTE,
    TY_CHAR,
    TY_STRING,
    TY_BOOL,
    TY_STRUCT,
    TY_DYN,
    TY_PTR,
    TY_ARRAY,
    TY_FUNCTION,
    TY_PROCEDURE,
    TY_IMPORT
};

typedef std::pair<SymbolSuperType, std::string> TypeChainElement;
typedef std::stack<TypeChainElement> TypeChain;

class SymbolType {
public:
    // Constructors
    explicit SymbolType(SymbolSuperType superType) : typeChain({ std::make_pair(superType, "") }) {}
    SymbolType(SymbolSuperType superType, const std::string& subType) : typeChain({ std::make_pair(superType, subType) }) {}
    explicit SymbolType(TypeChain types) : typeChain(std::move(types)) {}
    SymbolType() = default;

    // Public methods
    SymbolType toPointer(const ErrorFactory* err, const antlr4::Token& token);
    SymbolType toArray(const ErrorFactory* err, const antlr4::Token& token, unsigned int size = 0);
    [[nodiscard]] SymbolType getContainedTy() const;
    SymbolType replaceSubType(const std::string& newSubType);
    [[nodiscard]] bool isPointer() const;
    [[nodiscard]] bool isPointerOf(SymbolSuperType superType) const;
    [[nodiscard]] bool isArray() const;
    [[nodiscard]] bool isArrayOf(SymbolSuperType superType) const;
    [[nodiscard]] bool isArrayOf(const SymbolType& symbolType) const;
    [[nodiscard]] bool is(SymbolSuperType superType) const;
    [[nodiscard]] bool is(SymbolSuperType superType, const std::string& subType) const;
    [[nodiscard]] bool isPrimitive() const;
    [[nodiscard]] bool isBaseType(SymbolSuperType superType) const;
    [[nodiscard]] bool isOneOf(const std::vector<SymbolSuperType>& superTypes) const;
    [[nodiscard]] SymbolSuperType getSuperType() const;
    [[nodiscard]] std::string getSubType() const;
    [[nodiscard]] SymbolType getBaseType() const;
    [[nodiscard]] std::string getName(bool withSize) const;
    [[nodiscard]] unsigned int getArraySize() const;
    friend bool operator== (const SymbolType& lhs, const SymbolType& rhs);
    friend bool operator!= (const SymbolType& lhs, const SymbolType& rhs);
private:
    // Members
    TypeChain typeChain;

    // Private methods
    [[nodiscard]] std::string getNameFromChainElement(const TypeChainElement& chainElement, bool withSize) const;
};