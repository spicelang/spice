package util

import (
	"os"
	"path/filepath"
	"runtime"
	"strings"
)

// FileExists checks if a file or directory exists
func FileExists(path string) bool {
	_, err := os.Stat(path)
	if err != nil {
		return false
	}
	return !os.IsNotExist(err)
}

// SourceFileToInstallPath converts a source file name to a path where to install to
func SourceFileToInstallPath(sourceFile string) (installPath string) {
	sourceFileName := filepath.Base(sourceFile)
	sourceFileNameWithoutExt := strings.TrimSuffix(sourceFileName, filepath.Ext(sourceFileName))

	// Get path to install to
	installDir := "./"
	installPath = installDir + sourceFileNameWithoutExt
	if runtime.GOOS == "windows" {
		installDir = os.Getenv("USERPROFILE") + "\\spice\\bin"
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

// GetObjectFileTree searches for object files in a certain directory
func GetObjectFileTree(objectDir string) (objectFiles []string) {
	err := filepath.Walk(objectDir, func(path string, info os.FileInfo, err error) error {
		if err != nil {
			return err
		}
		fi, err := os.Stat(path)
		if err != nil {
			return err
		}
		if !fi.IsDir() && filepath.Ext(path) == ".o" {
			path = strings.ReplaceAll(path, "\\", "/")
			objectFiles = append(objectFiles, path)
		}
		return nil
	})
	if err != nil {
		Error("Unable to search in object directory", true)
	}
	return
}
