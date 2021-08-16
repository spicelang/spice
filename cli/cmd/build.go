package cmd

import (
	"os"
	"path/filepath"
	"runtime"
	"spice/internal"
	"spice/util"
	"strings"
)

// Build takes the passed code file, resolves its dependencies and emits an executable, representing its functionality
func Build(sourceFile string, targetTriple string, outputFile string, debugOutput bool, optLevel int) {
	sourceFileName := filepath.Base(sourceFile)
	sourceFileNameWithoutExt := strings.TrimSuffix(sourceFileName, filepath.Ext(sourceFileName))

	// Get temp dir as location for object files
	tmpDir := os.TempDir()
	objectDir := tmpDir + "/spice-output"
	if err := os.MkdirAll(objectDir, 0750); err != nil {
		util.Error("Could not create output dir", true)
	}

	// Compile program ane emit object file to temp dir
	internal.Compile(sourceFile, targetTriple, objectDir, debugOutput, optLevel)

	// Set default value for outputFile
	if runtime.GOOS == "windows" {
		if outputFile == "" {
			outputFile = ".\\" + sourceFileNameWithoutExt + ".exe"
		} else if !strings.HasSuffix(outputFile, ".exe") {
			outputFile += ".exe"
		}
	} else if runtime.GOOS == "linux" {
		if outputFile == "" {
			outputFile = "./" + sourceFileNameWithoutExt
		}
	}

	// Run g++ with all object files
	objectFiles := util.GetObjectFileTree(objectDir)
	internal.Link(objectFiles, outputFile)
}
