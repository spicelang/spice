// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "CompilerInstance.h"

#include <exception/LexerParserError.h>
#include <exception/SemanticError.h>
#include <exception/IRError.h>

/**
 * Entry point to the Spice compiler
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @return Return code
 */
int main(int argc, char** argv) { // Call ./spicec filePath targetArch targetVendor targetOs outputDir debugOutput optimizerLevel
    // Parse cli args
    std::vector<std::string> args;
    for (size_t i = 1; i < argc; i++)
        args.emplace_back(argv[i]);

    if (args.size() < 7)
        throw std::runtime_error("Call: ./spicec filePath targetArch targetVendor targetOs outputDir debugOutput optimizerLevel");

    // Extract args from cli
    std::string mainSourceFile = args[0];
    std::string targetArch = args[1];
    std::string targetVendor = args[2];
    std::string targetOs = args[3];
    std::string objectDir = args[4];
    bool debugOutput = args[5] == "true";
    int optLevel = std::stoi(args[6]);

    // Add relative prefix to filename
    if (mainSourceFile.find("/\\") != std::string::npos) mainSourceFile = "./" + mainSourceFile;

    /*
     * Compile main source file. All files, that are included by the main source file will call the 'compileSourceFile'
     * function again.
     */
    try {
        CompilerInstance::CompileSourceFile(mainSourceFile, targetArch, targetVendor, targetOs, objectDir, debugOutput,
                                            optLevel, true, false);
    } catch (SemanticError& e) {
        std::cout << e.what() << std::endl;
        std::exit(1); // Exit with result code other than 0
    } catch (IRError& e) {
        std::cout << e.what() << std::endl;
        std::exit(1); // Exit with result code other than 0
    } catch (LexerParserError& e) {
        std::cout << e.what() << std::endl;
        std::exit(1); // Exit with result code other than 0
    }
    return 0;
}