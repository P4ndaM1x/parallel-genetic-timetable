# parallel-genetic-timetable

This repository contains a C++ program that solves the School Timetabling Problem using a genetic algorithm parallelized with MPI (Message Passing Interface).

## Quickstart

> [!NOTE]
> All the following commands should be executed from the root of the project.

To unleash the parallel capability, firstly setup the environment, e.g.:
```bash
. ./setup.sh
```

Since in-source building is forbidden, creating build directory for CMake is mandatory:
```bash
mkdir ./build
```

### UPC++
When using UPC++ as a backend, the `main` binary can be easily built:
```bash
cmake -D UPCXX_ENABLE=ON -B ./build -S ./ && cmake --build ./build
```

And then executed  with `upcxx-run`, e.g. using provided script:
```bash
./run_upc.sh <custom-parameters>
```

### MPI
When using MPI as a backend, the `main` binary can be easily built:
```bash
cmake -D UPCXX_ENABLE=OFF -B ./build -S ./ && cmake --build ./build
```

And then executed  with `mpiexec`, e.g. using provided script:
```bash
./run_mpi.sh <custom-parameters>
```

## Custom targets

Currently supported custom commands:
- `clear` - removes whole `./build` directory and creates it once again, without any content
- `test` - runs the algorithm with sample data

Example usage:
```bash
cmake --build ./build --target test
```

## References

### External libraries

- [UPC++](https://bitbucket.org/berkeleylab/upcxx/wiki/Home)
- [MPI](https://www.open-mpi.org/)
- [CLI11](https://github.com/CLIUtils/CLI11)
- [csv-parser](https://github.com/vincentlaucsb/csv-parser)


### Authors

- [Michał Rutkowski](https://github.com/P4ndaM1x)
- [Eryk Zarębski](https://github.com/erzar0)
