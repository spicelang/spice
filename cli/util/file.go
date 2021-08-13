package util

import (
	"os"
	"path/filepath"
)

// FileExists checks if a file or directory exists
func FileExists(path string) bool {
	_, err := os.Stat(path)
	if err != nil {
		return false
	}
	return !os.IsNotExist(err)
}

func GetObjectFileTree(objectDir string) (objectFiles []string) {
	filepath.Walk(".", func(path string, info os.FileInfo, err error) error {
		if err != nil {
			return err
		}
		fi, err := os.Stat(path)
		if err != nil {
			return err
		}
		if !fi.IsDir() && filepath.Ext(path) == ".o" {
			objectFiles = append(objectFiles, path)
		}
		return nil
	})
	return
}
