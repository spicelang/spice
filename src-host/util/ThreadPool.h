// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <condition_variable>
#include <cstddef>
#include <exception>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace spice::compiler {

/**
 * Fixed-size pool of worker threads, used to run independent compiler passes concurrently.
 *
 * The pool is intentionally minimal: work is submitted as void() tasks and the submitter blocks in waitForAll() until
 * the whole batch is done. Two properties matter for the compiler:
 *
 * - Compiler passes report failures by throwing (LexerError, SemanticError, CompilerError, ...). An exception escaping
 *   a task would terminate the process, so tasks are wrapped and the exception is re-thrown on the waiting thread.
 * - Diagnostics must not depend on thread scheduling. Every task carries the index it was submitted with and, if
 *   multiple tasks fail, the one with the lowest index wins. The user therefore always sees the same error, no matter
 *   which worker happened to get there first.
 *
 * As soon as one task has failed, queued tasks that have not started yet are dropped, so a broken build fails fast.
 */
class ThreadPool final {
public:
  // Constructors
  explicit ThreadPool(size_t threadCount);

  // Prevent copy
  ThreadPool(const ThreadPool &) = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;

  // Destructor
  ~ThreadPool();

  // Public methods
  void submit(std::function<void()> task);
  void waitForAll();
  [[nodiscard]] size_t getThreadCount() const { return workers.size(); }

private:
  // Structs
  struct Task {
    size_t index = 0;
    std::function<void()> job;
  };

  // Private methods
  void workerLoop();

  // Members
  std::vector<std::thread> workers;
  std::queue<Task> tasks;
  std::mutex mutex;
  std::condition_variable taskAvailable;
  std::condition_variable allTasksDone;
  std::exception_ptr firstException;
  size_t firstExceptionTaskIndex = 0;
  size_t nextTaskIndex = 0;
  size_t pendingCount = 0;
  bool canceled = false;
  bool shuttingDown = false;
};

} // namespace spice::compiler
