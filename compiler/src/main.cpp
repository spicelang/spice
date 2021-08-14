// Copyright (c) 2021 ChilliBits. All rights reserved.

#include "CompilerInstance.h"

/**
 * Entry point to the Spice compiler
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @return Return code
 */
int main(int argc, char** argv) { // Call ./spicec filePath targetTriplet outputDir debugOutput optimizerLevel
    // Parse cli args
    std::vector<std::string> args;
    for (size_t iArg = 0; iArg < argc; ++iArg)
        args.emplace_back(argv[iArg]);

    // Extract args from cli
    std::string filePath = args[1];
    std::string targetTriple = args[2]; // Default for dev host: x86_64-w64-windows-gnu
    std::string outputDir = args[3];
    bool debugOutput = args[4] == "true";
    int optimizerLevel = std::stoi(args[5]);

    /*
     * Compile main source file. All files, that are included by the main source file will call the 'compileSourceFile'
     * function again.
     */
    CompilerInstance::CompileSourceFile(filePath, targetTriple, outputDir, debugOutput, optimizerLevel, true);

    return 0;
}