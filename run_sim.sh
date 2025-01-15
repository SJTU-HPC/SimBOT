#!/bin/sh

ROOT_PATH=$(cd $(dirname ${BASH_SOURCE[0]}); pwd)
GRAPHVIZ_PATH=$ROOT_PATH/deps/graphviz-8.0.5

# configuration
CONFIG_FILE_PATH=example/config.json
NP=64
SIM_TIMES=100
OUT_DIR=$ROOT_PATH/output

# environment val
LOGGOPSIM_PATH=$ROOT_PATH/deps/LogGOPSim-1.1
SCHEDGEN_PATH=$ROOT_PATH/deps/Schedgen-1.1
source $LOGGOPSIM_PATH/env.sh
source $SCHEDGEN_PATH/env.sh
export CPATH=$GRAPHVIZ_PATH/graphviz-install/include:$CPATH
export LD_LIBRARY_PATH=$GRAPHVIZ_PATH/graphviz-install/lib:$LD_LIBRARY_PATH
export LIBRARY_PATH=$GRAPHVIZ_PATH/graphviz-install/lib:$LIBRARY_PATH

# run
rm -rf OUT_DIR
mpirun -n $NP ./pfhsim -c $CONFIG_FILE_PATH -n $SIM_TIMES -o $OUT_DIR
