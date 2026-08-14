// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <atomic>
#include <mutex>

namespace spice::compiler {

/**
 * Global switch that tells the process-wide caches of the compiler (type registry, function lookup cache, ...) whether
 * compiler passes are currently executed on more than one thread.
 *
 * Those caches sit on the hottest paths of the single-threaded front end and middle end, where taking a lock for every
 * access would be pure overhead. The back end on the other hand runs one pipeline per source file in parallel (see
 * SourceFile::runBackEnd), and there the caches do need protection. ParallelSection flips this switch for exactly the
 * time span in which worker threads are alive.
 */
inline std::atomic<bool> concurrentPassesRunning = false;

/**
 * RAII marker for a section of the compiler in which passes are executed on multiple threads.
 *
 * It has to be entered before the worker threads are handed any work and left only after all of them are joined again,
 * so that every ConditionalLock taken on a worker thread actually locks.
 */
class ParallelSection final {
public:
  // Constructors
  ParallelSection() { concurrentPassesRunning.store(true, std::memory_order_relaxed); }

  // Prevent copy
  ParallelSection(const ParallelSection &) = delete;
  ParallelSection &operator=(const ParallelSection &) = delete;

  // Destructor
  ~ParallelSection() { concurrentPassesRunning.store(false, std::memory_order_relaxed); }
};

/**
 * RAII lock that is only actually acquired while compiler passes run concurrently.
 *
 * Outside of a ParallelSection this degrades to a relaxed atomic load plus a well-predicted branch, which keeps the
 * single-threaded stages at their current speed.
 */
template <typename MutexT> class ConditionalLock final {
public:
  // Constructors
  explicit ConditionalLock(MutexT &mutex) : mutex(concurrentPassesRunning.load(std::memory_order_relaxed) ? &mutex : nullptr) {
    if (this->mutex != nullptr)
      this->mutex->lock();
  }

  // Prevent copy
  ConditionalLock(const ConditionalLock &) = delete;
  ConditionalLock &operator=(const ConditionalLock &) = delete;

  // Destructor
  ~ConditionalLock() {
    if (mutex != nullptr)
      mutex->unlock();
  }

private:
  // Members
  MutexT *mutex;
};

/**
 * Guards the process-wide symbol lookup machinery: the lookup caches of FunctionManager, StructManager and
 * InterfaceManager, plus the manifestation lists and symbol table entries those matchers touch.
 *
 * The IR generator lowers struct and interface types through QualType::getStruct/getInterface and looks up implicit
 * functions (e.g. copy ctors), so all three matchers are reachable from the back-end worker threads. By the time the
 * back end runs, the type checker has already created every manifestation, so these calls degenerate to cache hits and
 * the lock is barely contended.
 *
 * It is recursive because struct matching recurses into struct and interface matching for field and interface types.
 */
inline std::recursive_mutex symbolRegistryMutex;

} // namespace spice::compiler
