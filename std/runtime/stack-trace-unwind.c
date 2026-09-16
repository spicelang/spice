/* Stack trace capture for POSIX targets, backing captureReturnAddresses() in
 * stack_trace_capture_rt.spice.
 *
 * This exists as C rather than Spice for two reasons, both of which are properties of Spice today rather than
 * preferences:
 *
 *   1. _Unwind_Backtrace takes a callback, and a Spice function converted to a raw pointer is not a bare
 *      function pointer - IRGenerator wraps it in a '.fatthunk' carrying an extra leading capture-struct
 *      pointer, so the unwinder's (context, arg) arrives as (captures, context) and every argument shifts by
 *      one. See spicelang/spice#1392. A C callback has no such problem.
 *
 *   2. The obvious callback-free alternative, backtrace(3), is not equivalent across platforms. glibc's
 *      implementation goes through this same unwinder, but Apple's walks the frame-pointer chain - and since
 *      Spice omits frame pointers unless --keep-frame-pointers is passed, it finds nothing there. That is
 *      exactly the dependency the move to the unwinder was meant to remove, so it cannot be relied on.
 *
 * Going through _Unwind_Backtrace directly avoids both. It is driven by the unwind tables that IRGenerator's
 * 'uwtable' attribute always emits, so it needs no frame pointers, and it is available wherever a C++ ABI
 * unwinder is - libgcc_s on Linux, the libunwind inside libSystem on macOS, and musl, which has no
 * <execinfo.h> and therefore no backtrace(3) at all.
 */

#include <stddef.h>
#include <stdint.h>
#include <unwind.h>

struct SpiceUnwindState {
  void **addresses;
  int capacity;
  int count;
  int skipFrames;
};

/* Invoked once per frame, innermost first. */
static _Unwind_Reason_Code spiceCollectFrame(struct _Unwind_Context *context, void *arg) {
  struct SpiceUnwindState *state = (struct SpiceUnwindState *)arg;

  const uintptr_t instructionPointer = (uintptr_t)_Unwind_GetIP(context);
  if (instructionPointer == 0)
    return _URC_END_OF_STACK;

  if (state->skipFrames > 0) {
    state->skipFrames--;
    return _URC_NO_REASON;
  }
  if (state->count >= state->capacity)
    return _URC_END_OF_STACK; /* buffer full - stop rather than overwrite the frames nearest the capture */

  state->addresses[state->count++] = (void *)instructionPointer;
  return _URC_NO_REASON;
}

/* Fills 'addresses' with up to 'capacity' return addresses, most recent call first, and returns how many were
 * written. Frame 0 is this function, so a caller that wants its own frame hidden passes skipFrames >= 1. */
int spiceCaptureReturnAddresses(void **addresses, int capacity, int skipFrames) {
  if (addresses == NULL || capacity <= 0 || skipFrames < 0)
    return 0;

  struct SpiceUnwindState state;
  state.addresses = addresses;
  state.capacity = capacity;
  state.count = 0;
  state.skipFrames = skipFrames;

  _Unwind_Backtrace(spiceCollectFrame, &state);
  return state.count;
}
