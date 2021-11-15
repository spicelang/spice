package cmd

import (
	"spice/util"

	"github.com/urfave/cli/v2"
)

// CliCommands is a list of all available cli commands for the Spice cli
var CliCommands = []*cli.Command{
	{
		Name:    "build",
		Aliases: []string{"b"},
		Usage:   "Builds your Spice program and emits an executable",
		Flags:   BuildCliFlags,
		Action:  Build,
	},
	{
		Name:    "run",
		Aliases: []string{"r"},
		Usage:   "Builds your Spice program and runs it immediately",
		Flags:   RunCliFlags,
		Action:  Run,
	},
	{
		Name:    "install",
		Aliases: []string{"i"},
		Usage:   "Builds your Spice program and installs it to a directory in the PATH variable",
		Flags:   InstallCliFlags,
		Action:  Install,
		Hidden:  util.IsDockerized(),
	},
	{
		Name:    "uninstall",
		Aliases: []string{"u"},
		Usage:   "Deletes an installed executable from the binary directory",
		Action:  Uninstall,
		Hidden:  util.IsDockerized(),
	},
}
