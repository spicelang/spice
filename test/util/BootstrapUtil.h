// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <array>
#include <filesystem>
#include <optional>
#include <string>

namespace spice::testing {

const char *const PATH_BOOTSTRAP_COMPILER_ARTIFACTS = "./test-tmp/bootstrap-compiler";
const char *const BOOTSTRAP_SERIALIZED_AST_CAPTION = "Serialized AST:\n\n";

// Names of the opt levels (in the order of the OptLevel enum) for the -O cli option
static constexpr std::array BOOTSTRAP_OPT_LEVEL_NAMES = {'0', '1', '2', '3', 's', 'z'};

class BootstrapUtil {
public:
  // Public static methods
  static bool buildBootstrapCompiler();
  static std::optional<std::string> extractErrorMessage(const std::string &output);
  static std::optional<std::string> extractSerializedAST(const std::string &output);
  static std::string extractWarnings(const std::string &output);
  static void eraseDSOLocalMarkers(std::string &irCode);
  static void normalizeProducerString(std::string &irCode);
  static std::string stripAnsiCodes(const std::string &text);
};

} // namespace spice::testing
