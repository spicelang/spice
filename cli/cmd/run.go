package cmd

import (
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"spice/util"
	"strings"

	"github.com/urfave/cli/v2"
)

// RunCliFlags are the cli flags for the run command
var RunCliFlags = []cli.Flag{
	&cli.BoolFlag{
		Name:    "debug-output",
		Aliases: []string{"d"},
		Usage:   "Print compiler output for debugging",
		Value:   false,
	},
	&cli.BoolFlag{
		Name:    "opt-0",
		Aliases: []string{"O0"},
		Usage:   "Set optimization level to 0",
	},
	&cli.BoolFlag{
		Name:    "opt-1",
		Aliases: []string{"O1"},
		Usage:   "Set optimization level to 1",
	},
	&cli.BoolFlag{
		Name:    "opt-2",
		Aliases: []string{"O2"},
		Usage:   "Set optimization level to 2",
		Value:   true,
	},
	&cli.BoolFlag{
		Name:    "opt-3",
		Aliases: []string{"O3"},
		Usage:   "Set optimization level to 3",
	},
	&cli.PathFlag{
		Name:    "output",
		Aliases: []string{"o"},
		Usage:   "Path to the location where the output executable should go",
	},
}

// ---------------------------------------------------------------- Public functions ---------------------------------------------------------------

// Run takes the passed code file, resolves its dependencies, emits an executable and runs it
func Run(c *cli.Context) error {
	// Extract flags
	sourceFile := c.Args().Get(0)
	debugOutput := c.Bool("debug-output")
	optLevel := 2
	if c.Bool("opt-0") {
		optLevel = 0
	} else if c.Bool("opt-1") {
		optLevel = 1
	} else if c.Bool("opt-3") {
		optLevel = 3
	}

	sourceFileName := filepath.Base(sourceFile)
	sourceFileNameWithoutExt := strings.TrimSuffix(sourceFileName, filepath.Ext(sourceFileName))

	// Get path to build to
	buildPath := "./" + sourceFileNameWithoutExt
	if runtime.GOOS == "windows" {
		buildPath = os.TempDir()
		if err := os.MkdirAll(buildPath, 0750); err != nil {
			util.Error("Could not find output dir", true)
			return err
		}
		buildPath += "\\spice-executable.exe"
	} else if runtime.GOOS == "linux" {
		buildPath = os.TempDir()
		if err := os.MkdirAll(buildPath, 0750); err != nil {
			util.Error("Could not find output dir", true)
			return err
		}
		buildPath += "/spice-executable"
	}

	// Compile program and emit executable file to tmp dir
	err := buildFromSourceFile(sourceFile, "", "", "", buildPath, debugOutput, optLevel)
	if err != nil {
		util.Error("Building source file failed", true)
		return err
	}

	// Run executable
	cmd := exec.Command(buildPath)
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	err = cmd.Start()
	if err != nil {
		util.Error("Could not run executable", true)
	}
	err = cmd.Wait()
	if err != nil {
		util.Error("Could not run executable", true)
	}
	return nil
}
