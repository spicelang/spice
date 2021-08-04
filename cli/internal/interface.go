package internal

import (
	"os/exec"
	"spice/util"
	"strconv"
	"strings"
	"syscall"

	"github.com/cli/safeexec"
	"github.com/kardianos/osext"
)

const COMPILER_EXECUTABLE_NAME = "spicec"

// Compile executes the compiler executable with the provided input arguments
func Compile(sourceFile string, targetTriple string, outputPath string, debugOutput bool) {
	// Search for executable
	executablePath, _ := osext.Executable()
	executablePath = strings.ReplaceAll(executablePath, "\\", "/")
	executablePath = executablePath[:strings.LastIndex(executablePath, "/")] + "/"
	if util.FileExists("/usr/lib/spice") {
		executablePath = "/usr/lib/spice/"
	}

	// Check if compiler executable exists
	if !CommandExists(executablePath + COMPILER_EXECUTABLE_NAME) {
		util.Error("Compiler executable not found. Please check your installation / re-install Spice", true)
	}

	// Execute compiler executable. e.g.: spicec "./sourceFile.spice" "x86_64-w64-windows-gnu"
	cmd := exec.Command(executablePath+COMPILER_EXECUTABLE_NAME, sourceFile, targetTriple, outputPath, strconv.FormatBool(debugOutput))
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
