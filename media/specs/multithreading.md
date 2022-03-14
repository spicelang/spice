# Technical Specification for Multithreading in Spice

## Syntax

### Push work to a new anonymous thread
This enables the programmer to execute portions of code in another thread that runs in parallel to the original one. The block immediately returns after launching the new thread and continues executing the code after the thread block.

```spice
import "std/os/thread" as t;

// ...

thread {
    printf("Thread Id: %d", tid());
    // Do something
}
```

Within the thread block, the builtin function `tid()` can be used to obtain the so-called thread id (in the following called `tid`). This is the id, which Spice assigned to the anonymous thread.

### Waiting for a thread to terminate
To wait for another thread to end its execution, the builtin `join(byte*)` can be used. The program will suspend the execution when calling `join` until the thread with the given tid has terminated.

```spice
byte* t1;
byte* t2;
byte* t3;

t1 = thread {
    join(t2, t3);
    printf("Thread 1");
};

t2 = thread {
    printf("Thread 2");
};

t3 = thread {
    printf("Thread 3");
};
```

### Communication between threads
For the communication between threads, there is a feature, called `Pipes`. A pipe is a wrapper around any type and can act like a temporary buffer queue for one or multiple (primitive or complex) values. Those values can be pushed by calling the builtin function `yield(pipe<any>, any)` and received by calling the `pick(pipe<any>)` builtin. If `pick` is called on a pipe and this pipe currently has no value present, the execution will pause until there is a new value for that pipe. 

```spice
pipe<int> intPipe;

byte* t1 = thread {
    int yieldValue = 12345;
    yield(intPipe, yieldValue);
};

byte* t2 = thread 2 {
    int receivedValue = pick(intPipe);
    printf("Received value: %d", receivedValue); // Output: 12345
};
```

### Thread pools (long way off, not finalized, may change)
To support thread pools in Spice, a std module called `std/os/threadpool` is planned. <br>
We probably need Spice support for function pointers to realize thread pools efficiently. Furthermore, it would be useful to have the `Queue` data structure to manage the tasks to execute. And for realizing Queues, we probably first need to support generics.

Idea: https://stackoverflow.com/questions/18627817/is-there-any-method-other-than-pthread-create-to-assign-work-to-the-same-thread

This std module could contain a struct called `ThreadPool`:

```spice
import "std/data/queue" as queue;

type ThreadPool struct {
	unsigned int threadCount // Number of threads in the thread pool
	unsigned long taskCount // Counter, which gets incremented when a task comes in
	queue.Queue<void*> taskQueue // Queue of tasks to execute. Whenever a thread has no work to do it pops an item from the queue and executes that.
}

f<int> setThreadCount(); // Add or remove threads from the thread pool
f<int> getThreadCount(); // Return thread count
f<int> getTaskCount(); // Return how many tasks were already executed
p pushWork(void*); // Pushes a function pointer to the queue of tasks
f<bool> hasTasks(); // Returns true if the task list contains at least one item
f<int> getQueueSize(); // Returns the number of items in the task queue
```