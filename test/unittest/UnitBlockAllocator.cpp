// Copyright (c) 2021-2025 ChilliBits. All rights reserved.

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <ast/ASTNodes.h>
#include <util/BlockAllocator.h>
#include <util/CodeLoc.h>
#include <util/Memory.h>

namespace spice::testing {

using namespace spice::compiler;

static size_t destructedDummyNodes = 0;

class DummyNode final : public ASTNode {
  // Constructors
  using ASTNode::ASTNode;

  // Destructors
  ~DummyNode() override { destructedDummyNodes++; }

  // Visitor methods
  std::any accept(AbstractASTVisitor *visitor) override { return {}; }             // LCOV_EXCL_LINE
  std::any accept(ParallelizableASTVisitor *visitor) const override { return {}; } // LCOV_EXCL_LINE

  // Other methods
  GET_CHILDREN();
};
static constexpr size_t DUMMY_NODE_SIZE = sizeof(DummyNode);
static_assert(DUMMY_NODE_SIZE == 104, "DummyNode size has changed. Update test accordingly.");

class MockMemoryManager final : public MemoryManager {
public:
  MOCK_METHOD(byte *, allocate, (size_t size), (const override));
  MOCK_METHOD(void, deallocate, (byte * ptr), (const override));
};

TEST(BlockAllocatorTest, TestBlockAllocatorLarge) {
  destructedDummyNodes = 0;                     // Reset destruction counter
  static constexpr size_t NODE_COUNT = 100'000; // 100.000 * 104 bytes = 10.4 MB

  {
    // Create allocator, that can hold 5 nodes per block
    constexpr DefaultMemoryManager memoryManager;
    BlockAllocator<ASTNode> alloc(memoryManager, DUMMY_NODE_SIZE * 5);

    // Allocate nodes
    std::vector<ASTNode *> nodes;
    for (size_t i = 0; i < NODE_COUNT; i++) {
      auto node = alloc.allocate<DummyNode>(CodeLoc(i, 1));
      ASSERT_NE(nullptr, node);
      nodes.push_back(node);
      ASSERT_EQ(i, nodes.at(i)->codeLoc.line);
      ASSERT_EQ(1, nodes.at(i)->codeLoc.col);
    }

    // Check if stats are correct
    ASSERT_EQ(NODE_COUNT, alloc.getAllocationCount());
    ASSERT_EQ(13'000'000, alloc.getTotalAllocatedSize());

    // Block Allocator gets destructed here and with that, all allocated nodes should be destructed
  }

  ASSERT_EQ(NODE_COUNT, destructedDummyNodes);
}

TEST(BlockAllocatorTest, TestBlockAllocatorUnevenBlockSize) {
  destructedDummyNodes = 0;                   // Reset destruction counter
  static constexpr size_t NODE_COUNT = 1'000; // 1.000 * 104 bytes = 104 KB

  {
    // Create allocator, that can hold 4.5 nodes per block
    constexpr DefaultMemoryManager memoryManager;
    BlockAllocator<ASTNode> alloc(memoryManager, DUMMY_NODE_SIZE * 4.5);

    // Allocate nodes
    std::vector<ASTNode *> nodes;
    for (size_t i = 0; i < NODE_COUNT; i++) {
      auto node = alloc.allocate<DummyNode>(CodeLoc(i, 1));
      ASSERT_NE(nullptr, node);
      nodes.push_back(node);
      ASSERT_EQ(i, nodes.at(i)->codeLoc.line);
      ASSERT_EQ(1, nodes.at(i)->codeLoc.col);
    }

    // Check if stats are correct
    ASSERT_EQ(NODE_COUNT, alloc.getAllocationCount());
    ASSERT_EQ(117'000, alloc.getTotalAllocatedSize());

    // Block Allocator gets destructed here and with that, all allocated nodes should be destructed
  }

  ASSERT_EQ(NODE_COUNT, destructedDummyNodes);
}

TEST(BlockAllocatorTest, TestBlockAllocatorOOM) {
  destructedDummyNodes = 0;                // Reset destruction counter
  static constexpr size_t NODE_COUNT = 10; // 10 * 104 bytes = 1.04 KB

  // Prepare mock methods
  MockMemoryManager mockMemoryManager;

  // Make sure, that the memory manager returns nullptr when trying to allocate the fifth block
  ::testing::InSequence s;
  auto mallocCallback = [](size_t size) { return static_cast<byte *>(malloc(size)); };
  EXPECT_CALL(mockMemoryManager, allocate(::testing::_)).Times(4).WillRepeatedly(mallocCallback);
  EXPECT_CALL(mockMemoryManager, allocate(::testing::_)).Times(1).WillOnce(::testing::Return(nullptr));
  EXPECT_CALL(mockMemoryManager, deallocate(::testing::_)).Times(4).WillRepeatedly(::testing::Invoke(free));

  {
    // Create allocator, that can hold 2 nodes per block
    BlockAllocator<ASTNode> alloc(mockMemoryManager, DUMMY_NODE_SIZE * 2.25);

    try {
      // Allocate nodes
      std::vector<ASTNode *> nodes;
      for (size_t i = 0; i < NODE_COUNT; i++) {
        auto node = alloc.allocate<DummyNode>(CodeLoc(i, 1));
        ASSERT_NE(nullptr, node);
        nodes.push_back(node);
        ASSERT_EQ(i, nodes.at(i)->codeLoc.line);
        ASSERT_EQ(1, nodes.at(i)->codeLoc.col);
      }
      FAIL(); // LCOV_EXCL_LINE - Should not reach this point
    } catch (CompilerError &ce) {
      std::stringstream ss;
      ss << "[Error|Compiler]:\n";
      ss << "An out of memory error occurred: Could not allocate memory for BlockAllocator. Already allocated 4 blocks.";
      ASSERT_EQ(ss.str(), ce.what());
    }

    // Block Allocator gets destructed here and with that, all allocated nodes should be destructed
  }

  ASSERT_EQ(8, destructedDummyNodes); // Only 8 blocks were constructed until the OOM error occurred
  ::testing::Mock::VerifyAndClearExpectations(&mockMemoryManager);
}

} // namespace spice::testing