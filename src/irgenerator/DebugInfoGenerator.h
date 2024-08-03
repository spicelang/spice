// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <filesystem>
#include <stack>

#include <llvm/IR/DIBuilder.h>

namespace spice::compiler {

// Forward declarations
class IRGenerator;
class SymbolTableEntry;
class QualType;
class Function;
class Struct;
class ASTNode;
struct CodeLoc;

class DebugInfoGenerator {
public:
  // Constructors
  explicit DebugInfoGenerator(IRGenerator *irGenerator) : irGenerator(irGenerator) {}

  // Public methods
  void initialize(const std::string &sourceFileName, std::filesystem::path sourceFileDir);
  void generateFunctionDebugInfo(llvm::Function *llvmFunction, const Function *spiceFunc, bool isLambda = false);
  void concludeFunctionDebugInfo();
  void pushLexicalBlock(const ASTNode *node);
  void popLexicalBlock();
  llvm::DICompositeType *generateCaptureStructDebugInfo(const Function *spiceFunc);
  void generateGlobalVarDebugInfo(llvm::GlobalVariable *global, const SymbolTableEntry *globalEntry);
  void generateGlobalStringDebugInfo(llvm::GlobalVariable *global, const std::string &name, size_t length,
                                     const CodeLoc &codeLoc) const;
  void generateLocalVarDebugInfo(const std::string &varName, llvm::Value *address, const size_t argNumber = SIZE_MAX);
  void setSourceLocation(const CodeLoc &codeLoc);
  void setSourceLocation(const ASTNode *node);
  void finalize() const;

private:
  // Private members
  IRGenerator *irGenerator;
  std::unique_ptr<llvm::DIBuilder> diBuilder;
  llvm::DICompileUnit *compileUnit = nullptr;
  llvm::DIFile *diFile = nullptr;
  std::stack<llvm::DIScope *> lexicalBlocks;
  std::unordered_map<size_t, llvm::DICompositeType *> structTypeCache;
  unsigned int pointerWidth = 0;
  // Debug types
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
  llvm::DIType *voidTy = nullptr;
  llvm::DICompositeType *fatPtrTy = nullptr;

  // Private methods
  [[nodiscard]] llvm::DIType *getDITypeForQualType(const ASTNode *node, const QualType &ty);
};

} // namespace spice::compiler