package cmd

import (
	"os"
	"path/filepath"
	"runtime"
	"spice/internal"
	"spice/util"
	"strings"

	"github.com/urfave/cli/v2"
)

// BuildCliFlags are the cli flags for the build command
var BuildCliFlags = []cli.Flag{
	&cli.BoolFlag{
		Name:    "debug-output",
		Aliases: []string{"d"},
		Usage:   "Print compiler output for debugging",
	},
	&cli.StringFlag{
		Name:    "target-arch",
		Aliases: []string{"ta"},
		Usage:   "Target CPU architecture for the emitted executable (for cross-compiling)",
	},
	&cli.StringFlag{
		Name:    "target-vendor",
		Aliases: []string{"tv"},
		Usage:   "Target vendor for the emitted executable (for cross-compiling)",
	},
	&cli.StringFlag{
		Name:    "target-os",
		Aliases: []string{"to"},
		Usage:   "Target os for the emitted executable (for cross-compiling)",
	},
	&cli.BoolFlag{
		Name:    "opt-0",
		Aliases: []string{"O0"},
		Usage:   "Set optimization level to 0",
	},
	&cli.BoolFlag{
		Name:    "opt-1",
		Aliases: []string{"O1"},
		Usage:   "Set optimization level to 1",
	},
	&cli.BoolFlag{
		Name:    "opt-2",
		Aliases: []string{"O2"},
		Usage:   "Set optimization level to 2",
		Value:   true,
	},
	&cli.BoolFlag{
		Name:    "opt-3",
		Aliases: []string{"O3"},
		Usage:   "Set optimization level to 3",
	},
	&cli.BoolFlag{
		Name:    "link-static",
		Aliases: []string{"s"},
		Usage:   "Produce portable executable by linking it statically",
	},
	&cli.BoolFlag{
		Name:    "pthreads",
		Aliases: []string{"p", "t"},
		Usage:   "Compiled program makes use of the Posix Threads (pthreads) library",
	},
	&cli.PathFlag{
		Name:    "output",
		Aliases: []string{"o"},
		Usage:   "Path to the location where the output executable should go",
	},
}

// ---------------------------------------------------------------- Public functions ---------------------------------------------------------------

// Build takes the passed code file, resolves its dependencies and emits an executable, representing its functionality
func Build(c *cli.Context) error {
	// Extract flags
	sourceFile := c.Args().Get(0)
	targetArch := c.String("target-arch")
	targetVendor := c.String("target-vendor")
	targetOs := c.String("target-os")
	outputFile := c.Path("output")
	debugOutput := c.Bool("debug-output")
	linkStatic := c.Bool("link-static")
	pThreads := c.Bool("pthreads")
	optLevel := 2
	if c.Bool("opt-0") {
		optLevel = 0
	} else if c.Bool("opt-1") {
		optLevel = 1
	} else if c.Bool("opt-3") {
		optLevel = 3
	}

	return buildFromSourceFile(sourceFile, targetArch, targetVendor, targetOs, outputFile, optLevel, debugOutput, linkStatic, pThreads)
}

// ---------------------------------------------------------------- Private functions --------------------------------------------------------------

func buildFromSourceFile(
	sourceFile, targetArch, targetVendor, targetOs, outputFile string,
	optLevel int,
	debugOutput, linkStatic, pThreads bool,
) error {
	sourceFileName := filepath.Base(sourceFile)
	sourceFileNameWithoutExt := strings.TrimSuffix(sourceFileName, filepath.Ext(sourceFileName))

	// Get temp dir as location for object files
	tmpDir := os.TempDir()
	objectDir := tmpDir + "/spice-output"
	if err := os.MkdirAll(objectDir, 0750); err != nil {
		util.Error("Could not create output dir", true)
		return err
	}

	// Compile source and emit object file to temp dir
	internal.Compile(sourceFile, targetArch, targetVendor, targetOs, objectDir, debugOutput, optLevel)

	// Set default value for outputFile
	if strings.HasPrefix(targetOs, "windows") || (targetOs == "" && runtime.GOOS == "windows") {
		if outputFile == "" {
			outputFile = ".\\" + sourceFileNameWithoutExt + ".exe"
		} else if !strings.HasSuffix(outputFile, ".exe") {
			outputFile += ".exe"
		}
	} else if strings.HasPrefix(targetOs, "linux") || (targetOs == "" && runtime.GOOS == "linux") {
		if outputFile == "" {
			outputFile = "./" + sourceFileNameWithoutExt
		}
	} else {
		util.Error("Unable to build. You have to specify a target OS", true)
		return nil
	}

	// Link all object files together
	objectFiles := util.GetObjectFileTree(objectDir)
	internal.Link(objectFiles, outputFile, linkStatic, pThreads)

	// Clear object files
	for _, objectFile := range objectFiles {
		if err := os.Remove(objectFile); err != nil {
			util.Error("Problem with object file cleanup. This could cause problems with future builds", true)
			return err
		}
	}

	return nil
}
