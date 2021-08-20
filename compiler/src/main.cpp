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
    std::string mainSourceFile = args[1];
    std::string targetTriple = args[2]; // Default for dev host: x86_64-w64-windows-gnu
    std::string objectDir = args[3];
    bool debugOutput = args[4] == "true";
    int optLevel = std::stoi(args[5]);

    // Add relative prefix to filename
    if (mainSourceFile.find("/\\") != std::string::npos)
        mainSourceFile = "./" + mainSourceFile;

    /*
     * Compile main source file. All files, that are included by the main source file will call the 'compileSourceFile'
     * function again.
     */
    try {
        CompilerInstance::CompileSourceFile(mainSourceFile, targetTriple, objectDir, debugOutput, optLevel, true);
    } catch (SemanticError& e) {
        std::cout << e.what() << std::endl;
        std::exit(1); // Exit with negative result code
    } catch (IRError& e) {
        std::cout << e.what() << std::endl;
        std::exit(1); // Exit with negative result code
    }

    return 0;
}