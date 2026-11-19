---
title: Sanitizers
---

Spice gives you memory safety in most of your code, but `unsafe` blocks, raw pointers, C interop and threads still leave
room for bugs that do not show up until much later, if at all: a write one element past the end of a buffer, a read of
freed memory or two threads updating the same variable. Sanitizers catch these bugs while the program runs. The compiler
instruments the generated code with extra checks, and a runtime library reports the first violation, together with the
stack traces that lead to it.

Spice offers four sanitizers, all based on the ones of LLVM:

| Sanitizer                               | Option                  | Finds                                                                       |
|-----------------------------------------|-------------------------|-----------------------------------------------------------------------------|
| [AddressSanitizer](#addresssanitizer)   | `--sanitizer=address`   | Out-of-bounds accesses, use after free, use after scope, memory leaks       |
| [ThreadSanitizer](#threadsanitizer)     | `--sanitizer=thread`    | Data races between threads                                                  |
| [MemorySanitizer](#memorysanitizer)     | `--sanitizer=memory`    | Reads of uninitialized memory (Linux only)                                  |
| [TypeSanitizer](#typesanitizer)         | `--sanitizer=type`      | Accesses to an object through a pointer of an incompatible type (aliasing) |

## Enabling a sanitizer

The `--sanitizer` option is available on the [`build`](../cli/build.md), [`run`](../cli/run.md) and
[`test`](../cli/test.md) subcommands. Pass it together with `-g`, so that the reports show the file, line and column of
every frame instead of only the function name:

```sh
$ spice build -g --sanitizer=address main.spice -o main
$ ./main
```

`spice run` builds and runs the program in one step, and `spice test` runs the `#[test]` functions of a file with the
instrumentation enabled:

```sh
$ spice run -g --sanitizer=thread main.spice
$ spice test -g --sanitizer=address main.spice
```

A few things to keep in mind:

- **One sanitizer at a time.** Each of them comes with its own runtime, and they cannot be combined. Build once per
  sanitizer, if you want to use more than one.
- **Some options are implied.** AddressSanitizer and MemorySanitizer turn on `--use-lifetime-markers`, so that they know
  when a local variable goes out of scope. TypeSanitizer turns on `--use-tbaa-metadata`, since this metadata tells it the
  type of every memory access.
- **Instrument with few optimizations.** Without `-O` options, Spice builds with `-O0`, which is the best fit for
  sanitizer runs: the reports point to the right source locations, and the optimizer cannot remove the bug before the
  sanitizer gets to see it. `-O1` is fine as well, if the instrumented program is too slow. See the
  [MemorySanitizer](#memorysanitizer) section for an example of a bug that `-O2` hides.
- **The runtime comes from your C toolchain.** The compiler links the program with `-fsanitize=<sanitizer>`, so the
  sanitizer runtime is the one of the Clang or GCC installation, that Spice uses as linker invoker. Spice prefers Clang
  and falls back to GCC. GCC does not implement MemorySanitizer and TypeSanitizer, so those two need Clang.
- **Instrumented programs are slower and use more memory.** Use the sanitizers to test and debug your program, not in
  the builds you ship.

## AddressSanitizer

AddressSanitizer (ASan) finds the most common memory bugs: accesses outside of heap, stack or global objects, accesses to
memory after it was freed, accesses to local variables after their scope ended and, on Linux, memory leaks. It slows a
program down by a factor of about two, which is why it is the sanitizer to reach for first.

### Out-of-bounds access

The loop below writes five elements into a buffer that only has room for four:

```spice
f<int> main() {
    unsafe {
        heap int* numbers = cast<heap int*>(sAllocUnsafe(4l * sizeof<int>()));
        for int i = 0; i <= 4; i++ {
            numbers[i] = i * 10;
        }
        sDealloc(cast<heap byte*>(numbers));
    }
}
```

Without instrumentation, the program exits normally: the write lands in memory that happens to belong to nobody yet.
Built with `-g --sanitizer=address`, it stops at the first invalid write:

```text
=================================================================
==202917==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x6e070f7e0020 at pc 0x596255fe23b3 bp 0x7ffc988090b0 sp 0x7ffc988090a8
WRITE of size 4 at 0x6e070f7e0020 thread T0
    #0 0x596255fe23b2 in main overflow.spice:5:30
    #1 0x71e71042a1c9 in __libc_start_call_main csu/../sysdeps/nptl/libc_start_call_main.h:58:16
    #2 0x71e71042a28a in __libc_start_main csu/../csu/libc-start.c:360:3
    #3 0x596255ed97e4 in _start (/home/me/overflow+0x7f7e4)

0x6e070f7e0020 is located 0 bytes after 16-byte region [0x6e070f7e0010,0x6e070f7e0020)
allocated by thread T0 here:
    #0 0x596255f88024 in malloc
    #1 0x596255fe12f8 in sAllocUnsafe(unsigned long) /usr/lib/spice/std/runtime/memory_rt.spice:32:29
    #2 0x596255fe233b in main overflow.spice:3:63
    ...

SUMMARY: AddressSanitizer: heap-buffer-overflow overflow.spice:5:30 in main
Shadow bytes around the buggy address:
  ...
==202917==ABORTING
```

Read a report from the top:

1. The first line names the kind of bug, here `heap-buffer-overflow`.
2. The first stack trace shows where the invalid access happened: line 5, column 30 of `overflow.spice`, the
   `numbers[i]` subscript.
3. The paragraph below describes the memory that was accessed: the address lies right behind a 16-byte region, i.e. the
   four `int`s, and the second stack trace shows where that region was allocated.
4. The `SUMMARY` line repeats the most important facts in one line, which is handy to search for in long logs.

The shadow bytes at the end show ASan's view of the memory around the address. You rarely need them.

The program exits with exit code `1` after the first report.

### Use after free

A pointer that outlives the memory it points to is a dangling pointer. Reading through it is undefined behavior, which
often goes unnoticed, since the freed memory still holds the old value for a while:

```spice
f<int> main() {
    heap int* value = __new<int>();
    *value = 42;
    int* copy = value;
    unsafe {
        sDealloc(cast<heap byte*>(value));
    }
    printf("Value: %d\n", *copy);
}
```

ASan reports a `heap-use-after-free` and adds the stack trace of the deallocation to the one of the allocation:

```text
==204066==ERROR: AddressSanitizer: heap-use-after-free on address 0x76f02d7e0010 at pc 0x56f26ef354c8 bp 0x7fff80f40e90 sp 0x7fff80f40e88
READ of size 4 at 0x76f02d7e0010 thread T0
    #0 0x56f26ef354c7 in main use-after-free.spice:8:28
    ...

0x76f02d7e0010 is located 0 bytes inside of 4-byte region [0x76f02d7e0010,0x76f02d7e0014)
freed by thread T0 here:
    #0 0x56f26eedab16 in free
    #1 0x56f26ef35307 in sDealloc(unsigned char volatile*&) /usr/lib/spice/std/runtime/memory_rt.spice:134:10
    ...

previously allocated by thread T0 here:
    #0 0x56f26eedb0e4 in malloc
    #1 0x56f26ef343b8 in sAllocUnsafe(unsigned long) /usr/lib/spice/std/runtime/memory_rt.spice:32:29
    #2 0x56f26ef353f7 in main use-after-free.spice:2:23
    ...
```

### Use after scope

The same goes for a pointer to a local variable, whose scope already ended:

```spice
f<int> main() {
    int* ptr = nil<int*>;
    {
        int local = 7;
        ptr = &local;
    }
    printf("Value: %d\n", *ptr);
}
```

```text
==203701==ERROR: AddressSanitizer: stack-use-after-scope on address 0x7448404f0020 at pc 0x571551941523 bp 0x7ffc672cc970 sp 0x7ffc672cc968
READ of size 4 at 0x7448404f0020 thread T0
    #0 0x571551941522 in main use-after-scope.spice:7:28
    ...
```

This check relies on the lifetime markers, which `--sanitizer=address` turns on for you.

### Memory leaks

On Linux, ASan includes LeakSanitizer. When the program exits, it looks for heap memory, that is no longer reachable from
anywhere, and reports where it was allocated. Variables with the [`heap` qualifier](../language/declaration-qualifiers.md#the-heap-qualifier)
are freed automatically when they go out of scope, so a leak typically comes from memory that was allocated through
C interop and never handed back:

```spice
ext f<byte*> malloc(unsigned long);

f<int> main() {
    unsafe {
        byte* buffer = malloc(64l);
        printf("Allocated %p\n", buffer);
    }
}
```

```text
Allocated 0x6ebe903e0020

=================================================================
==203155==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 64 byte(s) in 1 object(s) allocated from:
    #0 0x63e2c73f3764 in malloc
    #1 0x63e2c743541a in main leak.spice:5:31
    ...

SUMMARY: AddressSanitizer: 64 byte(s) leaked in 1 allocation(s).
```

A program with a leak exits with exit code `1`, even though it ran to the end. Set `ASAN_OPTIONS=detect_leaks=0` to turn
the leak check off, e.g. while you hunt a different bug.

## ThreadSanitizer

ThreadSanitizer (TSan) finds data races: two threads access the same memory at the same time, at least one of them
writes, and nothing orders the two accesses, like a [mutex](../language/threads.md#mutexes) does. Data races are notoriously
hard to debug, since they depend on the timing of the threads and may disappear as soon as you add a `printf` or attach
a debugger. TSan detects them, even if the race did not lead to a wrong result in this particular run.

Both threads below increment the same global counter without synchronization:

```spice
import "std/os/thread";

int COUNTER = 0;

p increment() {
    for int i = 0; i < 100000; i++ {
        COUNTER++;
    }
}

f<int> main() {
    Thread first = Thread(increment);
    Thread second = Thread(increment);
    first.run();
    second.run();
    first.join();
    second.join();
    printf("Counter: %d\n", COUNTER);
}
```

Built with `-g --sanitizer=thread`, the report shows both conflicting accesses, the variable they access and where the
involved threads were started:

```text
==================
WARNING: ThreadSanitizer: data race (pid=204199)
  Write of size 4 at 0x555556a30ca4 by thread T2:
    #0 increment() data-race.spice:7:9 (data-race+0x149bd4)
    #1 <null> <null> (data-race+0x149cff)

  Previous write of size 4 at 0x555556a30ca4 by thread T1:
    #0 increment() data-race.spice:7:9 (data-race+0x149bd4)
    #1 <null> <null> (data-race+0x149cff)

  Location is global 'COUNTER' of size 4 at 0x555556a30ca4 (data-race+0x14dcca4)

  Thread T2 (tid=204203, running) created by main thread at:
    #0 pthread_create
    #1 Thread::run() /usr/lib/spice/std/os/thread.spice:54:61 (data-race+0x1499d6)
    #2 main data-race.spice:15:5 (data-race+0x149c96)

  Thread T1 (tid=204202, finished) created by main thread at:
    #0 pthread_create
    #1 Thread::run() /usr/lib/spice/std/os/thread.spice:54:61 (data-race+0x1499d6)
    #2 main data-race.spice:14:5 (data-race+0x149c8c)

SUMMARY: ThreadSanitizer: data race data-race.spice:7:9 in increment()
==================
Counter: 197013
ThreadSanitizer: reported 1 warnings
```

Unlike ASan, TSan does not stop the program at the first report. It reports every race once, lets the program finish
and then exits with exit code `66`. The counter also shows what the race costs: 2,987 increments got lost.

To fix the race, guard the counter with a `Mutex` from `std/os/mutex`. A `LockGuard` acquires the mutex in its
constructor and releases it in its destructor, i.e. at the end of every loop round:

```spice
import "std/os/thread";
import "std/os/mutex";

int COUNTER = 0;

p increment(Mutex& mutex) {
    for int i = 0; i < 100000; i++ {
        LockGuard _ = LockGuard(mutex);
        COUNTER++;
    }
}

f<int> main() {
    Mutex mutex;
    Thread first = Thread(p() { increment(mutex); });
    Thread second = Thread(p() { increment(mutex); });
    first.run();
    second.run();
    first.join();
    second.join();
    printf("Counter: %d\n", COUNTER);
}
```

TSan stays silent now, and the program prints `Counter: 200000` every time.

## MemorySanitizer

MemorySanitizer (MSan) finds reads of uninitialized memory. It does not report the read itself, since copying
uninitialized memory around is harmless. Instead, it tracks which bits of memory are initialized and reports the first
time an uninitialized value decides something: a branch, a pointer that gets dereferenced or a call argument. Spice
enables the eager checks of MSan, so passing an uninitialized value to a function or returning one is reported right
there, instead of where the value is used later.

```spice
ext f<byte*> malloc(unsigned long);
ext p free(byte*);

f<int> main() {
    unsafe {
        int* values = cast<int*>(malloc(4l * sizeof<int>()));
        values[0] = 1;
        values[1] = 2;
        // values[2] and values[3] are never written
        int sum = 0;
        for int i = 0; i < 4; i++ {
            sum += values[i];
        }
        if sum > 10 {
            printf("Large sum\n");
        }
        free(cast<byte*>(values));
    }
}
```

Adding up the values is fine, but the `if` on line 14 depends on them. MSan reports it and exits with exit code `1`:

```text
==205606==WARNING: MemorySanitizer: use-of-uninitialized-value
    #0 0x5b852cbf4191 in main uninitialized.spice:14:18
    ...

SUMMARY: MemorySanitizer: use-of-uninitialized-value uninitialized.spice:14:18 in main
Exiting
```

MSan comes with a few restrictions:

- **Linux only.** The compiler rejects `--sanitizer=memory` for other targets.
- **Clang only.** GCC does not ship an MSan runtime, so Clang has to be installed.
- **Everything has to be instrumented.** MSan knows nothing about memory written by code, that was not compiled with
  MSan, so it reports false positives for values coming from such code. The C standard library is covered by the
  runtime, but other C or C++ libraries you link against are not, unless you build them with `-fsanitize=memory`
  yourself.
- **Mind the optimization level.** Built with `-O2`, the example above passes without a report. The optimizer is
  allowed to assume any value for the uninitialized elements, so it removes the check, before MSan instruments the
  code. Clang behaves the same way for the equivalent C program. Stay at the default `-O0` or at `-O1` for MSan runs.

## TypeSanitizer

TypeSanitizer (TySan) finds violations of the strict aliasing rules: accessing an object through a pointer of an
incompatible type. The optimizer relies on these rules, as it assumes that a `double*` and a `long*` never point to the
same memory. If they do, the optimized program may read stale values or reorder writes in surprising ways. This kind of
bug typically hides in `unsafe` code, that reinterprets the bytes of a value with a pointer cast:

```spice
f<int> main() {
    long bits = 4607182418800017408l;
    unsafe {
        double* asDouble = cast<double*>(&bits);
        printf("Value: %f\n", *asDouble);
    }
}
```

```text
==206406==ERROR: TypeSanitizer: type-aliasing-violation on address 0x7ffec8f86ee0 (pc 0x58a9bcd37a88 bp 0x7ffec8f86ec0 sp 0x7ffec8f86e48 tid 206406)
READ of size 8 at 0x7ffec8f86ee0 with type double accesses an existing object of type long
    #0 0x58a9bcd37a87 in main type-punning.spice:5:32

Value: 1.000000
```

By default, TySan only reports the violation and lets the program continue, which then exits with its regular exit code.
Set `TYSAN_OPTIONS=halt_on_error=1` to stop at the first report with exit code `1` instead, e.g. in CI.

To reinterpret the bytes of a value without breaking the aliasing rules, copy them into a variable of the target type.
`memcpy` is allowed to access any memory, and the optimizer turns the copy into a plain register move:

```spice
ext f<byte*> memcpy(byte*, const byte*, unsigned long);

f<int> main() {
    long bits = 4607182418800017408l;
    double value = 0.0;
    unsafe {
        byte* _ = memcpy(cast<byte*>(&value), cast<const byte*>(&bits), sizeof<long>());
    }
    printf("Value: %f\n", value);
}
```

Like MSan, TySan needs Clang as linker invoker. TypeSanitizer is a recent addition to LLVM and still considered
experimental there, so expect it to miss some violations.

## Runtime options

Every sanitizer runtime reads its options from an environment variable, when the program starts. Separate multiple
options with `:`:

```sh
$ ASAN_OPTIONS=detect_leaks=0:halt_on_error=1 ./main
```

| Sanitizer        | Environment variable | Useful options                                                                         |
|------------------|----------------------|----------------------------------------------------------------------------------------|
| AddressSanitizer | `ASAN_OPTIONS`       | `detect_leaks=0`, `detect_stack_use_after_return=1`, `log_path=<file>`                 |
| LeakSanitizer    | `LSAN_OPTIONS`       | `suppressions=<file>` to ignore known leaks, e.g. in a third-party library             |
| ThreadSanitizer  | `TSAN_OPTIONS`       | `halt_on_error=1`, `exitcode=<code>`, `suppressions=<file>`                            |
| MemorySanitizer  | `MSAN_OPTIONS`       | `exitcode=<code>`, `log_path=<file>`                                                   |
| TypeSanitizer    | `TYSAN_OPTIONS`      | `halt_on_error=1`                                                                      |

Pass `help=1` in any of them to make the program print all options of that runtime. The Clang documentation of
[AddressSanitizer](https://clang.llvm.org/docs/AddressSanitizer.html),
[LeakSanitizer](https://clang.llvm.org/docs/LeakSanitizer.html),
[ThreadSanitizer](https://clang.llvm.org/docs/ThreadSanitizer.html),
[MemorySanitizer](https://clang.llvm.org/docs/MemorySanitizer.html) and
[TypeSanitizer](https://clang.llvm.org/docs/TypeSanitizer.html) covers them in detail.

## Limitations

- **Freestanding builds** cannot use sanitizers, since their runtimes need a hosted libc. See
  [Freestanding builds](freestanding.md).
- **The [TPDE backend](experimental-backends.md)** does not support sanitizers, so the compiler rejects `--sanitizer` in
  combination with `--backend=tpde`. Use the default LLVM backend for instrumented builds.
- **Uninstrumented code** is invisible to the sanitizers. Bugs inside a C library you link against, that was not built
  with the same sanitizer, go unnoticed, and with MemorySanitizer they even cause false positives.
- **Sanitizers only see the code that runs.** A clean sanitizer run tells you that this particular run had no bugs of
  that kind, not that the program is free of them. Run the sanitizers over your tests, so that they cover as many code
  paths as possible.
