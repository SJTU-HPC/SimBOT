CXX=g++
CXXFLAGS= -O0 -g -pedantic #-Wno-deprecated -Wall -Wno-long-long
CCFLAGS= -O0 -g
LDFLAGS= -lcgraph

AUTOGEN_SRC= cmdline.c cmdline.h
LOGGOPSIM_OBJECTS= LogGOPSim.o
HLPR_OBJECTS= cmdline.o
ALL_OBJECTS= $(LOGGOPSIM_OBJECTS) $(HLPR_OBJECTS)

all: $(ALL_OBJECTS) cmdline.h txt2bin
	$(CXX) $(CXXFLAGS) $(ALL_OBJECTS) -fPIC -shared -o libLogGOPSim_.so $(LDFLAGS)
	g++ -shared -fPIC -o libLogGOPSim.so wrapper.cpp -lLogGOPSim_
	gcc main.c -L. -I. -o loggops_main -lLogGOPSim

txt2bin:
	$(CXX) -g -O3 txt2bin.cpp cmdline_txt2bin.c -o txt2bin

%.o: %.cpp $(AUTOGEN_SRC) *.hpp 
	$(CXX) $(CXXFLAGS) -c $< -fPIC

%.o: %.c $(AUTOGEN_SRC) *.h
	$(CXX) $(CCFLAGS) -c $< -fPIC

clean:
	rm -f $(ALL_OBJECTS)
	rm -f txt2bin
	rm -f libLogGOPSim.so
	rm -f loggops_main
	rm -f libLogGOPSim_.so
