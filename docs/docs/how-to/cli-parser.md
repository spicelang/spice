---
title: Build a CLI Interface
---

Spice comes with a batteries-included CLI parser to make it easy to build command line interfaces. The following example shows
how to build a simple CLI interface with a few sub-commands and options.

## Minimal usage
Here you can see what the minimal configuration for the CLI parser looks like:

```spice
// app-name.spice

import "std/io/cli-parser";

f<int> main(int argc, string[] argv) {
    CliParser cli = CliParser("app-name", "Short description of the app");
    return cli.parse(argc, argv);
}
```

These two lines of code provide you with a fully functional CLI parser. The `CliParser` constructor takes two arguments:
- `app-name`: string - The name of the application. This will be used in the help text.
- `app-description`: string - A short description of the application. This will also be used in the help text.


The CLI parser automatically provides two flags, one for printing the help text and one for printing the version info.
So if we compile and run this code, we will get the following output:

```console
$ ./app-name
Short description of the app

Usage: ./app-name [options]

Flags:
--help,-h                       Print this help message
--version,-v                    Print version info
```

The default version is `v0.0.1`.

## Add flags
Now, let's add more information to the help text and add a flag:

```spice
// app-name.spice

import "std/io/cli-parser";

f<int> main(int argc, string[] argv) {
    CliParser cli = CliParser("app-name", "Short description of the app");
    cli.setVersion("v1.0.0");
    cli.setFooter("(c) 2024 by John Doe");
    
    bool flagValue = false;
    cli.addFlag("--hi", flagValue, "Say Hi");
    
    cli.parse(argc, argv);
    
    if flagValue {
        printf("Hi!\n");
    }
}
```

Now we get this help text:

```console
$ ./app-name
Short description of the app

Usage: ./app-name [options]

Flags:
--help,-h                       Print this help message
--version,-v                    Print version info
--hi                            Say Hi

(c) 2024 by John Doe
```

When we run the program with the `--hi` flag, we get the following output:

```console
$ ./app-name --hi
Hi!
```

## Add subcommands

To build more advanced CLI interfaces, you can add subcommands. Let's add a `greet` subcommand to our CLI interface:

```spice
// app-name.spice

import "std/io/cli-parser";

f<int> main(int argc, string[] argv) {
    CliParser cli = CliParser("app-name", "Short description of the app");
    cli.setVersion("v1.0.0");
    cli.setFooter("(c) 2024 by John Doe");
    
    CliSubcommand& greet = cli.addSubcommand("greet", "Greet someone");
    CliSubcommand& walk = cli.addSubcommand("walk", "Walk somewhere");
    
    cli.parse(argc, argv);
}
```

Each subcommand comes with its own `--help` flag out of the box, that prints the help text for that specific subcommand.
Subcommands can also have their own sub-commands. In other words, they can be nested:

```spice
// app-name.spice

import "std/io/cli-parser";

f<int> main(int argc, string[] argv) {
    CliParser cli = CliParser("app-name", "Short description of the app");
    cli.setVersion("v1.0.0");
    cli.setFooter("(c) 2024 by John Doe");
    
    CliSubcommand& greet = cli.addSubcommand("greet", "Greet someone");
    CliSubcommand& greetFriendly = greet.addSubcommand("friendly", "Greet someone in a friendly way");
    CliSubcommand& greetFormal = greet.addSubcommand("formal", "Greet someone in a formal way");
    CliSubcommand& walk = cli.addSubcommand("walk", "Walk somewhere");
    
    cli.parse(argc, argv);
}
```