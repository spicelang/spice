// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "SemanticError.h"

const char *SemanticError::what() const noexcept {
    return errorMessage.c_str();
}