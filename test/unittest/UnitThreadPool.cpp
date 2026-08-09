// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include <gtest/gtest.h>

#include <atomic>
#include <mutex>
#include <ranges>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>

#include <util/Concurrency.h>
#include <util/ThreadPool.h>

namespace spice::testing {

using namespace spice::compiler;

TEST(ThreadPoolTest, RunsAllSubmittedTasks) {
  ThreadPool pool(4);
  std::atomic<size_t> executedTasks = 0;
  for (size_t i = 0; i < 1000; i++)
    pool.submit([&executedTasks] { executedTasks++; });
  pool.waitForAll();

  EXPECT_EQ(1000u, executedTasks);
  EXPECT_EQ(4u, pool.getThreadCount());
}

TEST(ThreadPoolTest, RethrowsExceptionOfEarliestFailingTask) {
  ThreadPool pool(4);
  // Two tasks fail. Independent of which worker gets there first, the failure of the task that was submitted first has
  // to win, so that the reported compiler error does not depend on the scheduling.
  for (size_t i = 0; i < 200; i++)
    pool.submit([i] {
      if (i == 7 || i == 90)
        throw std::runtime_error("task " + std::to_string(i));
    });

  try {
    pool.waitForAll();
    FAIL() << "Expected the failure of the earliest task to be re-thrown"; // GCOV_EXCL_LINE
  } catch (const std::runtime_error &e) {
    EXPECT_EQ("task 7", std::string(e.what()));
  }
}

TEST(ThreadPoolTest, IsReusableAfterFailure) {
  ThreadPool pool(2);
  pool.submit([] { throw std::runtime_error("boom"); });
  EXPECT_THROW(pool.waitForAll(), std::runtime_error);

  std::atomic<size_t> executedTasks = 0;
  for (size_t i = 0; i < 100; i++)
    pool.submit([&executedTasks] { executedTasks++; });
  pool.waitForAll();

  EXPECT_EQ(100u, executedTasks);
}

TEST(ConcurrencyTest, ConditionalLockOnlyLocksInParallelSection) {
  std::mutex mutex;
  // try_lock on a mutex that is already held by the calling thread is undefined behavior, so the probe has to run on
  // another thread
  const auto isLockedByUs = [&mutex] {
    bool couldLock = false;
    std::thread probe([&] {
      couldLock = mutex.try_lock();
      if (couldLock)
        mutex.unlock();
    });
    probe.join();
    return !couldLock;
  };

  ASSERT_FALSE(concurrentPassesRunning);

  { // Outside of a parallel section the lock is a no-op
    ConditionalLock lock(mutex);
    EXPECT_FALSE(isLockedByUs());
  }

  { // Inside a parallel section the mutex is held for the lifetime of the lock
    const ParallelSection parallelSection;
    ASSERT_TRUE(concurrentPassesRunning);
    ConditionalLock lock(mutex);
    EXPECT_TRUE(isLockedByUs());
  }

  EXPECT_FALSE(concurrentPassesRunning);
}

TEST(ConcurrencyTest, ConditionalLockGuardsSharedMapUnderContention) {
  ThreadPool pool(4);
  std::mutex mutex;
  std::unordered_map<size_t, size_t> sharedMap; // Stands in for the type registry / lookup caches

  {
    const ParallelSection parallelSection;
    for (size_t i = 0; i < 4; i++)
      pool.submit([&] {
        for (size_t key = 0; key < 10000; key++) {
          ConditionalLock lock(mutex);
          sharedMap[key % 500]++;
        }
      });
    pool.waitForAll();
  }

  ASSERT_EQ(500u, sharedMap.size());
  for (const auto &count : sharedMap | std::views::values)
    EXPECT_EQ(4u * 20u, count);
}

} // namespace spice::testing
