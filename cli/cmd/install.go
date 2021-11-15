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
		Value:   false,
	},
	&cli.IntFlag{
		Name:    "opt-level",
		Aliases: []string{"o"},
		Usage:   "Set optimization level",
		Value:   2,
	},
}

// ---------------------------------------------------------------- Public functions ---------------------------------------------------------------

// Install takes the passed code file, resolves its dependencies, emits an executable and installs it to a directory in the system PATH
func Install(c *cli.Context) error {
	// Extract flags
	sourceFile := c.Args().Get(0)
	debugOutput := c.Bool("debug-output")
	optLevel := c.Int("opt-level")

	// Only execute the command if the environment is not dockerized
	if util.IsDockerized() {
		util.Error("Cannot run install command in a dockerized environment", true)
		return nil
	}

	// Get the install path
	installPath := util.SourceFileToInstallPath(sourceFile)

	// Build executable to the install path
	buildFromSourceFile(sourceFile, "", "", "", installPath, debugOutput, optLevel)

	return nil
}
