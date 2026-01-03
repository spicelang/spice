// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#include "RawStringOStream.h"

namespace spice::compiler {

// LCOV_EXCL_START
RawStringOStream::RawStringOStream(std::string &output) : raw_pwrite_stream(true), output(output) {}

void RawStringOStream::pwrite_impl(const char *Ptr, const size_t Size, const uint64_t Offset) {
  assert(output.length() >= Offset + Size);
  std::memcpy(output.data() + Offset, Ptr, Size);
}

void RawStringOStream::write_impl(const char *Ptr, const size_t Size) { output.append(Ptr, Size); }

uint64_t RawStringOStream::current_pos() const { return output.size(); }
// LCOV_EXCL_STOP

} // namespace spice::compiler