package cmd

import (
	"os"
	"path/filepath"
	"runtime"
	"spice/internal"
	"strings"
)

// Build takes the passed code file, resolves its dependencies and emits an executable, representing its functionality
func Build(sourceFile string, targetTriple string, outputPath string, debugOutput bool) {
	sourceFileName := filepath.Base(sourceFile)
	sourceFileNameWithoutExt := strings.TrimSuffix(sourceFileName, filepath.Ext(sourceFileName))

	// Get temp dir as location for object files
	tmpDir := os.TempDir()
	objectPath := tmpDir + "/spice-output.o"

	// Compile program ane emit object file to temp dir
	internal.Compile(sourceFile, targetTriple, objectPath, debugOutput)

	// Set default value for outputPath
	if runtime.GOOS == "windows" {
		if outputPath == "" {
			outputPath = ".\\" + sourceFileNameWithoutExt + ".exe"
		} else if !strings.HasSuffix(outputPath, ".exe") {
			outputPath += ".exe"
		}
	} else if runtime.GOOS == "linux" {
		if outputPath == "" {
			outputPath = "./" + sourceFileNameWithoutExt
		}
	}

	// Run g++ with output file
	internal.Link(objectPath, outputPath)
}
