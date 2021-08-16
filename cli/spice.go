package main

import (
	"log"
	"os"
	"spice/cmd"
	"spice/util"

	"github.com/urfave/cli/v2"
)

// nolint: gochecknoglobals
var (
	version = "dev"
	commit  = ""
	date    = ""
	builtBy = ""
)

func main() {
	// Version flag
	cli.VersionFlag = &cli.BoolFlag{
		Name:    "version",
		Aliases: []string{"v"},
		Usage:   "Prints the Spice version, you're running",
	}

	// Commands
	buildCommand := cli.Command{
		Name:    "build",
		Aliases: []string{"b"},
		Usage:   "Builds your Spice program and emits an executable",
		Flags: []cli.Flag{
			&cli.BoolFlag{Name: "debug-output", Aliases: []string{"d"}, Usage: "Print compiler output for debugging", Value: false},
			&cli.StringFlag{Name: "target", Aliases: []string{"t"}, Usage: "Target triple for the emitted executable (for cross-compiling)"},
			&cli.IntFlag{Name: "opt-level", Aliases: []string{"o"}, Usage: "Set optimization level", Value: 2},
			&cli.PathFlag{Name: "output", Usage: "Path to the location where the output executable should go"},
		},
		Action: func(c *cli.Context) error {
			cmd.Build(c.Args().Get(0), c.String("target"), c.Path("output"), c.Bool("debug-output"), c.Int("opt-level"))
			return nil
		},
	}
	installCommand := cli.Command{
		Name:    "install",
		Aliases: []string{"i"},
		Usage:   "Builds your Spice program and installs it to a directory in the PATH variable",
		Flags: []cli.Flag{
			&cli.BoolFlag{Name: "debug-output", Aliases: []string{"d"}, Usage: "Print compiler output for debugging", Value: false},
			&cli.IntFlag{Name: "opt-level", Aliases: []string{"o"}, Usage: "Set optimization level", Value: 2},
		},
		Action: func(c *cli.Context) error {
			cmd.Install(c.Args().Get(0), c.Bool("debug-output"), c.Int("opt-level"))
			return nil
		},
	}
	runCommand := cli.Command{
		Name:    "run",
		Aliases: []string{"r"},
		Usage:   "Builds your Spice program and runs it immediately",
		Flags: []cli.Flag{
			&cli.BoolFlag{Name: "debug-output", Aliases: []string{"d"}, Usage: "Print compiler output for debugging", Value: false},
			&cli.IntFlag{Name: "opt-level", Aliases: []string{"o"}, Usage: "Set optimization level", Value: 2},
		},
		Action: func(c *cli.Context) error {
			cmd.Run(c.Args().Get(0), c.Bool("debug-output"), c.Int("opt-level"))
			return nil
		},
	}
	uninstallCommand := cli.Command{
		Name:    "uninstall",
		Aliases: []string{"u"},
		Usage:   "Deletes an installed executable from the binary directory",
		Action: func(c *cli.Context) error {
			cmd.Uninstall(c.Args().Get(0))
			return nil
		},
	}

	commands := []*cli.Command{&buildCommand, &runCommand, &uninstallCommand}
	if !util.IsDockerized() {
		commands = append(commands, &installCommand)
	}

	// Main cli configuration
	app := &cli.App{
		Name:    "spice",
		Version: util.BuildVersion(version, commit, date, builtBy),
		Authors: []*cli.Author{
			{Name: "Marc Auberer", Email: "marc.auberer@chillibits.com"},
		},
		Copyright:              "© 2021 Marc Auberer",
		Usage:                  "Official CLI for the Spice programming language.",
		Commands:               commands,
		UseShortOptionHandling: true,
	}

	err := app.Run(os.Args)
	if err != nil {
		log.Fatal(err)
	}
}
