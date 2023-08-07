---
title: Threads
---

Spice supports multithreading. It is very easy to use and very lightweight. Spice uses posix pthreads under the hood.

## Usage
A simple worker thread can be spawned like this:

```spice
import "std/os/thread";

f<int> main() {
    Thread thread = Thread(p() {
        // Do something
    });
}
```

To join a thread to the main thread, use the `join()` method:

```spice
import "std/os/thread";

f<int> main() {
    Thread thread1 = Thread(p() {
        // Do something
    });
    Thread thread2 = Thread(p() {
        // Do something
    });
    // Do something
    thread1.join();
    thread2.join();
}
```

To get the ID of a thread, use the `getId()` method. To get the ID of the current thread (i.e. within the thread routine), you can
call the static `getThreadId()` function:

```spice
import "std/os/thread";

f<int> main() {
    Thread thread1 = Thread(p() {
        // Do something
    });
    Thread thread2 = Thread(p() {
        // Do something
    });
    printf("Thread 1 ID: %d\n", thread1.getId());
    printf("Thread 2 ID: %d\n", thread2.getId());
}
```

## Thread pools

ToDo

## Mutexes

ToDo