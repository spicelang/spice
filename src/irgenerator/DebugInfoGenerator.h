// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <stack>

#include <llvm/IR/DIBuilder.h>

// Forward declarations
class IRGenerator;
class SymbolTableEntry;
class SymbolType;
class Function;
class ASTNode;
struct CodeLoc;

class DebugInfoGenerator {
public:
  // Constructors
  explicit DebugInfoGenerator(IRGenerator *irGenerator) : irGenerator(irGenerator) {}

  // Public methods
  void initialize(const std::string &sourceFileName, const std::string &sourceFileDir);
  void finalize();
  [[nodiscard]] llvm::DIType *getDITypeForSymbolType(const SymbolType &symbolType) const;
  void generateFunctionDebugInfo(llvm::Function *llvmFunction, const Function *spiceFunc);
  //[[nodiscard]] llvm::DIType *generateStructDebugInfo(llvm::StructType *llvmStructTy, const Struct *spiceStruct) const;
  void generateGlobalVarDebugInfo(llvm::GlobalVariable *global, const SymbolTableEntry *globalEntry);
  void generateDeclDebugInfo(const CodeLoc &codeLoc, const std::string &varName, llvm::Value *address, size_t argNo = SIZE_MAX,
                             bool moveToPrev = false);
  void setSourceLocation(const ASTNode *node);

private:
  // Private members
  IRGenerator *irGenerator;
  std::unique_ptr<llvm::DIBuilder> diBuilder;
  struct DebugInfo {
    llvm::DIFile *diFile = nullptr;
    llvm::DICompileUnit *compileUnit = nullptr;
    std::stack<llvm::DIScope *> lexicalBlocks;
    llvm::DIType *doubleTy = nullptr;
    llvm::DIType *intTy = nullptr;
    llvm::DIType *uIntTy = nullptr;
    llvm::DIType *shortTy = nullptr;
    llvm::DIType *uShortTy = nullptr;
    llvm::DIType *longTy = nullptr;
    llvm::DIType *uLongTy = nullptr;
    llvm::DIType *byteTy = nullptr;
    llvm::DIType *charTy = nullptr;
    llvm::DIType *stringTy = nullptr;
    llvm::DIType *boolTy = nullptr;
  } debugInfo;
};