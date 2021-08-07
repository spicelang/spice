package cmd

import (
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"strings"
)

// Run takes the passed code file, resolves its dependencies, emits an executable and runs it
func Run(sourceFile string) {
	sourceFileName := filepath.Base(sourceFile)
	sourceFileNameWithoutExt := strings.TrimSuffix(sourceFileName, filepath.Ext(sourceFileName))

	// Get path to build to
	buildPath := "./" + sourceFileNameWithoutExt
	if runtime.GOOS == "windows" {
		buildPath = os.TempDir()
		os.MkdirAll(buildPath, 0755)
		buildPath += "\\spice-executable.exe"
	} else if runtime.GOOS == "linux" {
		buildPath = os.TempDir()
		os.MkdirAll(buildPath, 0755)
		buildPath += "/spice-executable"
	}

	// Compile program and emit executable file to tmp dir
	Build(sourceFile, "", buildPath, false)

	// Run executable
	exec.Command(buildPath).Run()
}
