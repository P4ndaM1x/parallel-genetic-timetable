# parallel-genetic-timetable

This repository contains a C++ program that solves the School Timetabling Problem using a genetic algorithm parallelized with MPI (Message Passing Interface).

## Quickstart

> [!NOTE]
> All the following commands should be executed from the root of the project.

Since in-source building is forbidden, creating build directory for CMake is mandatory:
```bash
mkdir ./build
```

Then, the `main` binary can be easly built:
```bash
cmake -B ./build -S ./ && cmake --build ./build
```

And run:
```bash
./build/main
```

## Custom targets

Currently supported custom commands:
- `clear` - removes whole `./build` directory and creates it once again, without any content
- `test` - runs the algorithm with sample data

Example usage:
```bash
cmake --build ./build --target clear
```

## Parallel environment

To unleash the parallel capability, firstly setup the environment, e.g.:
```bash
. ./setup.sh
```

Secondly, execute binary with `mpiexec`, e.g. using provided script:
```bash
./run.sh <custom-parameters>
```


## References

### External libraries

- [MPI](https://www.open-mpi.org/)
- [CLI11](https://github.com/CLIUtils/CLI11)
- [csv-parser](https://github.com/vincentlaucsb/csv-parser)


### Authors

- [Michał Rutkowski](https://github.com/P4ndaM1x)
- [Eryk Zarębski](https://github.com/erzar0)