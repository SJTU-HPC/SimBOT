#ifndef COMMAND
#define COMMAND
#include <stdio.h>
#include <argp.h>
#include <stdlib.h>
#include <string.h>

/* This structure is used by main to communicate with parse_opt. */
struct arguments
{
  int verbose;              /* The -v flag */
  int runnum;               /* Argument for -n */
  char *outfile;            /* Argument for -o */
  char *config;             /* Argument for -c */
};
extern struct arguments arguments;
void command_parse(int argc, char **argv);

#endif