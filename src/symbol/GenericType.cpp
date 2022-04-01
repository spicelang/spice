// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "GenericType.h"
GenericType::GenericType(const std::string &name) {
  this->name = name;
  this->typeChain.push({TY_GENERIC, name});
}