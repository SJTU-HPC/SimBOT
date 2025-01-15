#include "command.h"

const char *argp_program_version ="SimBot 1.0";
const char *argp_program_bug_address = 0;

static struct argp_option options[] =
{
  {"verbose", 'v', 0, 0, "Produce verbose output"},
  {"ntime",   'n', "TIMES", 0,
   "Number of simulations"},
  {"config",   'c', "FILE", 0,
   "Path to configuration file"},
  {"output",  'o', "FILE", 0,
   "Output to OUTFILE instead of to standard output"},
  {0}
};

static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  struct arguments *arguments = state->input;
  switch (key) {
    case 'v':
      arguments->verbose = 1;
      break;
    case 'n':
      arguments->runnum = atoi(arg);
      break;
    case 'c':
      arguments->config = arg;
      break;
    case 'o':
      arguments->outfile = arg;
      break;
  }
  return 0;
}

static struct argp argp = {options, parse_opt, 0, 0};

struct arguments arguments;

void command_parse(int argc, char **argv){
  char* cmd;
  /* Set argument defaults */
  arguments.outfile = "out";
  arguments.config = "config.json";
  arguments.runnum = 50;
  arguments.verbose = 0;

  /* parse arguments */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  /* output/ -> output */
  if (arguments.outfile[strlen(arguments.outfile)] == '/') {
    arguments.outfile[strlen(arguments.outfile)] = 0;
  }
  /* make directory */
  cmd = (char*) malloc(sizeof(char) * (FILENAME_MAX + 8));
  sprintf(cmd, "%s %s", "mkdir -p", arguments.outfile);
  system(cmd);
  free(cmd);

  return;
}