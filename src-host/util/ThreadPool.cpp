// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "ThreadPool.h"

#include <cassert>
#include <utility>

namespace spice::compiler {

ThreadPool::ThreadPool(size_t threadCount) {
  assert(threadCount > 0);
  workers.reserve(threadCount);
  for (size_t i = 0; i < threadCount; i++)
    workers.emplace_back([this] { workerLoop(); });
}

ThreadPool::~ThreadPool() {
  {
    std::lock_guard lock(mutex);
    shuttingDown = true;
  }
  taskAvailable.notify_all();
  for (std::thread &worker : workers)
    worker.join();
}

/**
 * Enqueue a task for execution on one of the worker threads
 *
 * @param task Task to execute
 */
void ThreadPool::submit(std::function<void()> task) {
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
void ThreadPool::waitForAll() {
  std::exception_ptr failure;
  {
    std::unique_lock lock(mutex);
    allTasksDone.wait(lock, [this] { return pendingCount == 0; });
    // Reset for the next batch
    failure = std::exchange(firstException, nullptr);
    canceled = false;
    nextTaskIndex = 0;
  }
  if (failure)
    std::rethrow_exception(failure);
}

/**
 * Body of a worker thread: take tasks off the queue until the pool shuts down
 */
void ThreadPool::workerLoop() {
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
      skip = canceled;
    }

    if (!skip) {
      try {
        task.job();
      } catch (...) {
        std::lock_guard lock(mutex);
        // Keep the exception of the earliest submitted task, to make error reporting independent of the scheduling
        if (!firstException || task.index < firstExceptionTaskIndex) {
          firstException = std::current_exception();
          firstExceptionTaskIndex = task.index;
        }
        canceled = true;
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

} // namespace spice::compiler
