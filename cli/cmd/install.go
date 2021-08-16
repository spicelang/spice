package cmd

import (
	"spice/util"
)

// Install takes the passed code file, resolves its dependencies, emits an executable and installs it to a directory in the system PATH
func Install(sourceFile string, debugOutput bool, optLevel int) {
	installPath := util.SourceFileToInstallPath(sourceFile)

	// Build executable to the install path
	Build(sourceFile, "", installPath, debugOutput, optLevel)
}
