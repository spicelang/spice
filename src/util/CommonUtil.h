// Copyright (c) 2021-2026 ChilliBits. All rights reserved.

#pragma once

#include <string>

#include <Token.h>

// Forward declarations
namespace llvm {
class Triple;
} // namespace llvm

namespace spice::compiler {

// Forward declarations
class SourceFile;

// Compile time check, if T is a vector of a class, that is derived from BaseT
template <typename T, typename BaseT> struct is_vector_of_derived_from {
  using ElTy = std::remove_pointer_t<typename T::value_type>;
  static constexpr bool value = std::is_base_of_v<BaseT, ElTy> && std::is_same_v<T, std::vector<typename T::value_type>>;
};
template <typename T, typename BaseT> constexpr bool is_vector_of_derived_from_v = is_vector_of_derived_from<T, BaseT>::value;

/**
 * Util for general simplification of tasks
 */
class CommonUtil {
public:
  static void replaceAll(std::string &haystack, const std::string &needle, const std::string &replacement);
  static std::string getLastFragment(const std::string &haystack, const std::string &needle);
  static std::string trim(const std::string &input);
  static std::vector<std::string> split(const std::string &input);
  static size_t getSystemPageSize();
  static std::string formatBytes(size_t bytes);
  static std::string demangleTypeName(const char *mangledName);
  static bool isValidMangledName(const std::string &mangledName);
  static std::string getCircularImportMessage(std::stack<const SourceFile *> &sourceFiles);
  static int getCurrentYear();
  static std::string buildVersionInfo();
};

} // namespace spice::compiler