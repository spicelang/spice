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

	// Main cli configuration
	app := &cli.App{
		Name:    "spice",
		Version: util.BuildVersion(version, commit, date, builtBy),
		Authors: []*cli.Author{
			{Name: "Marc Auberer", Email: "marc.auberer@chillibits.com"},
		},
		Copyright: "© 2021 Marc Auberer",
		Usage:     "Official CLI for the Spice programming language.",
		Commands: []*cli.Command{
			{
				Name:    "build",
				Aliases: []string{"b"},
				Usage:   "Builds your Spice program and emits an executable",
				Flags: []cli.Flag{
					&cli.StringFlag{Name: "target", Aliases: []string{"t"}, Usage: "Target triple for the emitted executable (for cross-compiling)"},
				},
				Action: func(c *cli.Context) error {
					cmd.Build(c.Args().Get(0), c.String("target"))
					return nil
				},
			},
			{
				Name:    "install",
				Aliases: []string{"i"},
				Usage:   "Builds your Spice program, installs it and adds it to the path env variable",
				Flags:   []cli.Flag{},
				Action: func(c *cli.Context) error {
					cmd.Install(c.Args().Get(0))
					return nil
				},
			},
		},
		UseShortOptionHandling: true,
	}

	err := app.Run(os.Args)
	if err != nil {
		log.Fatal(err)
	}
}
