---
title: Hello World
---

## Introduction

In this example, we will code a simple "Hello World" program in Spice. What it does is to simply print "Hello World!" to the
console when it gets started. The guide assumes, that you already have Spice
[installed on your development system](../install/linux.md) and are ready to go.

## Write your first program

The first thing you need is a new file to hold the Spice code for your program. Please create a new file in an arbitrary directory
and call it `hello-world.spice`. Open up the file in a text editor of your choice and paste following code in it:

```spice
f<int> main() {
	printf("Hello World!");
}
```

## Run your first program

Save the file and open a terminal in the directory, where the `hello-world.spice` file lives. Then run the following command:
`spice run hello-world.spice`.

This is what you should see:
```shell
$ ./hello-world
Hello World!
```

Congratulations! You just wrote and executed your first Spice program!

You can follow the next pages to see, what else Spice can do for you. Feel free to skip to sections of interest to you.
