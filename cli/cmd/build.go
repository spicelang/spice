package cmd

import (
	"spice/internal"
	"strings"
)

func Build(sourceFile string, targetTriple string, outputPath string) {
	// Set default value for outputPath
	if outputPath == "" {
		outputPath = "./output.o"
	} else if !strings.HasSuffix(outputPath, ".o") {
		outputPath += ".o"
	}

	// Compile program ane emit executable to output file
	internal.Compile(sourceFile, targetTriple, outputPath)
}
