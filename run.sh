#/bin/bash

# mpiexec -f ./nodes -ppn 1 ./build/main "$@"

upcxx-run -n 8 $(upcxx-nodes ./nodes) ./build/main "$@"
