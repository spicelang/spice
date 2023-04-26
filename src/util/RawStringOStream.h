// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#pragma once

#include <llvm/Support/raw_ostream.h>

namespace spice::compiler {

class RawStringOStream : public llvm::raw_pwrite_stream {
public:
  // Public constructors
  explicit RawStringOStream(std::string &output);

  // Public destructor
  // virtual ~RawStringOStream() override;

  // Public methods
  /*raw_ostream &changeColor(enum Colors Color, bool Bold, bool BG) override { return *this; }
  raw_ostream &resetColor() override { return *this; }
  raw_ostream &reverseColor() override { return *this; }
  [[nodiscard]] size_t preferred_buffer_size() const override;*/

private:
  // Private members
  std::string &output;

  // Private methods
  void pwrite_impl(const char *Ptr, size_t Size, uint64_t Offset) override;
  void write_impl(const char *Ptr, size_t Size) override;
  [[nodiscard]] uint64_t current_pos() const override;
};

} // namespace spice::compiler