package internal

import (
	"fmt"
	"os/exec"
	"spice/util"
	"strconv"
	"strings"
	"syscall"

	"github.com/cli/safeexec"
	"github.com/kardianos/osext"
)

// COMPILER_EXECUTABLE_NAME represents the name of the compiler executable
const COMPILER_EXECUTABLE_NAME = "spicec"

// Compile executes the compiler executable with the provided input arguments
func Compile(sourceFile, targetArch, targetVendor, targetOs, objectDir string, debugOutput bool, optLevel int) {
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

	// Execute compiler executable for main source file. e.g.: spicec "./sourceFile.spice" "x86_64" "w64" "windows" "." "true", "3"
	cmd := exec.Command(
		executablePath+COMPILER_EXECUTABLE_NAME,
		sourceFile,
		targetArch,
		targetVendor,
		targetOs,
		objectDir,
		strconv.FormatBool(debugOutput),
		strconv.Itoa(optLevel),
	)
	output, err := cmd.CombinedOutput()
	if err != nil {
		if exiterr, ok := err.(*exec.ExitError); ok {
			if status, ok := exiterr.Sys().(syscall.WaitStatus); ok {
				util.Pel()
				util.Pel()
				util.Error("Compiler exited with status code "+strconv.Itoa(status.ExitStatus())+
					"\nFailed to compile: "+string(output), true)
			}
		} else {
			util.Error("Failed to call compiler executable", true)
		}
	} else if debugOutput {
		fmt.Println(string(output))
	}
}

// Link bundles the object files which were created by the compiler to an output executable
func Link(sourceFiles []string, outputFile string) {
	// Search for gcc
	gccPath := "gcc"
	if !CommandExists(gccPath) {
		util.Error("gcc executable not found. Please make sure you have the package 'build-essential', "+
			"containing the g++ executable installed", true)
	}

	// Build args array for g++
	args := []string{"-no-pie", "-o", outputFile}
	args = append(args, sourceFiles...)

	// Execute g++
	cmd := exec.Command(gccPath, args...)
	output, err := cmd.CombinedOutput()
	if err != nil {
		if exiterr, ok := err.(*exec.ExitError); ok {
			if status, ok := exiterr.Sys().(syscall.WaitStatus); ok {
				util.Pel()
				util.Pel()
				util.Error("Linker exited with status code "+strconv.Itoa(status.ExitStatus())+
					"\nFailed to link: "+string(output), true)
			}
		} else {
			util.Error("Failed to call linker executable", true)
		}
	}
}

// CommandExists checks if the stated command exists on the host system
func CommandExists(cmd string) bool {
	_, err := safeexec.LookPath(cmd)
	return err == nil
}
