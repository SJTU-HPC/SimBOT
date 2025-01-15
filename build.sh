#!/bin/sh

ROOT_PATH=$PWD
LOGGOPSIM_PATH=$ROOT_PATH/deps/LogGOPSim-1.1
SCHEDGEN_PATH=$ROOT_PATH/deps/Schedgen-1.1
GRAPHVIZ_PATH=$ROOT_PATH/deps/graphviz-8.0.5
NETGAUGE_PATH=$ROOT_PATH/deps/netgauge-2.4.6
GENGETOPT_PATH=$ROOT_PATH/deps/gengetopt-2.22.6

# build netgauge
cd $NETGAUGE_PATH
./configure --prefix=$NETGAUGE_PATH/netgauge-install MPICC=mpicc MPICXX=mpicxx
make -j
make install

#build gengetopt
cd $GENGETOPT_PATH
mkdir -p build && cd build
../configure --prefix=$GENGETOPT_PATH/gengetopt-install
make -j
make install
export PATH=$GENGETOPT_PATH/gengetopt-install/bin:$PATH

# build graphviz
cd $GRAPHVIZ_PATH
mkdir -p build && cd build
../configure --prefix=$GRAPHVIZ_PATH/graphviz-install
make -j
make install
export CPATH=$GRAPHVIZ_PATH/graphviz-install/include:$CPATH
export LD_LIBRARY_PATH=$GRAPHVIZ_PATH/graphviz-install/lib:$LD_LIBRARY_PATH
export LIBRARY_PATH=$GRAPHVIZ_PATH/graphviz-install/lib:$LIBRARY_PATH

# build LogGOPSim
source $LOGGOPSIM_PATH/env.sh
cd $LOGGOPSIM_PATH
make -j

# build Schedgen
cd $SCHEDGEN_PATH
make -j

# build SimBOT
source $SCHEDGEN_PATH/env.sh
cd $ROOT_PATH
make
