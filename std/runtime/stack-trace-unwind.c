/* Frame capture for POSIX targets, backing captureReturnAddresses() in stack_trace_capture_rt.spice.
 *
 * Written in C because _Unwind_Backtrace takes a callback, and a Spice function converted to a raw pointer is
 * not a bare function pointer: it becomes a '.fatthunk' carrying an extra leading capture-struct pointer, which
 * shifts every argument (spicelang/spice#1392). A C callback has no such problem.
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
