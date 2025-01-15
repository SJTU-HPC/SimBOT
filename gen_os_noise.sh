#!/bin/sh
ROOT_PATH=$(cd $(dirname ${BASH_SOURCE[0]}); pwd)
NETGAUGE_PATH=$ROOT_PATH/deps/netgauge-2.4.6

export PATH=$NETGAUGE_PATH/netgauge-install/bin:$PATH

TRACE_DIR=os_trace
CORE_NUM=64
SAMPLE_NUM=500000

mkdir -p $TRACE_DIR && cd $TRACE_DIR
mpirun -n $CORE_NUM --bind-to core --map-by slot:PE=1 netgauge -o trace -x noise -s $SAMPLE_NUM -w
