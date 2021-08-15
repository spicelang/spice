package cmd

import (
	"os"
	"path/filepath"
	"runtime"
	"spice/util"
	"strings"
)

// Install takes the passed code file, resolves its dependencies, emits an executable and installs it to a directory in the system PATH
func Install(sourceFile string, debugOutput bool, optLevel int) {
	sourceFileName := filepath.Base(sourceFile)
	sourceFileNameWithoutExt := strings.TrimSuffix(sourceFileName, filepath.Ext(sourceFileName))

	// Get path to install to
	installDir := "./"
	installPath := installDir + sourceFileNameWithoutExt
	if runtime.GOOS == "windows" {
		installDir = os.Getenv("USERPROFILE") + "\\spice\\bin\\"
		if err := os.MkdirAll(installDir, 0750); err != nil {
			util.Error("Could not create binary dir", true)
		}
		installPath = installDir + "\\" + sourceFileNameWithoutExt + ".exe"
	} else if runtime.GOOS == "linux" {
		installDir = "/usr/local/bin"
		if err := os.MkdirAll(installDir, 0750); err != nil {
			util.Error("Could not create binary dir", true)
		}
		installPath = installDir + "/" + sourceFileNameWithoutExt
	}

	// Build executable to the install path
	Build(sourceFile, "", installPath, debugOutput, optLevel)
}
