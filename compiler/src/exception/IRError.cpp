// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "IRError.h"

const char *IRError::what() const noexcept {
    return errorMessage.c_str();
}
