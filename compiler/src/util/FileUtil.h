// Copyright (c) 2021 ChilliBits. All rights reserved.

#pragma once

#include <string>
#include <iostream>
#include <fstream>

class FileUtil {
public:
    static bool fileExists(const std::string&);
    static std::string getFileName(const std::string&);
};
