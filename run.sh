#/bin/bash

mpiexec -f ./nodes -n 2 ./build/main "$@"
