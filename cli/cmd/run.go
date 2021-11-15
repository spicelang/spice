package cmd

import (
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"spice/util"
	"strings"
)

// Run takes the passed code file, resolves its dependencies, emits an executable and runs it
func Run(sourceFile string, debugOutput bool, optLevel int) {
	sourceFileName := filepath.Base(sourceFile)
	sourceFileNameWithoutExt := strings.TrimSuffix(sourceFileName, filepath.Ext(sourceFileName))

	// Get path to build to
	buildPath := "./" + sourceFileNameWithoutExt
	if runtime.GOOS == "windows" {
		buildPath = os.TempDir()
		if err := os.MkdirAll(buildPath, 0750); err != nil {
			util.Error("Could not find output dir", true)
		}
		buildPath += "\\spice-executable.exe"
	} else if runtime.GOOS == "linux" {
		buildPath = os.TempDir()
		if err := os.MkdirAll(buildPath, 0750); err != nil {
			util.Error("Could not find output dir", true)
		}
		buildPath += "/spice-executable"
	}

	// Compile program and emit executable file to tmp dir
	Build(sourceFile, "", "", "", buildPath, debugOutput, optLevel)

	// Run executable
	cmd := exec.Command(buildPath)
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	err := cmd.Start()
	if err != nil {
		util.Error("Could not run executable", true)
	}
	err = cmd.Wait()
	if err != nil {
		util.Error("Could not run executable", true)
	}
}
