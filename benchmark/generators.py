"""Generators for the synthetic compile-time benchmarks. Each generator writes the sources of a program into a directory and
returns the path of its main source file. The output only depends on the scale, so that the benchmarks are reproducible."""
from pathlib import Path


def many_instantiations(out_dir: Path, scale: int) -> Path:
    """'scale' struct types, each used with a generic struct, a generic function and a Vector"""
    lines = ['import "std/data/vector";', "", "type T dyn;", "", "type Box<T> struct {", "    T value", "    unsigned long accessCount", "}", "",
             "p Box.ctor<T>(const T& value) {", "    this.value = value;", "    this.accessCount = 0l;", "}", "",
             "f<T&> Box.get<T>() {", "    this.accessCount++;", "    return this.value;", "}", "",
             "f<Box<T>> wrap<T>(const T& value) {", "    return Box<T>(value);", "}", ""]
    for i in range(scale):
        lines += [f"type S{i} struct {{", "    int value", "    long weight", "}", ""]
    lines += ["f<int> main() {", "    long total = 0l;"]
    for i in range(scale):
        lines += [f"    Vector<S{i}> v{i};",
                  f"    v{i}.pushBack(S{i}{{ {i}, {i}l }});",
                  f"    Box<S{i}> b{i} = wrap(v{i}.get(0ul));",
                  f"    S{i}& s{i} = b{i}.get();",
                  f"    total += s{i}.weight + s{i}.value;"]
    lines += ['    printf("%ld\\n", total);', "}", ""]
    main_file = out_dir / "source.spice"
    main_file.write_text("\n".join(lines))
    return main_file


def deep_imports(out_dir: Path, scale: int) -> Path:
    """A chain of 'scale' source files, each importing the next one"""
    for i in range(scale):
        lines = []
        if i + 1 < scale:
            lines += [f'import "module{i + 1}" as next;', ""]
        lines += [f"public type Item{i} struct {{", "    public int id", "    public double score", "}", "",
                  f"public f<Item{i}> createItem{i}(int id) {{", f"    return Item{i}{{ id, id * {i + 1}.5 }};", "}", "",
                  f"public f<int> depth{i}(int value) {{"]
        if i + 1 < scale:
            lines += [f"    const Item{i} item = createItem{i}(value);", f"    return next::depth{i + 1}(item.id + 1);"]
        else:
            lines += ["    return value;"]
        lines += ["}", ""]
        (out_dir / f"module{i}.spice").write_text("\n".join(lines))
    main_lines = ['import "module0" as first;', "", "f<int> main() {", '    printf("%d\\n", first::depth0(0));', "}", ""]
    main_file = out_dir / "source.spice"
    main_file.write_text("\n".join(main_lines))
    return main_file


def large_function(out_dir: Path, scale: int) -> Path:
    """One function with 'scale' blocks of statements"""
    lines = ["f<long> compute(long seed) {", "    long a = seed;", "    long b = seed * 3l;", "    long c = 0l;"]
    for i in range(scale):
        lines += [f"    a = a * {i % 7 + 2}l + b;",
                  f"    if a % {i % 5 + 2}l == 0l {{",
                  f"        b += a ^ {i}l;",
                  "    } else {",
                  f"        c -= b >> {i % 3 + 1}l;",
                  "    }",
                  f"    for int i{i} = 0; i{i} < {i % 4 + 1}; i{i}++ {{ c += i{i}; }}"]
    # The seed depends on the arguments, so that the optimizer cannot fold the whole function
    lines += ["    return a + b + c;", "}", "", "f<int> main(int argc, string[] argv) {",
              '    printf("%ld\\n", compute(cast<long>(argc) + 6l));', "}", ""]
    main_file = out_dir / "source.spice"
    main_file.write_text("\n".join(lines))
    return main_file
