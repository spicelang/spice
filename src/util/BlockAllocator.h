// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include <ast/ASTNodes.h>

namespace spice::compiler {

// Typedefs
using byte = uint8_t;

template <typename Base> class BlockAllocator {
public:
  // Constructors
  explicit BlockAllocator(size_t blockSize = 2048ull) : blockSize(blockSize) { allocateNewBlock(); }
  ~BlockAllocator() {
    // Destruct all objects
    for (Base *ptr : allocatedObjects)
      ptr->~Base();

    // Free memory
    for (byte *ptr : memoryBlocks)
      std::free(static_cast<void *>(ptr));
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
  std::vector<byte *> memoryBlocks;
  std::vector<Base *> allocatedObjects;
  size_t blockSize;
  size_t offsetInBlock = 0;

  // Private methods
  void allocateNewBlock() {
    // Allocate new block
    byte *ptr = static_cast<byte *>(std::malloc(blockSize));
    if (!ptr)
      throw CompilerError(OOM, "Could not allocate memory for ArenaAllocator. Already allocated " +
                                   std::to_string(memoryBlocks.size()) + " blocks.");

    // Store pointer and reset offset
    memoryBlocks.push_back(ptr);
    offsetInBlock = 0;
  }
};

} // namespace spice::compiler