// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "RawStringOStream.h"

namespace spice::compiler {

RawStringOStream::RawStringOStream(std::string &output) : llvm::raw_pwrite_stream(true), output(output) {}

/*size_t RawStringOStream::preferred_buffer_size() const {
#ifdef _WIN32
  // On Windows BUFSIZ is only 512 which results in more calls to write. This
  // overhead can cause significant performance degradation. Therefore, use a
  // better default.
  return 16 * 1024;
#else
  // BUFSIZ is intended to be a reasonable default.
  return BUFSIZ;
#endif
}

RawStringOStream::~RawStringOStream() { flush(); }*/

void RawStringOStream::pwrite_impl(const char *Ptr, size_t Size, uint64_t Offset) {
  assert(output.length() >= Offset + Size);
  std::memcpy((void *)(output.c_str() + Offset), Ptr, Size);
}

void RawStringOStream::write_impl(const char *Ptr, size_t Size) { output.append(Ptr, Size); }

uint64_t RawStringOStream::current_pos() const { return output.size(); }

} // namespace spice::compiler