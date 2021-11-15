// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "SymbolTable.h"

/**
 * Insert a new symbol into the current symbol table. If it is a parameter, append its name to the paramNames vector
 *
 * @param name Name of the symbol
 * @param type Type of the symbol
 * @param state State of the symbol (declared or initialized)
 * @param isConstant Enabled if the symbol is a constant
 * @param isParameter Enabled if the symbol is a function/procedure parameter
 */
void SymbolTable::insert(const std::string& name, SymbolType type, SymbolState state, bool isConstant, bool isParameter) {
    symbols.insert({name, SymbolTableEntry(name, type, state, isConstant)});
    if (isParameter) paramNames.push_back(name);
}

/**
 * Check if a symbol exists in the current or any parent scope and return it if possible
 *
 * @param name Name of the desired symbol
 * @return Desired symbol / nullptr if the symbol was not round
 */
SymbolTableEntry* SymbolTable::lookup(const std::string& name) {
    // If not available in the current scope, search in the parent scope
    if (symbols.find(name) == symbols.end()) {
        if (parent == nullptr) return nullptr;
        return parent->lookup(name);
    }
    // Otherwise, return the entry
    return &symbols.at(name);
}

/**
 * Search for a symbol table by its name, where a function is defined. Used for function calls to function/procedures
 * which were linked in from other modules
 *
 * @param nameSpace Name of the scope of the desired symbol table
 * @return Desired symbol table
 */
SymbolTable* SymbolTable::lookupTable(const std::vector<std::string>& nameSpace) {
    // Check if scope contains this namespace
    SymbolTable* currentTable = this;
    for (int i = 0; i < nameSpace.size(); i++) {
        if (i == nameSpace.size() - 1) {
            if (currentTable->symbols.find(nameSpace[i]) == currentTable->symbols.end()) break;
            return currentTable;
        } else {
            if (currentTable->children.find(nameSpace[i]) == currentTable->children.end()) break;
            currentTable = &children.at(nameSpace[i]);
        }
    }
    // Current scope does not contain the namespace => go up one table
    if (parent == nullptr) return nullptr;
    return parent->lookupTable(nameSpace);
}

/**
 * Update the state of a symbol in the current symbol table or a parent scope.
 *
 * @throws runtime_error When trying to update a non-existent symbol
 * @param name Name of the symbol to update
 * @param newState New state of the symbol to update
 */
void SymbolTable::update(const std::string& name, SymbolState newState) {
    // If not available in the current scope, search in the parent scope
    if (symbols.find(name) == symbols.end()) {
        if (parent == nullptr) throw std::runtime_error("Updating a non-existent symbol: " + name);
        parent->update(name, newState);
    }
    // Otherwise, update the entry
    symbols.at(name).updateState(newState);
}

/**
 * Update the type of a symbol in the current symbol table or a parent scope. This is used for type inference of
 * dyn variables
 *
 * @param name Name of the symbol to update
 * @param newType New type of the symbol to update
 */
void SymbolTable::update(const std::string& name, SymbolType newType) {
    // If not available in the current scope, search in the parent scope
    if (symbols.find(name) == symbols.end()) {
        if (parent == nullptr) throw std::runtime_error("Updating a non-existent symbol: " + name);
        parent->update(name, newType);
    }
    // Otherwise, update the entry
    symbols.at(name).updateType(newType);
}

/**
 * Create a child leaf for the tree of symbol tables and return it
 *
 * @param blockName Name of the child scope
 * @return Newly created child table
 */
SymbolTable* SymbolTable::createChildBlock(const std::string& blockName) {
    children.insert({blockName, SymbolTable(this)});
    return &children.at(blockName);
}

/**
 * Mount in symbol tables manually. This is used to hook in symbol tables of imported modules into the symbol table of
 * the source file, which imported the modules
 *
 * @param blockName Name of the child scope
 * @param childBlock Child symbol table
 */
void SymbolTable::mountChildBlock(const std::string& blockName, SymbolTable* childBlock) {
    children.insert({blockName, *childBlock});
}

/**
 * Rename the scope of a symbol table. This is useful for realizing function overloading by storing a function with not
 * only its name, but also its signature
 *
 * @param oldName Old name of the child table
 * @param newName New name of the child table
 */
