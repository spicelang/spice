// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <cassert>
#include <condition_variable>
#include <cstddef>
#include <exception>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
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
  explicit ThreadPool(size_t threadCount) {
    assert(threadCount > 0);
    workers.reserve(threadCount);
    for (size_t i = 0; i < threadCount; i++)
      workers.emplace_back([this] { workerLoop(); });
  }

  // Prevent copy
  ThreadPool(const ThreadPool &) = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;

  // Destructor
  ~ThreadPool() {
    {
      std::lock_guard lock(mutex);
      shuttingDown = true;
    }
    taskAvailable.notify_all();
    for (std::thread &worker : workers)
      worker.join();
  }

  // Public methods

  /**
   * Enqueue a task for execution on one of the worker threads
   *
   * @param task Task to execute
   */
  void submit(std::function<void()> task) {
    {
      std::lock_guard lock(mutex);
      assert(!shuttingDown);
      tasks.emplace(nextTaskIndex++, std::move(task));
      pendingCount++;
    }
    taskAvailable.notify_one();
  }

  /**
   * Block until all submitted tasks are done and re-throw the exception of the first failing task, if there was one.
   * Afterwards the pool is reset and can be used for the next batch of tasks.
   */
  void waitForAll() {
    std::exception_ptr failure;
    {
      std::unique_lock lock(mutex);
      allTasksDone.wait(lock, [this] { return pendingCount == 0; });
      // Reset for the next batch
      failure = std::exchange(firstException, nullptr);
      cancelled = false;
      nextTaskIndex = 0;
    }
    if (failure)
      std::rethrow_exception(failure);
  }

  [[nodiscard]] size_t getThreadCount() const { return workers.size(); }

private:
  // Structs
  struct Task {
    size_t index = 0;
    std::function<void()> job;
  };

  // Private methods
  void workerLoop() {
    while (true) {
      Task task;
      bool skip;
      {
        std::unique_lock lock(mutex);
        taskAvailable.wait(lock, [this] { return shuttingDown || !tasks.empty(); });
        if (tasks.empty()) {
          assert(shuttingDown);
          return;
        }
        task = std::move(tasks.front());
        tasks.pop();
        // Do not start new work after another task has already failed
        skip = cancelled;
      }

      if (!skip) {
        try {
          task.job();
        } catch (...) { // NOLINT(bugprone-empty-catch) - the exception is stored and re-thrown in waitForAll
          std::lock_guard lock(mutex);
          // Keep the exception of the earliest submitted task, to make error reporting independent of the scheduling
          if (!firstException || task.index < firstExceptionTaskIndex) {
            firstException = std::current_exception();
            firstExceptionTaskIndex = task.index;
          }
          cancelled = true;
        }
      }

      {
        std::lock_guard lock(mutex);
        pendingCount--;
        if (pendingCount == 0)
          allTasksDone.notify_all();
      }
    }
  }

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
  bool cancelled = false;
  bool shuttingDown = false;
};

} // namespace spice::compiler
