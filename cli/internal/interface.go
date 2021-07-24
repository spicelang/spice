package internal

import (
	"os/exec"
	"spice/util"
	"strconv"
	"syscall"

	"github.com/cli/safeexec"
)

const COMPILER_EXECUTABLE_NAME = "spicec"

// Compile executes the compiler executable with the provided input arguments
func Compile(sourceFile string, targetTriple string, outputPath string) {
	// Check if compiler executable exists
	if !CommandExists(COMPILER_EXECUTABLE_NAME) {
		util.Error("Compiler executable not found. Please check your installation / re-install Spice", true)
	}

	// Execute compiler executable. e.g.: spicec "./sourceFile.spice" "x86_64-w64-windows-gnu"
	cmd := exec.Command(COMPILER_EXECUTABLE_NAME, sourceFile, targetTriple, outputPath)
	output, err := cmd.CombinedOutput()
	if err != nil {
		if exiterr, ok := err.(*exec.ExitError); ok {
			if status, ok := exiterr.Sys().(syscall.WaitStatus); ok {
				util.Pel()
				util.Pel()
				util.Error("Compiler exited with status code "+strconv.Itoa(status.ExitStatus())+"\nFailed to compile: "+string(output), true)
			}
		} else {
			util.Error("Failed to call compiler executable", true)
		}
	}
}

// CommandExists checks if the stated command exists on the host system
func CommandExists(cmd string) bool {
	_, err := safeexec.LookPath(cmd)
	return err == nil
}
