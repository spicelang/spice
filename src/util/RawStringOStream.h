// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <llvm/Support/raw_ostream.h>

class RawStringOStream : public llvm::raw_pwrite_stream {
public:
  // Public constructors
  explicit RawStringOStream(std::string &output) : llvm::raw_pwrite_stream(true), output(output) {}
  virtual ~RawStringOStream() override;

private:
  // Private members
  std::string &output;

  // Private methods
  void pwrite_impl(const char *Ptr, size_t Size, uint64_t Offset) override;
  void write_impl(const char *Ptr, size_t Size) override;
  [[nodiscard]] uint64_t current_pos() const override;
};