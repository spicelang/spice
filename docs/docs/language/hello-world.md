---
title: Hello World
---

In this example, we will code a simple "Hello World" program in Spice. What it does is to simply print "Hello World!" to the console when it gets started. The guide assumes, that you already have Spice installed on your development system and are ready to go.

The first thing you need is a new file to hold the Spice code for your progam. Please create a new file in an arbitrary directory and call it `hello-world.spice`. Open up the file in a text editor of your choice and paste following code in it:

```spice
f<int> main() {
	printf("Hello World!");
}
```

Save the file and open a terminal in the directory, where the `hello-world.spice` file lives. Then run the following command: `spice build hello-world.spice`. As of now, the current directory should contain an executable file called `hello-world` (Linux) or `hello-world.exe` (Windows). <br>
Now run `./hello-world` (Linux) or `.\hello-world.exe` (Windows) in your terminal instance.

This is what you should see:
```shell
$ ./hello-world
Hello World!
```

Congratulation! You just wrote and executed your first Spice program!