void SymbolTable::renameChildBlock(const std::string& oldName, const std::string& newName) {
    auto nodeHandler = children.extract(oldName);
    nodeHandler.key() = newName;
    children.insert(std::move(nodeHandler));
}

/**
 * Navigate to parent table of the current one in the tree structure
 *
 * @return Pointer to the parent symbol table
 */
SymbolTable* SymbolTable::getParent() {
    return parent;
}

/**
 * Navigate to a child table of the current one in the tree structure
 *
 * @param scopeId Name of the child scope
 * @return Pointer to the child symbol table
 */
SymbolTable* SymbolTable::getChild(const std::string& scopeId) {
    if (children.empty()) return nullptr;
    return &children.at(scopeId);
}

/**
 * Insert an item to the list of function declarations. This list is used to link in functions from other modules and
 * therefore not storing their definition, but their declaration.
 *
 * @param signature Signature of the function declaration
 * @param types List of parameter types of the function declaration
 */
void SymbolTable::insertFunctionDeclaration(const std::string& signature, const std::vector<SymbolType>& types) {
    functionDeclarations.insert({signature, types});
}

/**
 * Retrieve an item from the list of function declarations.
 *
 * @param signature Signature of the desired function declaration
 * @return List of parameter types of the desired function declaration
 */
std::vector<SymbolType> SymbolTable::getFunctionDeclaration(const std::string& signature) {
    if (functionDeclarations.find(signature) == functionDeclarations.end()) return {};
    return functionDeclarations.at(signature);
}

/**
 * Insert an item to the list of procedure declarations. This list is used to link in procedures from other modules and
 * therefore not storing their definition, but their declaration.
 *
 * @param signature Signature of the procedure declaration
 * @param types List of parameter types of the procedure declaration
 */
void SymbolTable::insertProcedureDeclaration(const std::string& signature, const std::vector<SymbolType>& types) {
    procedureDeclarations.insert({signature, types});
}

/**
 * Retrieve an item from the list of procedure declarations.
 *
 * @param signature Signature of the desired procedure declaration
 * @return List of parameter types of the desired procedure declaration
 */
std::vector<SymbolType> SymbolTable::getProcedureDeclaration(const std::string& signature) {
    if (procedureDeclarations.find(signature) == procedureDeclarations.end()) return {};
    return procedureDeclarations.at(signature);
}

/**
 * Push a function/procedure signature to a queue of function/procedure signatures. This is used to push the signatures
 * of function/procedure definitions and calls in the semantic analysis
 *
 * @param signature Signature of the function/procedure
 */
void SymbolTable::pushSignature(const FunctionSignature& signature) {
    functionSignatures.push(signature);
}

/**
 * Pop a function/procedure signature from a queue of function/procedure signatures. This is used to pop the signatures
 * of function/procedure definitions and calls in the generator component
 *
 * @return Signature of the function/procedure
 */
FunctionSignature SymbolTable::popSignature() {
    auto signature = functionSignatures.front();
    functionSignatures.pop();
    return signature;
}

/**
 * Retrieve the LLVM BasicBlock, which is currently set as continue block for the current scope
 *
 * @return Continue block
 */
llvm::BasicBlock* SymbolTable::getContinueBlock() const {
    return continueBlock;
}

/**
 * Set continue block, which marks where to continue when a continue instruction is executed in the current scope
 *
 * @param block
 */
void SymbolTable::setContinueBlock(llvm::BasicBlock* block) {
    continueBlock = block;
}

/**
 * Retrieve the LLVM BasicBlock, which is currently set as break block for the current scope
 *
 * @return Break block
 */
llvm::BasicBlock* SymbolTable::getBreakBlock() const {
    return breakBlock;
}

/**
 * Set break block, which marks where to continue when a break instruction is executed in the current scope
 *
 * @param block
 */
void SymbolTable::setBreakBlock(llvm::BasicBlock* block) {
    breakBlock = block;
}

/**
 * Stringify a symbol table to a human-readable form. This is used to realize dumps of symbol tables
 *
 * @return Symbol table if form of a string
 */
std::string SymbolTable::toString() {
    std::string symbolsString, childrenString;
    for (auto& symbol : symbols)
        symbolsString.append("(" + symbol.second.toString() + ")\n");
    for (auto& child : children)
        childrenString.append(child.first + ": " + child.second.toString() + "\n");
    return "SymbolTable(\n" + symbolsString + ") {\n" + childrenString + "}";
}
