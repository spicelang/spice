// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <cstdint>
#include <unistd.h>
#include <vector>

#include <ast/ASTNodes.h>
#include <util/Memory.h>

namespace spice::compiler {

template <typename Base> class BlockAllocator {
public:
  // Constructors
  explicit BlockAllocator(const MemoryManager &memoryManager, size_t blockSize = CommonUtil::getSystemPageSize())
      : memoryManager(memoryManager), blockSize(blockSize) {
    allocateNewBlock();
  }
  ~BlockAllocator() {
    // Destruct all objects
    for (Base *ptr : allocatedObjects)
      ptr->~Base();
    allocatedObjects.clear();

    // Free memory
    for (byte *ptr : memoryBlocks)
      memoryManager.deallocate(ptr);
    memoryBlocks.clear();
  }

  // Public methods
  template <typename T, typename... Args> T *allocate(Args &&...args) {
    static_assert(std::is_base_of<Base, T>::value, "T must be derived from Base");
    constexpr size_t objSize = sizeof(T);
    assert(objSize <= blockSize && "Object size exceeds block size");

    // Check if we need a new block
    if (offsetInBlock + objSize >= blockSize)
      allocateNewBlock();

    // Construct object at the offset address
    byte *destAddr = memoryBlocks.back() + offsetInBlock;
    T *ptr = new (destAddr) T(std::forward<Args>(args)...);
    allocatedObjects.push_back(ptr);

    // Update offset to be ready to store the next object
    offsetInBlock += objSize;
    return ptr;
  }

private:
  // Private members
  const MemoryManager &memoryManager;
  std::vector<byte *> memoryBlocks;
  std::vector<Base *> allocatedObjects;
  size_t blockSize;
  size_t offsetInBlock = 0;

  // Private methods
  void allocateNewBlock() {
    // Allocate new block
    byte *ptr = memoryManager.allocate(blockSize);
    if (!ptr)
      throw CompilerError(OOM, "Could not allocate memory for BlockAllocator. Already allocated " +
                                   std::to_string(memoryBlocks.size()) + " blocks.");

    // Store pointer and reset offset
    memoryBlocks.push_back(ptr);
    offsetInBlock = 0;
  }
};

} // namespace spice::compiler