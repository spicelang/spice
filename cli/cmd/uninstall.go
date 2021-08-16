package cmd

import (
	"os"
	"spice/util"
)

// Uninstall deletes the executable, which was installed with the install command
func Uninstall(sourceFile string) {
	installPath := util.SourceFileToInstallPath(sourceFile)

	if err := os.Remove(installPath); err != nil {
		util.Error("Could not delete the installed executable", true)
	}
}
