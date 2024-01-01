// Copyright (c) 2021-2024 ChilliBits. All rights reserved.

#include "RawStringOStream.h"

namespace spice::compiler {

RawStringOStream::RawStringOStream(std::string &output) : llvm::raw_pwrite_stream(true), output(output) {}

void RawStringOStream::pwrite_impl(const char *Ptr, size_t Size, uint64_t Offset) {
  assert(output.length() >= Offset + Size);
  std::memcpy((void *)(output.c_str() + Offset), Ptr, Size);
}

void RawStringOStream::write_impl(const char *Ptr, size_t Size) { output.append(Ptr, Size); }

uint64_t RawStringOStream::current_pos() const { return output.size(); }

} // namespace spice::compiler