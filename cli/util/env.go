package util

import (
	"os"
	"path/filepath"
	"runtime"
	"strings"
)

// IsDockerized checks if Spice runs in a dockerized environment
func IsDockerized() bool {
	return os.Getenv("SPICE_DOCKERIZED") == "1"
}

func SourceFileToInstallPath(sourceFile string) (installPath string) {
	sourceFileName := filepath.Base(sourceFile)
	sourceFileNameWithoutExt := strings.TrimSuffix(sourceFileName, filepath.Ext(sourceFileName))

	// Get path to install to
	installDir := "./"
	installPath = installDir + sourceFileNameWithoutExt
	if runtime.GOOS == "windows" {
		installDir = os.Getenv("USERPROFILE") + "\\spice\\bin\\"
		if err := os.MkdirAll(installDir, 0750); err != nil {
			Error("Could not create binary dir", true)
		}
		installPath = installDir + "\\" + sourceFileNameWithoutExt + ".exe"
	} else if runtime.GOOS == "linux" {
		installDir = "/usr/local/bin"
		if err := os.MkdirAll(installDir, 0750); err != nil {
			Error("Could not create binary dir", true)
		}
		installPath = installDir + "/" + sourceFileNameWithoutExt
	}
	return
}
