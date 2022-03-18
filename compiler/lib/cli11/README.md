# CLI11 lib
We use the command line parser library [CLI11](https://github.com/CLIUtils/CLI11) as a standalone header file, but we modified it, so it allows us to use short options, that are longer than one char e.g. `-O2`, which the CLI11 libraray does not allow per default.

The current version that is beeing used is `v2.1.2`.

## Modification

```cpp
if(name.length() == 2 && valid_first_char(name[1]))
    short_names.emplace_back(1, name[1]);
else
    throw BadNameString::OneCharName(name);
```

is now

```cpp
short_names.push_back(name);
```