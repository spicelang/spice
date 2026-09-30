// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <array>
#include <filesystem>
#include <optional>
#include <string>

namespace spice::testing {

const char *const PATH_BOOTSTRAP_COMPILER_ARTIFACTS = "./test-tmp/bootstrap-compiler";
const char *const BOOTSTRAP_SERIALIZED_AST_CAPTION = "Serialized ast:\n\n";

// Kinds of errors the bootstrap compiler can already raise. Errors of the other kinds (e.g. semantic errors) are raised by
// stages that are not ported yet, so a test expecting one of them is only checked if the bootstrap compiler raises an error.
// Extend this list as soon as the bootstrap compiler raises the respective errors.
static constexpr std::array<const char *, 2> BOOTSTRAP_SUPPORTED_ERROR_PREFIXES = {"[Error|Lexer]", "[Error|Parser]"};

// Names of the opt levels (in the order of the OptLevel enum) for the -O cli option
static constexpr std::array BOOTSTRAP_OPT_LEVEL_NAMES = {'0', '1', '2', '3', 's', 'z'};

class BootstrapUtil {
public:
  // Public static methods
  static bool buildBootstrapCompiler();
  static std::optional<std::string> extractErrorMessage(const std::string &output);
  static std::optional<std::string> extractSerializedAST(const std::string &output);
  static bool isErrorSupported(const std::filesystem::path &errorRefPath);
  static void eraseDSOLocalMarkers(std::string &irCode);
};

} // namespace spice::testing
