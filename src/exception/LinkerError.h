// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#pragma once

#include <string>

enum LinkerErrorType {
    LINKER_NOT_FOUND,
};

/**
 * Custom exception for errors, occurring when linking the output executable
 */
class LinkerError : public std::exception {
public:
    // Constructors
    explicit LinkerError(const LinkerErrorType& type, const std::string& message);

    // Public methods
    [[nodiscard]] const char* what() const noexcept override;

private:
    // Members
    std::string errorMessage;

    [[nodiscard]] std::string getMessagePrefix(LinkerErrorType errorType) const;
};