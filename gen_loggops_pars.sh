#!/bin/sh
ROOT_PATH=$(cd $(dirname ${BASH_SOURCE[0]}); pwd)
NETGAUGE_PATH=$ROOT_PATH/deps/netgauge-2.4.6

export PATH=$NETGAUGE_PATH/netgauge-install/bin:$PATH

taskset -c 0-1 mpirun -n 2 --mca opal_common_ucx_opal_mem_hooks 1 \
    --host node:1,node:1 netgauge -size=700417 -m mpi -o loggops_pars -x loggp
