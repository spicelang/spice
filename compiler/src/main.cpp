// Copyright (c) 2021-2022 ChilliBits. All rights reserved.

#include "CompilerInstance.h"

#include <stdexcept>

#include <cli/CliInterface.h>
#include <util/ThreadFactory.h>
#include <exception/LexerParserError.h>
#include <exception/SemanticError.h>
#include <exception/IRError.h>

#include <llvm/IR/IRBuilder.h>

#include "../lib/cli11/CLI11.hpp"

/**
 * Compile main source file. All files, that are included by the main source file will call the 'compileSourceFile'
 * function again.
 */
void compileProject(CliOptions* options) {
    // Add relative prefix to filename
    if (options->mainSourceFile.find("/\\") != std::string::npos) options->mainSourceFile = "./" + options->mainSourceFile;

    try {
        // Prepare global LLVM assets
        std::shared_ptr<llvm::LLVMContext> context = std::make_shared<llvm::LLVMContext>();
        std::shared_ptr<llvm::IRBuilder<>> builder = std::make_shared<llvm::IRBuilder<>>(*context);

        // Prepare instance of module registry and thread factory, which have to exist exactly once per executable
        ModuleRegistry moduleRegistry = ModuleRegistry();
        ThreadFactory threadFactory = ThreadFactory();

        // Compile main source file
        CompilerInstance::CompileSourceFile(context, builder, &moduleRegistry, &threadFactory, options,
                                            options->mainSourceFile, true, false);
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
}

/**
 * Entry point to the Spice compiler
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @return Return code
 */
int main(int argc, char** argv) {
    // Initialize command line parser
    CliInterface cli{};
    cli.createInterface();

    // Parser cli arguments
    CLI11_PARSE(cli.getApp(), argc, argv)

    compileProject(cli.getOptions());

    return 0;
}