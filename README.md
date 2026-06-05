# PODEM Project (Standalone)

This directory is a standalone copy of `podem_project` from `chinthana-w/my_playground`.

## Contents
- C source files for PODEM fault simulation (`main.c`, `input.c`, `user.c`, `input.h`)
- Benchmark circuit files (`*.isc`)
- Example output/pattern files (`*.pattern`, `*.podem`, `*.res`)
- `Makefile` for building the executable

## Build
```bash
make
```

## Run
```bash
./project c17.isc c17.podem
```

You can replace `c17.isc` and output file name with other benchmark files in this folder.

## Clean
```bash
make clean
```
