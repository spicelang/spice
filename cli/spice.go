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
			&cli.StringFlag{Name: "target", Aliases: []string{"t"}, Usage: "Target triple for the emitted executable (for cross-compiling)"},
			&cli.PathFlag{Name: "output", Aliases: []string{"o"}, Usage: "Path to the location where the output executable should go"},
			&cli.PathFlag{Name: "debug-output", Aliases: []string{"d"}, Usage: "Print compiler output for debugging"},
		},
		Action: func(c *cli.Context) error {
			cmd.Build(c.Args().Get(0), c.String("target"), c.Path("output"), c.Bool("debug-output"))
			return nil
		},
	}
	installCommand := cli.Command{
		Name:    "install",
		Aliases: []string{"i"},
		Usage:   "Builds your Spice program, installs it and adds it to the path env variable",
		Flags: []cli.Flag{
			&cli.PathFlag{Name: "debug-output", Aliases: []string{"d"}, Usage: "Print compiler output for debugging"},
		},
		Action: func(c *cli.Context) error {
			cmd.Install(c.Args().Get(0), c.Bool("debug-output"))
			return nil
		},
	}
	runCommand := cli.Command{
		Name:    "run",
		Aliases: []string{"r"},
		Usage:   "Builds your Spice program and runs it",
		Flags: []cli.Flag{
			&cli.PathFlag{Name: "debug-output", Aliases: []string{"d"}, Usage: "Print compiler output for debugging"},
		},
		Action: func(c *cli.Context) error {
			cmd.Run(c.Args().Get(0))
			return nil
		},
	}

	commands := []*cli.Command{&buildCommand, &runCommand}
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
