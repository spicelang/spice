package cmd

import (
	"os"
	"spice/util"

	"github.com/urfave/cli/v2"
)

// ---------------------------------------------------------------- Public functions ---------------------------------------------------------------

// Uninstall deletes the executable, which was installed with the install command
func Uninstall(c *cli.Context) error {
	// Extract flags
	sourceFile := c.Args().Get(0)

	// Only execute the command if the environment is not dockerized
	if util.IsDockerized() {
		util.Error("Cannot run uninstall command in a dockerized environment", true)
		return nil
	}

	// Get the install path
	installPath := util.SourceFileToInstallPath(sourceFile)

	// Delete the executable
	if err := os.Remove(installPath); err != nil {
		util.Error("Could not delete the installed executable", true)
		return err
	}
	return nil
}
