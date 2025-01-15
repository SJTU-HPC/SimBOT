#include <stdio.h>
#include "LogGOPSim.h"

/*
 * test case
*/
int main() {
  const int num_of_rank = 32;
  uint64_t init_bias[num_of_rank];
  uint64_t comm_time[num_of_rank];
  char comm_file[50] = "./schedule.bin";

  // 调用通信函数时，各进程初始相对时间
  for (int i = 0; i < num_of_rank; ++i) {
    init_bias[i] = 0;
  }

  int o = 8000;
  int O = 0;
  int g = 300 + 6500;
  int L = 252;
  int G = 0;
  int S = 500;

  simulation(o, O, g, L, G, S, comm_file, init_bias, comm_time, num_of_rank);

  printf("Times\n");
  for (int i = 0; i < num_of_rank; ++i) {
    printf("Host %i:%llu\n", i, comm_time[i]);
  }
}