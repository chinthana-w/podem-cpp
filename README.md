# PODEM Project (Standalone)

This directory is a standalone copy of `podem_project` from `chinthana-w/my_playground`.

## Contents
- C source files for PODEM fault simulation (`src/main.c`, `src/input.c`, `src/user.c`, `src/input.h`)
- Benchmark circuit files (`data/isc/*.isc`)
- Example output/pattern files (`data/out/*.pattern`, `data/out/*.podem`, `data/out/*.res`)
- `Makefile` for building the executable

## Build
```bash
make
```

## Run
```bash
./project c17.isc c17.podem
```

You can replace `c17.isc` and output file name with other benchmark files.
If relative names are provided, the program reads inputs from `data/isc/` and writes outputs to `data/out/`.

## Clean
```bash
make clean
```
