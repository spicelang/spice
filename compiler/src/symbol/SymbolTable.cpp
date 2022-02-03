// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "symbol/SymbolTable.h"
#include "analyzer/AnalyzerVisitor.h" // Must remain here due to circular import

/**
 * Insert a new symbol into the current symbol table. If it is a parameter, append its name to the paramNames vector
 *
 * @param name Name of the symbol
 * @param type Type of the symbol
 * @param state State of the symbol (declared or initialized)
 * @param isConstant Enabled if the symbol is a constant
 * @param isParameter Enabled if the symbol is a function/procedure parameter
 */
void SymbolTable::insert(const std::string& name, SymbolType type, SymbolSpecifiers specifiers, SymbolState state,
                         const antlr4::Token& token, bool isParameter) {
    bool isGlobal = getParent() == nullptr;
    unsigned int orderIndex = symbols.size();
    // Insert into symbols map
    symbols.insert({
        name,
        SymbolTableEntry(name, std::move(type), specifiers, state, token, orderIndex, isGlobal)
    });
    // If the symbol is a parameter, add it to the parameters list
    if (isParameter) paramNames.push_back(name);
}

/**
 * Check if a symbol exists in the current or any parent scope and return it if possible
 *
 * @param name Name of the desired symbol
 * @return Desired symbol / nullptr if the symbol was not found
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
 * Check if an order index exists in the current or any parent scope and returns it if possible.
 * Warning: Unlike the `lookup` method, this one doesn't consider the parent scopes
 *
 * @param orderIndex Order index of the desired symbol
 * @return Desired symbol / nullptr if the symbol was not found
 */
SymbolTableEntry* SymbolTable::lookupByIndexInCurrentScope(unsigned int orderIndex) {
    for (auto& [key, val] : symbols) {
        if (val.getOrderIndex() == orderIndex) return &val;
    }
    return nullptr;
}

/**
 * Search for a symbol table by its name, where a function is defined. Used for function calls to function/procedures
 * which were linked in from other modules
 *
 * @param scopeId Scope ID of the desired symbol table
 * @return Desired symbol table
 */
SymbolTable* SymbolTable::lookupTable(const std::string& scopeId) {
    // If not available in the current scope, search in the parent scope
    if (children.find(scopeId) == children.end()) {
        if (parent == nullptr) return nullptr;
        return parent->lookupTable(scopeId);
    }
    // Otherwise, return the entry
    return &children.at(scopeId);
}

/**
 * Search for a symbol table by its name, where a symbol is defined. Used for function calls to function/procedures
 * which were linked in from other modules
 *
 * @param signature Signature of the function/procedure
 * @return Desired symbol table
 */
SymbolTable* SymbolTable::lookupTableWithSignature(const std::string& signature) {
    // Check if scope contains this signature
    if (symbols.find(signature) != symbols.end()) return this;
    // Current scope does not contain the signature => go up one table
    if (parent == nullptr) return nullptr;
    return parent->lookupTableWithSignature(signature);
}

/**
 * Update the state of a symbol in the current symbol table or a parent scope.
 *
 * @throws runtime_error When trying to update a non-existent symbol
 * @param name Name of the symbol to update
 * @param newState New state of the symbol to update
 * @param token Lexer token, where the symbol table update was initiated
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
void SymbolTable::update(const std::string& name, const SymbolType& newType) {
    // If not available in the current scope, search in the parent scope
    if (symbols.find(name) == symbols.end()) {
        if (parent == nullptr) throw std::runtime_error("Updating a non-existent symbol: " + name);
        parent->update(name, newType);
    }
    // Otherwise, update the entry
    symbols.at(name).updateType(newType, false);
}

/**
 * Create a child leaf for the tree of symbol tables and return it
 *
 * @param blockName Name of the child scope
 * @return Newly created child table
 */
SymbolTable* SymbolTable::createChildBlock(const std::string& blockName) {
    children.insert({blockName, SymbolTable(this, inMainSourceFile)});
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
    childBlock->parent = this;
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
    if (children.find(scopeId) == children.end()) return nullptr;
    return &children.at(scopeId);
}

/**
 * Returns the number of symbols in the table, which are no functions, procedures or import
 *
 * @return Number of fields
 */
