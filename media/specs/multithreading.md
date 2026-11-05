# Technical Specification for Multithreading in Spice

## Implementation steps:

- [x] Support creating threads
- [x] Add tests for the feature
- [x] Support running those threads
- [x] Add tests for the feature
- [x] Support retrieving the thread id (`Thread.getId()` / `getThreadId()`)
- [x] Add tests for the feature
- [x] Support thread joining
- [x] Add tests for the feature
- [x] Add arbitrary benchmark test (`benchmark/success-fibonacci-threaded` test case)
- [x] Add mutexes (`Mutex` and `LockGuard` in `std/os/mutex`)
- [x] Add tests for the feature
- [x] Add thread pools (`ThreadPool` in `std/os/thread-pool`)
- [x] Add tests for the feature
- [ ] Implement variable volatility
- [x] Add message queues (`MessageQueue<T>` in `std/os/message-queue`)
- [x] Add tests for the feature
- [x] Add documentation (`docs/docs/language/threads.md`)

## Syntax

Multithreading in Spice is not a dedicated grammar construct. Instead, it is implemented as a lightweight std library
on top of POSIX threads (pthreads): `std/os/thread` provides the `Thread` struct, `std/os/thread-pool` provides
`ThreadPool`, and `std/os/mutex` provides `Mutex`/`LockGuard`. A thread routine is simply a procedure lambda
(`p() { ... }`).

### Spawning a thread
This enables the programmer to execute a procedure in another thread that runs in parallel to the original one.
Construct a `Thread` from a `p()` lambda and call `run()` to start it. `run()` returns immediately; the
calling thread continues executing the code after it.

```spice
import "std/os/thread";

f<int> main() {
    Thread thread = Thread(p() {
        // Do something
    });
    thread.run();
}
```

### Waiting for a thread to terminate
To wait for a thread to end its execution, call the `join()` method on the `Thread` instance. The calling thread
suspends until the target thread has terminated.

```spice
import "std/os/thread";

f<int> main() {
    Thread thread1 = Thread(p() {
        // Do something
    });
    thread1.run();
    Thread thread2 = Thread(p() {
        // Do something
    });
    thread2.run();
    // Do something
    thread1.join();
    thread2.join();
}
```

### Thread ids
To get the id of a thread, call `getId()` on the `Thread` instance (only valid after `run()` was called). To get the
id of the current thread from within the thread routine, call the free function `getThreadId()`.

```spice
import "std/os/thread";

f<int> main() {
    Thread thread1 = Thread(p() {
        printf("Running on thread: %d\n", getThreadId());
    });
    thread1.run();
    printf("Thread 1 ID: %d\n", thread1.getId());
    thread1.join();
}
```

### Captures
Variables from outside the thread routine that are used within it are called `captures`. Since `p()` is a
lambda, captured variables are taken over by the `Lambda<p()>` that backs the `Thread`, which takes ownership of any
captured state so it remains valid even after the enclosing stack frame returns. Mutable shared state that is
written from multiple threads must still be protected with a `Mutex` (see below); Spice does not yet track or
enforce a `volatile`/read-vs-write distinction for captures automatically.

### Thread synchronization
Spice provides a `Mutex` type (`std/os/mutex`) for protecting shared state between threads. It is backed by a POSIX
`pthread_mutex_t`, so `acquire()` blocks (the OS parks the thread) until the mutex becomes available, rather than
busy-waiting.

```spice
import "std/os/mutex";

f<int> main() {
    Mutex m;

    m.acquire();      // Block until we hold the mutex
    // critical section: only one thread can be here at a time
    m.release();      // Hand it off to the next waiter
}
```

`tryAcquire()` attempts to take the mutex without blocking and returns `bool`. For RAII-style locking, use
`LockGuard`, which acquires the mutex in its constructor and releases it in its destructor, so the mutex is freed
no matter how the scope is exited (early `return`, `panic`, etc.):

```spice
import "std/os/mutex";

p criticalWork(Mutex& m) {
    LockGuard _ = LockGuard(m);
    // critical section
}
```

A `Mutex` owns its underlying pthread handle, so it must be shared by reference (`Mutex&`) across threads rather than
by value, typically by placing it inside a struct whose pointer/reference the thread routine captures.

### Thread pools
Spice offers thread pools via the `std/os/thread-pool` module (`ThreadPool` struct). A thread pool spawns a fixed
number of worker threads up front (defaulting to the CPU core count) and keeps them alive until told to stop. Idle
workers pull queued jobs (`p()` lambdas) and execute them in parallel.

```spice
import "std/os/thread-pool";
import "std/time/delay";

f<int> main() {
    ThreadPool tp = ThreadPool(3s); // Create a thread pool with 3 worker threads
    tp.enqueue(p() {
        delay(50);
        printf("Hello from task 1\n");
    });
    tp.enqueue(p() {
        delay(100);
        printf("Hello from task 2\n");
    });
    tp.start(); // Kicks off the worker threads and returns immediately
    tp.join();  // Wait for all queued tasks to finish
}
```

Other `ThreadPool` methods: `pause()`/`resume()` to temporarily stop/continue dispatching new jobs to workers,
`stop()` to let workers finish their current job and then exit, and `getRunningJobCount()`/`getQueuedJobCount()`/
`getWorkerThreadCount()` for introspection.

### Communication between threads
Spice provides a `MessageQueue<T>` type (`std/os/message-queue`) for passing values between threads. It is a FIFO
queue, guarded by an internal mutex, so any number of producer and consumer threads can push and pop concurrently.
`push()` never blocks, `pop()` blocks until a value is available. `tryPop()` is the non-blocking variant.

Producers call `close()` to signal the end of the stream. After that, `push()` panics, while consumers keep draining the
remaining values and get an empty `Optional` once the queue has run dry.

```spice
import "std/os/message-queue";
import "std/data/optional";

f<int> main() {
    MessageQueue<int> mq;
    mq.push(1);
    mq.close();
    Optional<int> item = mq.pop(); // 1
    Optional<int> drained = mq.pop(); // empty, since the queue is closed and drained
}
```

### Reference points in the implementation

- `Thread` struct: [std/os/thread.spice](https://github.com/spicelang/spice/blob/main/std/os/thread.spice)
- `ThreadPool` struct: [std/os/thread-pool.spice](https://github.com/spicelang/spice/blob/main/std/os/thread-pool.spice)
- `Mutex` / `LockGuard`: [std/os/mutex.spice](https://github.com/spicelang/spice/blob/main/std/os/mutex.spice)
- User-facing documentation: [docs/docs/language/threads.md](https://github.com/spicelang/spice/blob/main/docs/docs/language/threads.md)
- Tests for thread pools: [here](https://github.com/spicelang/spice/tree/main/test/test-files/std/os/thread-pool)
- `MessageQueue<T>`: [std/os/message-queue.spice](https://github.com/spicelang/spice/blob/main/std/os/message-queue.spice)
- Tests for message queues: [here](https://github.com/spicelang/spice/tree/main/test/test-files/std/os/message-queue)
- Tests for mutexes: [here](https://github.com/spicelang/spice/tree/main/test/test-files/std/os/mutex)
- Fully fledged functional test using `Thread` directly: [here](https://github.com/spicelang/spice/tree/main/test/test-files/benchmark/success-fibonacci-threaded)
