package cmd

import (
	"spice/util"

	"github.com/urfave/cli/v2"
)

// InstallCliFlags are the cli flags for the install command
var InstallCliFlags = []cli.Flag{
	&cli.BoolFlag{
		Name:    "debug-output",
		Aliases: []string{"d"},
		Usage:   "Print compiler output for debugging",
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

// Install takes the passed code file, resolves its dependencies, emits an executable and installs it to a directory in the system PATH
func Install(c *cli.Context) error {
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

	// Only execute the command if the environment is not dockerized
	if util.IsDockerized() {
		util.Error("Cannot run install command in a dockerized environment", true)
		return nil
	}

	// Get the install path
	installPath := util.SourceFileToInstallPath(sourceFile)

	// Build executable to the install path
	return buildFromSourceFile(sourceFile, "", "", "", installPath, debugOutput, optLevel)
}
