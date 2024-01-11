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

ToDo