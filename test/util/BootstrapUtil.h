// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <array>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace spice::testing {

const char *const PATH_BOOTSTRAP_COMPILER_ARTIFACTS = "./test-tmp/bootstrap-compiler";
const char *const BOOTSTRAP_GRAPH_NAME_AST = "AST";
const char *const BOOTSTRAP_GRAPH_NAME_DEP_GRAPH = "Dependency Graph";

// Names of the opt levels (in the order of the OptLevel enum) for the -O cli option
static constexpr std::array BOOTSTRAP_OPT_LEVEL_NAMES = {'0', '1', '2', '3', 's', 'z'};

class BootstrapUtil {
public:
  // Public static methods
  static bool buildBootstrapCompiler();
  static std::optional<std::string> extractErrorMessage(const std::string &output);
  static bool containsSanitizerReport(const std::string &output);
  static std::optional<std::string> extractSerializedGraph(const std::string &output, const char *graphName);
  static std::string extractWarnings(const std::string &output);
  static void eraseDSOLocalMarkers(std::string &irCode);
  static void normalizeProducerString(std::string &code);
  static std::string stripAnsiCodes(const std::string &text);
  static bool emitsDebugInfo(const std::vector<std::string> &args);
};

} // namespace spice::testing
