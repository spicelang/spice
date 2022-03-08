# Technical Specification for Multithreading in Spice

## Syntax

### Push work to a new anonymous thread
This enables the programmer to execute portions of code in another thread that runs in parallel to the original one. The block immediately returns after launching the new thread and continues executing the code after the thread block.

```spice
thread -1 {
    printf("Thread Id: %d", tid);
    // Do something
}
```

Later, there will be a constant in the stdlib which will hold the `-1` and can be used like this:

```spice
import "std/os/threading" as t;

// ...

thread t.ANONYMOUS {
    printf("Thread Id: %d", tid);
    // Do something
}
```

Within the thread block, the variable `tid` can be used to obtain the so-called thread id (in the following called `tid`). This is the id, which Spice assigned to the anonymous thread. To set this id manually, numbered threads can be used as described below.

> Using a magic value for `tid` seems the best solution for now. Providing a builtin e.g. `int getThreadId()` could be called from another function or another module which would produce wrong values with the current architecture.

### Push work to a (new) numbered thread
Same as above, but it pushes the work to the thread with the tid `3`. If the thread with this id does not exist, Spice will create and run it  automatically straight away. If the thread was already created, Spice will simply append the given work to the end of the execution queue of this thread.

```spice
thread 3 {
    // Do something
}
```

### Waiting for a thread to terminate
To wait for another thread to end its execution, the builtin `wait(int)` can be used. The program will suspend the execution when calling `wait` until the thread with the given tid has terminated.

```spice
thread 1 {
    wait(2);
    printf("Thread 1");
}

thread 2 {
    printf("Thread 2");
}
```

### Communication between threads
For the communication between threads, there is a feature, called `Pipes`. A pipe is a wrapper around any type and can act like a temporary buffer queue for one or multiple (primitive or complex) values. Those values can be pushed by calling the builtin function `yield(pipe<any>, any)` and received by calling the `pick(pipe<any>)` builtin. If `pick` is called on a pipe and this pipe currently has no value present, the execution will pause until there is a new value for that pipe. 

```spice
pipe<int> intPipe;

thread 1 {
    int yieldValue = 12345;
    yield(intPipe, yieldValue);
}

thread 2 {
    int receivedValue = pick(intPipe);
    printf("Received value: %d", receivedValue); // Output: 12345
}
```