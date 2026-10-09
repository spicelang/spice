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

## Add options

Options take a value. It is either written into a target variable or passed to a callback. By default, an option takes
exactly one value. Calling `allowMultipleValues()` on an option makes it take all following arguments as values, up to
the next option or the positional arguments that are still missing. Each value is passed to the option separately:

```spice
// app-name.spice

import "std/io/cli-parser";

f<int> main(int argc, string[] argv) {
    CliParser cli = CliParser("app-name", "Short description of the app");

    string name = "World";
    cli.addOption("--name", name, "Name to greet");

    CliOption<string>& tagOption = cli.addOption("--tag", p(const string& tag) {
        printf("Tag: %s\n", tag);
    }, "Tags to print");
    tagOption.allowMultipleValues();

    cli.parse(argc, argv);
    printf("Hello %s!\n", name);
}
```

```console
$ ./app-name --tag a b c --name Spice
Tag: a
Tag: b
Tag: c
Hello Spice!
```

The value of an option can be attached to its name with an equals sign instead of being passed as a separate argument.
Only the first `=` splits the name from the value, so the value itself may contain further `=` characters. Both of these
invocations are equivalent:

```console
$ ./app-name --name Spice
$ ./app-name --name=Spice
```

Flags take no value, so a value attached to a flag (e.g. `--hi=false`) is rejected as an unknown argument. A flag and an
option may share a name, though. Then the bare name sets the flag, while a value attached with `=` goes to the option. This
gives an option with an optional value, which does not take the following argument as its value:

```spice
cli.addOption("--level", p(const string& level) { /* use the given level */ }, "Set the level");
cli.addFlag("--level", p(const bool& _v) { /* use the default level */ }, "Use the default level");
```
