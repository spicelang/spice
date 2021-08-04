package cmd

import (
	"spice/internal"
	"strings"
)

// Build takes the passed code file, resolves its dependencies and emits an executable, representing its functionality
func Build(sourceFile string, targetTriple string, outputPath string, debugOutput bool) {
	// Set default value for outputPath
	if outputPath == "" {
		outputPath = "./output.o"
	} else if !strings.HasSuffix(outputPath, ".o") {
		outputPath += ".o"
	}

	// Compile program ane emit executable to output file
	internal.Compile(sourceFile, targetTriple, outputPath, debugOutput)
}
