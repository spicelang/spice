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
			{
				Name:  "Marc Auberer",
				Email: "marc.auberer@chillibits.com",
			},
		},
		UseShortOptionHandling: true,
		EnableBashCompletion:   true,
		Copyright:              "© 2021 Spice Contributors",
		Usage:                  "Official CLI for the Spice programming language.",
		Commands:               cmd.CliCommands,
	}

	err := app.Run(os.Args)
	if err != nil {
		log.Fatal(err)
	}
}
