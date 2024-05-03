#/bin/bash

echo "testing machines connectivity:"
/opt/nfs/config/station204_name_list.sh 1 2 | tee nodes

echo "sourcing mpich:"
source /opt/nfs/config/source_mpich401.sh
echo "success"
echo "sourcing cuda:"
source /opt/nfs/config/source_cuda121.sh
echo "success"

echo "testing mpexec:"
mpiexec -f nodes -n 16 uname -n | sort
echo "complete"
