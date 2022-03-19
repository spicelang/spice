# CLI11 lib
We use the command line parser library [CLI11](https://github.com/CLIUtils/CLI11) as a standalone header file, but we modified it, so it allows us to use short options, that are longer than one char e.g. `-O2`, which the CLI11 libraray does not allow per default.

The current version that is being used is `v2.1.2`.

## Modifications
### First modification
```cpp
if(name.length() == 2 && valid_first_char(name[1]))
    short_names.emplace_back(1, name[1]);
else
    throw BadNameString::OneCharName(name);
```

is now

```cpp
name = name.substr(1);
short_names.emplace_back(name);
```

### Second modification
```cpp
if(current.size() > 1 && current[0] == '-' && valid_first_char(current[1])) {
    name = current.substr(1, 1);
    rest = current.substr(2);
    return true;
}
```

is now

```cpp
if(current.size() > 1 && current[0] == '-' && valid_first_char(current[1])) {
    name = current.substr(1);
    return true;
}
```