unsigned int SymbolTable::getFieldCount() {
    unsigned int count = 0;
    for (auto& [key, symbol] : symbols) {
        if (!symbol.getType().isOneOf({ TY_FUNCTION, TY_PROCEDURE, TY_IMPORT }))
            count++;
    }
    return count;
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
 * Changes a specific type to another in the whole sub-table
 *
 * @param oldType Old symbol type
 * @param newType Replacement type
 */
void SymbolTable::updateSymbolTypes(const SymbolType& oldType, const SymbolType& newType) {
    // Update types in the symbol list
    for (auto& [key, symbol] : symbols) {
        SymbolType currentType = symbol.getType();
        std::vector<SymbolSuperType> ptrArrayList;
        while (currentType.isOneOf({ TY_PTR, TY_ARRAY })) {
            if (currentType.isPointer())
                ptrArrayList.push_back(TY_PTR);
            else
                ptrArrayList.push_back(TY_ARRAY);
            currentType = currentType.getContainedTy();
        }
        if (currentType == oldType) {
            std::reverse(ptrArrayList.begin(), ptrArrayList.end());
            SymbolType currentNewType = newType;
            for (auto& superType : ptrArrayList) {
                if (superType == TY_PTR)
                    currentNewType = currentNewType.toPointer();
                else
                    currentNewType = currentNewType.toArray();
            }
            symbol.updateType(currentNewType, true);
        }
    }
    // Visit all child tables
    for (auto& [key, child] : children)
        child.updateSymbolTypes(oldType, newType);
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
    assert(!functionSignatures.empty());
    auto signature = functionSignatures.front();
    functionSignatures.pop();
    return signature;
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
 * Retrieve the LLVM BasicBlock, which is currently set as continue block for the current scope
 *
 * @return Continue block
 */
llvm::BasicBlock* SymbolTable::getContinueBlock() const {
    return continueBlock;
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
 * Retrieve the LLVM BasicBlock, which is currently set as break block for the current scope
 *
 * @return Break block
 */
llvm::BasicBlock* SymbolTable::getBreakBlock() const {
    return breakBlock;
}

/**
 * Prints compiler values with regard to the symbol table
 */
void SymbolTable::printCompilerWarnings() {
    // Omit this table if it is an imported sub-table
    if (imported) return;
    // Visit own symbols
    for (auto& [key, entry] : symbols) {
        if (!entry.isUsed()) {
            if (entry.getType().is(TY_FUNCTION)) {
                CompilerWarning(entry.getDefinitionToken(), UNUSED_FUNCTION,
                                "The function '" + entry.getName() + "' is unused").print();
            } else if (entry.getType().is(TY_PROCEDURE)) {
                CompilerWarning(entry.getDefinitionToken(), UNUSED_PROCEDURE,
                                "The procedure '" + entry.getName() + "' is unused").print();
            } else if (entry.getType().is(TY_STRUCT) || entry.getType().isPointerOf(TY_STRUCT)) {
                CompilerWarning(entry.getDefinitionToken(), UNUSED_STRUCT,
                                "The struct '" + entry.getName() + "' is unused").print();
            } else if (entry.getType().isOneOf({ TY_IMPORT })) {
                CompilerWarning(entry.getDefinitionToken(), UNUSED_IMPORT,
                                "The import '" + entry.getName() + "' is unused").print();
            } else {
                if (entry.getName() != UNUSED_VARIABLE_NAME)
                    CompilerWarning(entry.getDefinitionToken(), UNUSED_VARIABLE,
                                    "The variable '" + entry.getName() + "' is unused").print();
            }
        }
    }
    // Visit children
    for (auto& [key, child] : children) child.printCompilerWarnings();
}

/**
 * Stringify a symbol table to a human-readable form. This is used to realize dumps of symbol tables
 *
 * Example:
 * {
 *   "symbols": [
 *     ... (SymbolTableEntry)
 *   ],
 *   "children": [
 *     ... (SymbolTable)
 *   ]
 * }
 *
 * @return Symbol table if form of a string
 */
nlohmann::ordered_json SymbolTable::toJSON() {
    // Collect all symbols
    std::vector<nlohmann::json> jsonSymbols;
    jsonSymbols.reserve(symbols.size());
    for (auto& symbol : symbols)
        jsonSymbols.push_back(symbol.second.toJSON());

    // Collect all children
    std::vector<nlohmann::json> jsonChildren;
    jsonChildren.reserve(symbols.size());
    for (auto& child : children)
        jsonChildren.push_back(child.second.toJSON());

    // Generate json
    nlohmann::json result;
    result["symbols"] = jsonSymbols;
    result["children"] = jsonChildren;
    return result;
}

/**
 * Marks this symbol table as imported. This means, that it is a nested symbol table in the main symbol table
 */
void SymbolTable::setImported() {
    imported = true;
}

/**
 * Checks if this symbol table is imported
 *
 * @return Imported / not imported
 */
bool SymbolTable::isImported() const {
    return imported;
}
