---
title: Threads
---

Spice supports multithreading. It is very easy to use and very lightweight. Under the hood Spice uses posix pthreads to realize
multi-threading.

## Usage
A simple worker thread can be spawned like this:

```spice
import "std/os/thread";

f<int> main() {
    Thread thread = Thread(p() [[async]] {
        // Do something
    });
    thread.run();
}
```

Please always add the `[[async]]` attribute to the thread routine. This will ensure, that the thread routine is executed in a
thread-safe manner. The `run()` method will start the thread. The thread will run until the thread routine returns.

To join a thread to the main thread, use the `join()` method:

```spice
import "std/os/thread";

f<int> main() {
    Thread thread1 = Thread(p() [[async]] {
        // Do something
    });
    thread1.run();
    Thread thread2 = Thread(p() [[async]] {
        // Do something
    });
    thread2.run();
    // Do something
    thread1.join();
    thread2.join();
}
```

To get the ID of a thread, use the `getId()` method. This only works, if `run()` was already called on the thread object.
To get the ID of the current thread (i.e. within the thread routine), you can call the static `getThreadId()` function:

```spice
import "std/os/thread";

f<int> main() {
    Thread thread1 = Thread(p() [[async]] {
        // Do something
    });
    Thread thread2 = Thread(p() [[async]] {
        // Do something
    });
    thread2.run();
    thread1.run();
    printf("Thread 1 ID: %d\n", thread1.getId());
    printf("Thread 2 ID: %d\n", thread2.getId());
}
```

## Thread pools

Spice offers thread pools out of the box via the `std/os/threadpool` module. A thread pool is a collection of worker threads of
a fixed size. The threads are spawned when the thread pool is created and are kept alive until the thread pool is destroyed.
The threads are idle until a task is enqueued to the thread pool. The thread pool then assigns the task to one of the workers.
The thread pool can be used like this:

```spice
import "std/os/threadpool";

f<int> main() {
    ThreadPool tp = ThreadPool(3s); // Create a thread pool with 3 worker threads
    // Enque tasks to the thread pool, that will run in parallel
    tp.enqueue(p() [[async]] {
        delay(50);
        printf("Hello from task 1\n");
    });
    tp.enqueue(p() [[async]] {
        delay(100);
        printf("Hello from task 2\n");
    });
    tp.enqueue(p() [[async]] {
        delay(150);
        printf("Hello from task 3\n");
    });
    tp.start(); // This will kick off the worker threads and immediately return
}
```

To pause the thread pool, use the `pause()` method. This will pause all worker threads. To resume the thread pool, use the
`resume()` method. This will resume all worker threads. To stop the thread pool, use the `stop()` method. The worker threads will
continue the current task and then exit. You can start the thread pool again by calling the `start()` method at any time.

To wait for all tasks to finish, use the `join()` method. This will block the current thread until all tasks in the thread pool
are finished.

## Mutexes

When multiple threads share state, you need a way to make sure that only one thread modifies it at a time.
The `std/os/mutex` module provides a `Mutex` type for this. It is backed by a POSIX `pthread_mutex_t` under the hood,
so calls to `acquire()` block (the OS scheduler parks the thread) until the mutex becomes available, rather than
busy-waiting on the CPU.

### Basic usage

```spice
import "std/os/mutex";

f<int> main() {
    Mutex m;

    m.acquire();      // Block until we hold the mutex
    // critical section: only one thread can be here at a time
    m.release();      // Hand it off to the next waiter
}
```

The mutex initializes itself when constructed and destroys the underlying pthread handle in its destructor —
no explicit `init`/`destroy` calls are required.

If you only want to take the mutex when it is immediately available, use `tryAcquire()`:

```spice
if m.tryAcquire() {
    // we got the lock without blocking
    m.release();
} else {
    // someone else holds it; do something else
}
```

### LockGuard (RAII)

Forgetting to call `release()` — for example because an early `return` or a `panic` skips the call — leaves the
mutex permanently locked. The `LockGuard` type avoids that: it acquires the mutex in its constructor and releases
it in its destructor, so the mutex is freed when the guard goes out of scope, no matter how the scope exits.

```spice
import "std/os/mutex";

p criticalWork(Mutex& m) {
    LockGuard _ = LockGuard(m);
    // critical section
    // ...
    // m is released automatically when this scope ends
}
```

This is the recommended way to use a mutex unless you really need fine-grained control over when the lock is
released.

### Sharing a mutex across threads

A `Mutex` owns its underlying pthread handle, so you should not pass it by value across threads — always share it
by reference, typically by placing it inside a struct whose pointer the thread routine captures.

```spice
import "std/os/mutex";
import "std/os/thread";

type Counter struct {
    Mutex m
    int value = 0
}

p Counter.incrementMany(int n) {
    p() worker = p() [[async]] {
        for int i = 0; i < n; i++ {
            LockGuard _ = LockGuard(this.m);
            this.value++;
        }
    };
    Thread t1 = Thread(worker);
    Thread t2 = Thread(worker);
    t1.run();
    t2.run();
    t1.join();
    t2.join();
}

f<int> main() {
    Counter c;
    c.incrementMany(10000);
    printf("counter = %d\n", c.value); // always 20000
}
```

Because both threads increment `this.value` under the same `LockGuard`, no updates are lost. Without the mutex,
the read-modify-write of `this.value++` would race and the final count would be less than `20000`.