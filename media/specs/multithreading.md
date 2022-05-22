# Technical Specification for Multithreading in Spice

## Implementation steps:

- [x] 1. Support creating threads
- [x] 2. Add tests for the feature
- [x] 3. Support running those threads
- [x] 4. Add tests for the feature
- [x] 5. Support the `tid()` call
- [x] 6. Add tests for the feature
- [x] 7. Support thread joining
- [x] 8. Add tests for the feature
- [x] 9. Add arbitrary benchmark test (`generator/arbitrary/success-fibonacci-threaded` test case)
- [ ] 10. Add mutexes and `sync` specifier
- [ ] 11. Add tests for the feature
- [ ] 12. Implement variable volatility
- [ ] 13. Add support for pipes (paused due to the work on generics)
- [ ] 14. Add `stash` and `pick` builtin
- [ ] 15. Add documentation

## Syntax

### Push work to a new anonymous thread
This enables the programmer to execute portions of code in another thread that runs in parallel to the original one. The block
immediately returns after launching the new thread and continues executing the code after the thread block.

```spice
import "std/os/thread" as t;

// ...

thread {
    printf("Thread Id: %d", tid());
    // Do something
}
```

Within the thread block, the builtin function `tid()` can be used to obtain the so-called thread id (in the following called `tid`).
This is the id, which Spice assigned to the anonymous thread.

### Waiting for a thread to terminate
To wait for another thread to end its execution, the builtin `join(byte*...)` can be used. The program will suspend the execution
when calling `join` until the thread with the given tid has terminated.

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

### Captures
Variables from outside the thread, that are used within a thread are called `captures`. For being thread-safe, we need to know
whether it is only a reading capture or it also needs write access. If we write to a capture from within a thread, we need to mark
the allocation of the variable as `volatile`. This works, by marking a captured variable as `volatile` in the symbol table and
generate the corresponding volatile allocation at the point of declaration.

### Thread synchronization
To really become thread-safe, Spice needs support for Mutexes and synchronized functions/procedures/methods.

Synchronizing functions/procedures/methods could be achieved by providing the specifier `sync`, which can be attached to them and
mark them as synchronized. There could be an instance of `Mutex` for each occurrence of the `sync` keyword, that will track the
accessing threads. Mutexes could be realized with a `Mutex` struct in the runtime std lib for threading.

A minimalistic implementation could look like this:

```spice
import "std/os/cpu" as cpu;

type Mutex struct {
	bool occupied
}

p Mutex.acquire() {
	while this.occupied {
		cpu.yield();
	}
	this.occupied = true;
}

p Mutex.abandon() {
	this.occupied = false;
}
```

### Communication between threads (requires generics)
_Inspired by Goroutines and Channels from the Go programming language_

For the communication between threads, there is a feature, called `Pipes`. A pipe is a wrapper around any type and can act like a
temporary buffer queue for one or multiple (primitive or complex) values. Those values can be pushed by calling the builtin
function `stash(Pipe<any>, any)` and received by calling the `pick(Pipe<any>)` builtin. If `pick` is called on a pipe and this pipe
currently has no value present, the execution will pause until there is a new value for that pipe. 

```spice
f<int> main() {
    Pipe<int> intPipe;
    
    byte* t1 = thread {
        int stashValue = 12345;
        stash(intPipe, stashValue);
    };
    
    byte* t2 = thread {
        int receivedValue = pick(intPipe);
        printf("Received value: %d", receivedValue); // Output: 12345
    };
}
```

### Thread pools (long way off, not finalized, may change)
To support thread pools in Spice, a std module called `std/os/threadpool` is planned. <br>
We probably need Spice support for function pointers to realize thread pools efficiently. Furthermore, it would be useful to have
the `Queue` data structure to manage the tasks to execute. And for realizing Queues, we probably first need to support generics.

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

### Reference points in the implementation

- Thread definition node in ANTLR grammar: [here](https://github.com/spicelang/spice/blob/main/src/grammar/Spice.g4#L14)
- Semantic analysis of threads: [here](https://github.com/spicelang/spice/blob/main/src/analyzer/AnalyzerVisitor.cpp#L730)
- Code gen for threads: [here](https://github.com/spicelang/spice/blob/main/src/generator/GeneratorVisitor.cpp#L605)
- Tests for threads: [here](https://github.com/spicelang/spice/tree/main/test/test-files/analyzer/threads) and [here](https://github.com/spicelang/spice/tree/main/test/test-files/generator/threads)
- Fully fledged functional test: [here](https://github.com/spicelang/spice/tree/main/test/test-files/generator/arbitrary/success-fibonacci-threaded)
