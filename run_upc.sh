#/bin/bash

upcxx-run -n 8 $(upcxx-nodes ./nodes) ./build/main "$@"
