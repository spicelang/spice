// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "ABIInfo.h"

#include <ranges>

#include <llvm/IR/DerivedTypes.h>

namespace spice::compiler {

namespace {

// Register class of an eightbyte in the x86-64 SysV ABI
enum class ArgClass : uint8_t {
  NO_CLASS,
  INTEGER,
  SSE,
  MEMORY,
};

ArgClass mergeClasses(ArgClass accumulated, ArgClass field) {
  if (accumulated == field || field == ArgClass::NO_CLASS)
    return accumulated;
  if (accumulated == ArgClass::NO_CLASS)
    return field;
  if (accumulated == ArgClass::MEMORY || field == ArgClass::MEMORY)
    return ArgClass::MEMORY;
  return ArgClass::INTEGER; // INTEGER and SSE
}

} // namespace

ABIInfo::ABIInfo(const llvm::Triple &targetTriple, const llvm::DataLayout &dataLayout)
    : targetTriple(targetTriple), dataLayout(dataLayout) {}

/**
 * Classify how a value of the given type is returned from a function
 *
 * @param type Type of the return value
 * @param isNonTrivial True if the type has a custom copy ctor or dtor (also transitively via its fields)
 * @return Return ABI info
 */
ReturnABIInfo ABIInfo::classifyReturnType(llvm::Type *type, bool isNonTrivial) const {
  // Only aggregates are subject to lowering
  if (!type->isAggregateType())
    return direct(type);

  // Types, that can't be copied trivially, are always returned via memory, so that they keep their address
  if (isNonTrivial)
    return indirect(type);

  // Empty aggregates do not occupy any return register
  if (dataLayout.getTypeAllocSize(type) == 0)
    return direct(type);

  if (targetTriple.getArch() == llvm::Triple::x86_64)
    return targetTriple.isOSWindows() ? classifyReturnTypeWin64(type) : classifyReturnTypeX86_64SysV(type);
  if (targetTriple.isAArch64())
    return classifyReturnTypeAArch64(type);
  if (targetTriple.isWasm())
    return classifyReturnTypeWebAssembly(type);
  // Like the default ABI of Clang, return all other aggregates via memory
  return indirect(type);
}

ReturnABIInfo ABIInfo::classifyReturnTypeX86_64SysV(llvm::Type *type) const {
  // Aggregates larger than two eightbytes are returned via memory
  const uint64_t size = dataLayout.getTypeAllocSize(type);
  if (size > 16)
    return indirect(type);

  // Classify both eightbytes, based on the scalars they contain
  ArgClass classes[2] = {ArgClass::NO_CLASS, ArgClass::NO_CLASS};
  std::vector<std::pair<llvm::Type *, uint64_t>> scalars;
  collectScalars(type, 0, scalars);
  for (const auto &[scalarType, offset] : scalars) {
    ArgClass scalarClass;
    if (scalarType->isIntegerTy() || scalarType->isPointerTy())
      scalarClass = ArgClass::INTEGER;
    else if (scalarType->isDoubleTy() || scalarType->isFloatTy())
      scalarClass = ArgClass::SSE;
    else
      scalarClass = ArgClass::MEMORY;
    // Unaligned fields, e.g. in packed structs, force the aggregate to be returned via memory
    if (offset % dataLayout.getABITypeAlign(scalarType).value() != 0)
      scalarClass = ArgClass::MEMORY;
    const uint64_t firstEightbyte = offset / 8;
    const uint64_t lastEightbyte = (offset + dataLayout.getTypeStoreSize(scalarType) - 1) / 8;
    for (uint64_t i = firstEightbyte; i <= lastEightbyte && i < 2; i++)
      classes[i] = mergeClasses(classes[i], scalarClass);
  }
  if (classes[0] == ArgClass::MEMORY || classes[1] == ArgClass::MEMORY)
    return indirect(type);
  if (classes[0] == ArgClass::NO_CLASS && classes[1] == ArgClass::NO_CLASS)
    return direct(type);
  // Leading padding is passed in a general purpose register
  if (classes[0] == ArgClass::NO_CLASS)
    classes[0] = ArgClass::INTEGER;

  // Build the type, that is returned in registers
  const auto getEightbyteType = [&](uint64_t offset, ArgClass argClass) {
    return argClass == ArgClass::SSE ? getSSETypeAtOffset(type, offset) : getIntegerTypeAtOffset(type, offset);
  };
  llvm::Type *loType = getEightbyteType(0, classes[0]);
  if (classes[1] == ArgClass::NO_CLASS)
    return coerced(type, loType);
  llvm::Type *hiType = getEightbyteType(8, classes[1]);
  // The high part has to start at offset 8, so widen the low part, if the high part would start earlier otherwise
  const uint64_t hiStart = llvm::alignTo(dataLayout.getTypeAllocSize(loType), dataLayout.getABITypeAlign(hiType));
  if (hiStart != 8)
    loType =
        loType->isFloatingPointTy() ? llvm::Type::getDoubleTy(type->getContext()) : llvm::Type::getInt64Ty(type->getContext());
  return coerced(type, llvm::StructType::get(type->getContext(), {loType, hiType}));
}

ReturnABIInfo ABIInfo::classifyReturnTypeWin64(llvm::Type *type) const {
  // Aggregates of size 1, 2, 4 or 8 bytes are returned in RAX, all others via memory
  const uint64_t size = dataLayout.getTypeAllocSize(type);
  if (size > 8 || (size & (size - 1)) != 0)
    return indirect(type);
  return coerced(type, llvm::IntegerType::get(type->getContext(), size * 8));
}

ReturnABIInfo ABIInfo::classifyReturnTypeAArch64(llvm::Type *type) const {
  // Homogeneous floating point aggregates are returned in the floating point registers
  llvm::Type *baseType = nullptr;
  uint64_t memberCount = 0;
  if (isHomogeneousFPAggregate(type, baseType, memberCount) && memberCount <= 4)
    return direct(type);

  // Aggregates of up to 16 bytes are returned in general purpose registers, all others via memory
  const uint64_t size = dataLayout.getTypeAllocSize(type);
  if (size > 16)
    return indirect(type);
  llvm::LLVMContext &context = type->getContext();
  if (size <= 8)
    return coerced(type, llvm::IntegerType::get(context, size * 8));
  if (dataLayout.getABITypeAlign(type).value() < 16)
    return coerced(type, llvm::ArrayType::get(llvm::Type::getInt64Ty(context), 2));
  return coerced(type, llvm::Type::getInt128Ty(context));
}

ReturnABIInfo ABIInfo::classifyReturnTypeWebAssembly(llvm::Type *type) const {
  // Structs with a single scalar element are returned as this element, all other aggregates via memory
  if (llvm::Type *elementType = getSingleElementType(type))
    return coerced(type, elementType);
  return indirect(type);
}

/**
 * Get the integer type for the eightbyte at the given offset in the given type, like Clang's GetINTEGERTypeAtOffset
 */
llvm::Type *ABIInfo::getIntegerTypeAtOffset(llvm::Type *type, uint64_t offset) const {
  if (llvm::Type *scalarType = getScalarTypeAtOffset(type, offset)) {
    // Pointers and 64-bit integers fill the whole eightbyte
    if (scalarType->isPointerTy() || scalarType->isIntegerTy(64))
      return scalarType;
    // Smaller integers can be used, if the rest of the eightbyte contains only padding. Booleans are bytes in memory
    if (scalarType->isIntegerTy(1) || scalarType->isIntegerTy(8) || scalarType->isIntegerTy(16) || scalarType->isIntegerTy(32)) {
      const unsigned int bitWidth = std::max(scalarType->getIntegerBitWidth(), 8u);
      if (bitsContainNoUserData(type, offset * 8 + bitWidth, offset * 8 + 64))
        return llvm::IntegerType::get(type->getContext(), bitWidth);
    }
  }
  // Fall back to an integer, that covers the rest of the eightbyte
  const uint64_t size = dataLayout.getTypeAllocSize(type);
  return llvm::IntegerType::get(type->getContext(), std::min<uint64_t>(size - offset, 8) * 8);
}

/**
 * Get the floating point type for the eightbyte at the given offset in the given type, like Clang's GetSSETypeAtOffset
 */
llvm::Type *ABIInfo::getSSETypeAtOffset(llvm::Type *type, uint64_t offset) const {
  llvm::LLVMContext &context = type->getContext();
  const llvm::Type *scalarType = getScalarTypeAtOffset(type, offset);
  if (scalarType != nullptr && scalarType->isFloatTy()) {
    // A single float, followed by padding, is returned as float
    if (bitsContainNoUserData(type, offset * 8 + 32, offset * 8 + 64))
      return llvm::Type::getFloatTy(context);
    // Two floats are returned as vector of two floats
    const llvm::Type *nextScalarType = getScalarTypeAtOffset(type, offset + 4);
    if (nextScalarType != nullptr && nextScalarType->isFloatTy())
      return llvm::FixedVectorType::get(llvm::Type::getFloatTy(context), 2);
  }
  return llvm::Type::getDoubleTy(context);
}

/**
 * Get the scalar type, that starts at exactly the given offset in the given type
 *
 * @return Scalar type or nullptr, if there is none
 */
llvm::Type *ABIInfo::getScalarTypeAtOffset(llvm::Type *type, uint64_t offset) const { // NOLINT(*-no-recursion)
  if (auto *structType = llvm::dyn_cast<llvm::StructType>(type)) {
    const llvm::StructLayout *structLayout = dataLayout.getStructLayout(structType);
    if (offset >= structLayout->getSizeInBytes())
      return nullptr;
    const unsigned int elementIdx = structLayout->getElementContainingOffset(offset);
    const uint64_t elementOffset = structLayout->getElementOffset(elementIdx);
    return getScalarTypeAtOffset(structType->getElementType(elementIdx), offset - elementOffset);
  }
  if (auto *arrayType = llvm::dyn_cast<llvm::ArrayType>(type)) {
    llvm::Type *elementType = arrayType->getElementType();
    const uint64_t elementSize = dataLayout.getTypeAllocSize(elementType);
    if (elementSize == 0 || offset >= elementSize * arrayType->getNumElements())
      return nullptr;
    return getScalarTypeAtOffset(elementType, offset % elementSize);
  }
  return offset == 0 ? type : nullptr;
}

/**
 * Check if none of the scalars in the given type overlaps the given bit range, like Clang's BitsContainNoUserData
 */
bool ABIInfo::bitsContainNoUserData(llvm::Type *type, uint64_t startBit, uint64_t endBit) const {
  std::vector<std::pair<llvm::Type *, uint64_t>> scalars;
  collectScalars(type, 0, scalars);
  for (const auto &[scalarType, offset] : scalars) {
    const uint64_t scalarStartBit = offset * 8;
    const uint64_t scalarEndBit = scalarStartBit + dataLayout.getTypeStoreSizeInBits(scalarType);
    if (scalarStartBit < endBit && scalarEndBit > startBit)
      return false;
  }
  return true;
}

/**
 * Collect all scalars in the given type, together with their offsets
 */
void ABIInfo::collectScalars(llvm::Type *type, uint64_t offset, // NOLINT(*-no-recursion)
                             std::vector<std::pair<llvm::Type *, uint64_t>> &scalars) const {
  if (auto *structType = llvm::dyn_cast<llvm::StructType>(type)) {
    const llvm::StructLayout *structLayout = dataLayout.getStructLayout(structType);
    for (unsigned int i = 0; i < structType->getNumElements(); i++)
      collectScalars(structType->getElementType(i), offset + structLayout->getElementOffset(i), scalars);
  } else if (auto *arrayType = llvm::dyn_cast<llvm::ArrayType>(type)) {
    llvm::Type *elementType = arrayType->getElementType();
    const uint64_t elementSize = dataLayout.getTypeAllocSize(elementType);
    for (uint64_t i = 0; i < arrayType->getNumElements(); i++)
      collectScalars(elementType, offset + i * elementSize, scalars);
  } else {
    scalars.emplace_back(type, offset);
  }
}

/**
 * Check if the given type is a homogeneous floating point aggregate (all scalars have the same floating point type)
 */
bool ABIInfo::isHomogeneousFPAggregate(llvm::Type *type, llvm::Type *&baseType, uint64_t &memberCount) const {
  std::vector<std::pair<llvm::Type *, uint64_t>> scalars;
  collectScalars(type, 0, scalars);
  if (scalars.empty())
    return false;
  baseType = scalars.front().first;
  if (!baseType->isFloatingPointTy())
    return false;
  for (llvm::Type *scalarType : scalars | std::views::keys)
    if (scalarType != baseType)
      return false;
  memberCount = scalars.size();
  // There must not be any padding
  return memberCount * dataLayout.getTypeAllocSize(baseType) == dataLayout.getTypeAllocSize(type);
}

/**
 * Get the type of the only scalar of a struct, like Clang's isSingleElementStruct
 *
 * @return Element type or nullptr, if the type is no single element struct
 */
llvm::Type *ABIInfo::getSingleElementType(llvm::Type *type) const {
  std::vector<std::pair<llvm::Type *, uint64_t>> scalars;
  collectScalars(type, 0, scalars);
  if (scalars.size() != 1)
    return nullptr;
  llvm::Type *elementType = scalars.front().first;
  // The element must fill the whole struct
  if (dataLayout.getTypeAllocSize(elementType) != dataLayout.getTypeAllocSize(type))
    return nullptr;
  return elementType;
}

ReturnABIInfo ABIInfo::direct(llvm::Type *type) { return {.kind = ReturnABIKind::DIRECT, .type = type}; }

ReturnABIInfo ABIInfo::coerced(llvm::Type *type, llvm::Type *coercedType) {
  return {.kind = ReturnABIKind::COERCED, .type = type, .coercedType = coercedType};
}

ReturnABIInfo ABIInfo::indirect(llvm::Type *type) { return {.kind = ReturnABIKind::INDIRECT, .type = type}; }

} // namespace spice::compiler
