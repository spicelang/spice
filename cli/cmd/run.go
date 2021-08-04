package cmd

import (
	"os"
	"spice/internal"
)

// Run takes the passed code file, resolves its dependencies, emits an executable and runs it
func Run(sourceFile string) {
	// Compile program and emit executable file to tmp dir
	tmpPath := os.TempDir()
	println(tmpPath)
	internal.Compile(sourceFile, "", tmpPath+"/spice-output.o", false)

	// Run executable

}
