// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <CompilerPass.h>
#include <ast/ASTVisitor.h>

/**
 * The borrow checker enforces the following rules:
 *
 * - At any given time, there can be at most one mutable reference to a piece of memory. This rule helps to prevent data races and
 *   other undefined behavior that can occur when two pieces of code simultaneously modify the same piece of memory.
 * - There can be multiple immutable references to a piece of memory, but no immutable reference can exist while there is a
 *   mutable reference. This rule helps to ensure that code that only needs to read from memory can do so safely, even when other
 *   code is modifying the same memory.
 * - A reference must be valid for the entire lifetime of its use. This rule helps to prevent the use of references to memory that
 *   has been deallocated, which can lead to memory leaks and other undefined behavior.
 * - A reference cannot be borrowed for a shorter lifetime than the lifetime of the value it references. This rule helps to ensure
 *   that a reference is not used after the value it references has been deallocated.
 * - A reference cannot be borrowed mutably if it has already been borrowed immutably. This rule helps to ensure that a reference
 *   is not modified while it is being used for reading.
 * - A reference cannot be borrowed mutably while it is borrowed immutably. This rule helps to ensure that a reference is not
 *   modified while it is being used for reading.
 * - A reference cannot be borrowed mutably twice. This rule helps to ensure that a reference is not modified while it is being
 *   modified.
 * - A function that borrows a reference must return it before the function ends. This rule ensures that the borrowed reference is
 *   not used anymore after the function ends and it will avoid any leaks.
 * - A reference cannot be borrowed if it is already borrowed. This rule helps to ensure that a reference is not used while it is
 *   being borrowed by another piece of code.
 */
namespace spice::compiler {

class BorrowChecker : private CompilerPass, public ASTVisitor {
public:
  // Constructors
  BorrowChecker(GlobalResourceManager &resourceManager, SourceFile *sourceFile) : CompilerPass(resourceManager, sourceFile) {}

private:
  // Private members
};

} // namespace spice::compiler