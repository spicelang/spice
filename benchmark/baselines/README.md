# Performance baselines of the compiler re-implementation

`legacy-<commit>.json` is the output of `benchmark/run-benchmarks.py run` for the legacy compiler (`src/`) at the given
commit. It is the reference for the performance budgets of `media/specs/compiler-reimplementation.md`, section 6.5. The
`spice-next` numbers are compared against it, once `build/spice-next` can compile programs:

```sh
python benchmark/run-benchmarks.py run --compiler next=build/spice-next --suite full -o build/benchmark-next.json
python - <<'PY'
# merge the baseline and the next results into one file, then compare
import json
# Replace the path by build/benchmark-local-legacy.json for a baseline recorded on this machine
base = json.load(open("benchmark/baselines/legacy-41ee4a7.json"))
nxt = json.load(open("build/benchmark-next.json"))
for key in ("metadata", "results", "samples"):
    base[key].update(nxt[key])
json.dump(base, open("build/benchmark-merged.json", "w"))
PY
python benchmark/run-benchmarks.py compare build/benchmark-merged.json --baseline legacy --candidate next
```

## How it was recorded

```sh
python build.py                                   # release build of the legacy compiler (-O3, LTO)
python benchmark/run-benchmarks.py run --compiler legacy=build/spice --suite full -o build/benchmark-local-legacy.json
```

- Commit: `41ee4a7` (`main` after WP 0.5), stage0 0.30.2.
- Machine: 4 vCPUs of an Intel Xeon @ 2.10 GHz, Linux 6.18, ASLR off (`setarch`), LLVM 23 from the Ubuntu packages.
- The nightly `Benchmark` workflow uses the pinned LLVM 23.1.6 source build. **Instruction counts, sizes and peak memory
  usage differ slightly between the two LLVM builds. Wall times and stage times differ by machine.** Compare candidates
  only against a baseline of the same machine and LLVM build (re-record it with the command above), or against the
  nightly Bencher history.
  To re-record the baseline on another machine, write it to its own file (`-o build/benchmark-local-legacy.json`) and
  merge that file instead of the checked-in one in the recipe above. The checked-in file stays the historical reference. The numbers below are the reference for orders of magnitude and for the ratios.
- The self-compile benchmarks build the compiler sources with the release settings of `build.py`. The `-O0` self-compile
  of the budget table in section 6.5 is not part of the suite yet.

## Summary

| Benchmark                             | Instructions    | Peak RSS (MiB) | Wall time (ms) |
|---------------------------------------|----------------:|---------------:|---------------:|
| `compile/hello-world/O0-llvm`         |      10,710,311 |           39.4 |           49.4 |
| `compile/hello-world/O2-llvm`         |      14,173,826 |           43.4 |           51.7 |
| `compile/std-generics/O0-llvm`        |   1,444,833,875 |           70.7 |          343.4 |
| `compile/std-generics/O2-llvm`        |   6,264,035,884 |           81.5 |        1,641.3 |
| `compile/many-instantiations/O0-llvm` |   2,950,613,989 |           95.0 |          562.4 |
| `compile/many-instantiations/O2-llvm` |  10,304,546,901 |          102.7 |        2,623.0 |
| `compile/deep-imports/O0-llvm`        |     603,137,492 |           52.9 |          235.1 |
| `compile/deep-imports/O2-llvm`        |     993,636,778 |           56.3 |          386.2 |
| `compile/large-function/O0-llvm`      |     908,419,722 |           89.9 |          260.3 |
| `compile/large-function/O2-llvm`      |   8,625,968,341 |           83.4 |        1,223.8 |
| `runtime/binary-trees/O2`             |     175,091,943 |           17.8 |          720.9 |
| `runtime/n-body/O2`                   |      29,613,160 |            1.9 |          591.4 |
| `runtime/fannkuch/O2`                 |       9,157,239 |            1.9 |          218.6 |
| `runtime/sieve/O2`                    |      42,658,808 |           49.4 |          601.6 |
| `runtime/hash-map/O2`                 |      52,744,182 |          145.5 |          810.3 |
| `runtime/string-builder/O2`           |      78,947,643 |           50.3 |          305.7 |
| `runtime/regex/O2`                    |     377,670,521 |            4.8 |          677.2 |
| `self-compile/release`                | 165,676,397,780 |          628.7 |       44,194.6 |
| `self-compile-stage1/release`         | 165,760,609,840 |          629.0 |       46,171.8 |
