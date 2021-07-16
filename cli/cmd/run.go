package cmd

import (
	"os"
	"spice/internal"
)

func Run(sourceFile string) {
	// Compile program and emit executable file to tmp dir
	tmpPath := os.TempDir()
	println(tmpPath)
	internal.Compile(sourceFile, "", tmpPath+"/spice-output.o")

	// Run executable

}
