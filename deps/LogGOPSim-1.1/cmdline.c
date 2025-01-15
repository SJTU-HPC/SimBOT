/*
  File autogenerated by gengetopt version 2.22.6
  generated with the following command:
  gengetopt 

  The developers of gengetopt consider the fixed text that goes in all
  gengetopt output files to be in the public domain:
  we make no copyright claims on it.
*/

/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef FIX_UNUSED
#define FIX_UNUSED(X) (void) (X) /* avoid warnings for unused params */
#endif

#include <getopt.h>

#include "cmdline.h"

const char *gengetopt_args_info_purpose = "";

const char *gengetopt_args_info_usage = "Usage: goalsim [OPTIONS]...";

const char *gengetopt_args_info_versiontext = "";

const char *gengetopt_args_info_description = "";

const char *gengetopt_args_info_help[] = {
  "  -h, --help                 Print help and exit",
  "      --version              Print version and exit",
  "  -f, --filename=STRING      Prefix for the filenames which contain the\n                               schedules",
  "      --save-mem             Map the schedules as MAP_SHARED to enable\n                               processing of large schedule (larger than main\n                               memory). This will invalidate the schedules\n                               during simulation.  (default=off)",
  "  -L, --LogGOPS_L=INT        The latency parameter L in the LogGP model\n                               (default=`2500')",
  "  -o, --LogGOPS_o=INT        The overhead parameter o in the LogGP model\n                               (default=`1500')",
  "  -g, --LogGOPS_g=INT        The gap per message parameter g in the LogGP model\n                               (default=`1000')",
  "  -G, --LogGOPS_G=INT        The gap per byte parameter G in the LogGP model\n                               (default=`6')",
  "  -S, --LogGOPS_S=INT        Datasize at which we change from eager to\n                               rendezvous protocol  (default=`65535')",
  "  -O, --LogGOPS_O=INT        The overhead per byte in LogGOP  (default=`0')",
  "  -V, --vizfile=STRING       Name of the output file for visualization data",
  "  -v, --verbose              Enable more verbose output",
  "      --progress             print progress",
  "  -b, --batchmode            enable batchmode (never print detailed host info)",
  "      --noise-trace=STRING   Read Noise from trace <file>",
  "      --noise-cosched        Co-schedule noise (use same starttime on all\n                               processes)  (default=off)",
  "  -n, --network-type=STRING  Network type (LogGP=no network congestion;\n                               simple=simple linear model)  (possible\n                               values=\"LogGP\", \"simple\" default=`LogGP')",
  "      --network-file=STRING  Input file for network (annotated dot format)",
    0
};

typedef enum {ARG_NO
  , ARG_FLAG
  , ARG_STRING
  , ARG_INT
} cmdline_parser_arg_type;

static
void clear_given (struct gengetopt_args_info *args_info);
static
void clear_args (struct gengetopt_args_info *args_info);

static int
cmdline_parser_internal (int argc, char **argv, struct gengetopt_args_info *args_info,
                        struct cmdline_parser_params *params, const char *additional_error);

static int
cmdline_parser_required2 (struct gengetopt_args_info *args_info, const char *prog_name, const char *additional_error);

const char *cmdline_parser_network_type_values[] = {"LogGP", "simple", 0}; /*< Possible values for network-type. */

static char *
gengetopt_strdup (const char *s);

static
void clear_given (struct gengetopt_args_info *args_info)
{
  args_info->help_given = 0 ;
  args_info->version_given = 0 ;
  args_info->filename_given = 0 ;
  args_info->save_mem_given = 0 ;
  args_info->LogGOPS_L_given = 0 ;
  args_info->LogGOPS_o_given = 0 ;
  args_info->LogGOPS_g_given = 0 ;
  args_info->LogGOPS_G_given = 0 ;
  args_info->LogGOPS_S_given = 0 ;
  args_info->LogGOPS_O_given = 0 ;
  args_info->vizfile_given = 0 ;
  args_info->verbose_given = 0 ;
  args_info->progress_given = 0 ;
  args_info->batchmode_given = 0 ;
  args_info->noise_trace_given = 0 ;
  args_info->noise_cosched_given = 0 ;
  args_info->network_type_given = 0 ;
  args_info->network_file_given = 0 ;
}

static
void clear_args (struct gengetopt_args_info *args_info)
{
  FIX_UNUSED (args_info);
  args_info->filename_arg = NULL;
  args_info->filename_orig = NULL;
  args_info->save_mem_flag = 0;
  args_info->LogGOPS_L_arg = 2500;
  args_info->LogGOPS_L_orig = NULL;
  args_info->LogGOPS_o_arg = 1500;
  args_info->LogGOPS_o_orig = NULL;
  args_info->LogGOPS_g_arg = 1000;
  args_info->LogGOPS_g_orig = NULL;
  args_info->LogGOPS_G_arg = 6;
  args_info->LogGOPS_G_orig = NULL;
  args_info->LogGOPS_S_arg = 65535;
  args_info->LogGOPS_S_orig = NULL;
  args_info->LogGOPS_O_arg = 0;
  args_info->LogGOPS_O_orig = NULL;
  args_info->vizfile_arg = NULL;
  args_info->vizfile_orig = NULL;
  args_info->noise_trace_arg = NULL;
  args_info->noise_trace_orig = NULL;
  args_info->noise_cosched_flag = 0;
  args_info->network_type_arg = gengetopt_strdup ("LogGP");
  args_info->network_type_orig = NULL;
  args_info->network_file_arg = NULL;
  args_info->network_file_orig = NULL;
  
}

static
void init_args_info(struct gengetopt_args_info *args_info)
{


  args_info->help_help = gengetopt_args_info_help[0] ;
  args_info->version_help = gengetopt_args_info_help[1] ;
  args_info->filename_help = gengetopt_args_info_help[2] ;
  args_info->save_mem_help = gengetopt_args_info_help[3] ;
  args_info->LogGOPS_L_help = gengetopt_args_info_help[4] ;
  args_info->LogGOPS_o_help = gengetopt_args_info_help[5] ;
  args_info->LogGOPS_g_help = gengetopt_args_info_help[6] ;
  args_info->LogGOPS_G_help = gengetopt_args_info_help[7] ;
  args_info->LogGOPS_S_help = gengetopt_args_info_help[8] ;
  args_info->LogGOPS_O_help = gengetopt_args_info_help[9] ;
  args_info->vizfile_help = gengetopt_args_info_help[10] ;
  args_info->verbose_help = gengetopt_args_info_help[11] ;
  args_info->progress_help = gengetopt_args_info_help[12] ;
  args_info->batchmode_help = gengetopt_args_info_help[13] ;
  args_info->noise_trace_help = gengetopt_args_info_help[14] ;
  args_info->noise_cosched_help = gengetopt_args_info_help[15] ;
  args_info->network_type_help = gengetopt_args_info_help[16] ;
  args_info->network_file_help = gengetopt_args_info_help[17] ;
  
}

void
cmdline_parser_print_version (void)
{
  printf ("%s %s\n",
     (strlen(CMDLINE_PARSER_PACKAGE_NAME) ? CMDLINE_PARSER_PACKAGE_NAME : CMDLINE_PARSER_PACKAGE),
     CMDLINE_PARSER_VERSION);

  if (strlen(gengetopt_args_info_versiontext) > 0)
    printf("\n%s\n", gengetopt_args_info_versiontext);
}

static void print_help_common(void) {
  cmdline_parser_print_version ();

  if (strlen(gengetopt_args_info_purpose) > 0)
    printf("\n%s\n", gengetopt_args_info_purpose);

  if (strlen(gengetopt_args_info_usage) > 0)
    printf("\n%s\n", gengetopt_args_info_usage);

  printf("\n");

  if (strlen(gengetopt_args_info_description) > 0)
    printf("%s\n\n", gengetopt_args_info_description);
}

void
cmdline_parser_print_help (void)
{
  int i = 0;
  print_help_common();
  while (gengetopt_args_info_help[i])
    printf("%s\n", gengetopt_args_info_help[i++]);
}

void
cmdline_parser_init (struct gengetopt_args_info *args_info)
{
  clear_given (args_info);
  clear_args (args_info);
  init_args_info (args_info);
}

void
cmdline_parser_params_init(struct cmdline_parser_params *params)
{
  if (params)
    { 
      params->override = 0;
      params->initialize = 1;
      params->check_required = 1;
      params->check_ambiguity = 0;
      params->print_errors = 1;
    }
}

struct cmdline_parser_params *
cmdline_parser_params_create(void)
{
  struct cmdline_parser_params *params = 
    (struct cmdline_parser_params *)malloc(sizeof(struct cmdline_parser_params));
  cmdline_parser_params_init(params);  
  return params;
}

static void
free_string_field (char **s)
{
  if (*s)
    {
      free (*s);
      *s = 0;
    }
}


static void
cmdline_parser_release (struct gengetopt_args_info *args_info)
{

  free_string_field (&(args_info->filename_arg));
  free_string_field (&(args_info->filename_orig));
  free_string_field (&(args_info->LogGOPS_L_orig));
  free_string_field (&(args_info->LogGOPS_o_orig));
  free_string_field (&(args_info->LogGOPS_g_orig));
  free_string_field (&(args_info->LogGOPS_G_orig));
  free_string_field (&(args_info->LogGOPS_S_orig));
  free_string_field (&(args_info->LogGOPS_O_orig));
  free_string_field (&(args_info->vizfile_arg));
  free_string_field (&(args_info->vizfile_orig));
  free_string_field (&(args_info->noise_trace_arg));
  free_string_field (&(args_info->noise_trace_orig));
  free_string_field (&(args_info->network_type_arg));
  free_string_field (&(args_info->network_type_orig));
  free_string_field (&(args_info->network_file_arg));
  free_string_field (&(args_info->network_file_orig));
  
  

  clear_given (args_info);
}

/**
 * @param val the value to check
 * @param values the possible values
 * @return the index of the matched value:
 * -1 if no value matched,
 * -2 if more than one value has matched
 */
static int
check_possible_values(const char *val, const char *values[])
{
  int i, found, last;
  size_t len;

  if (!val)   /* otherwise strlen() crashes below */
    return -1; /* -1 means no argument for the option */

  found = last = 0;

  for (i = 0, len = strlen(val); values[i]; ++i)
    {
      if (strncmp(val, values[i], len) == 0)
        {
          ++found;
          last = i;
          if (strlen(values[i]) == len)
            return i; /* exact macth no need to check more */
        }
    }

  if (found == 1) /* one match: OK */
    return last;

  return (found ? -2 : -1); /* return many values or none matched */
}


static void
write_into_file(FILE *outfile, const char *opt, const char *arg, const char *values[])
{
  int found = -1;
  if (arg) {
    if (values) {
      found = check_possible_values(arg, values);      
    }
    if (found >= 0)
      fprintf(outfile, "%s=\"%s\" # %s\n", opt, arg, values[found]);
    else
      fprintf(outfile, "%s=\"%s\"\n", opt, arg);
  } else {
    fprintf(outfile, "%s\n", opt);
  }
}


int
cmdline_parser_dump(FILE *outfile, struct gengetopt_args_info *args_info)
{
  int i = 0;

  if (!outfile)
    {
      fprintf (stderr, "%s: cannot dump options to stream\n", CMDLINE_PARSER_PACKAGE);
      return EXIT_FAILURE;
    }

  if (args_info->help_given)
    write_into_file(outfile, "help", 0, 0 );
  if (args_info->version_given)
    write_into_file(outfile, "version", 0, 0 );
  if (args_info->filename_given)
    write_into_file(outfile, "filename", args_info->filename_orig, 0);
  if (args_info->save_mem_given)
    write_into_file(outfile, "save-mem", 0, 0 );
  if (args_info->LogGOPS_L_given)
    write_into_file(outfile, "LogGOPS_L", args_info->LogGOPS_L_orig, 0);
  if (args_info->LogGOPS_o_given)
    write_into_file(outfile, "LogGOPS_o", args_info->LogGOPS_o_orig, 0);
  if (args_info->LogGOPS_g_given)
    write_into_file(outfile, "LogGOPS_g", args_info->LogGOPS_g_orig, 0);
  if (args_info->LogGOPS_G_given)
    write_into_file(outfile, "LogGOPS_G", args_info->LogGOPS_G_orig, 0);
  if (args_info->LogGOPS_S_given)
    write_into_file(outfile, "LogGOPS_S", args_info->LogGOPS_S_orig, 0);
  if (args_info->LogGOPS_O_given)
    write_into_file(outfile, "LogGOPS_O", args_info->LogGOPS_O_orig, 0);
  if (args_info->vizfile_given)
    write_into_file(outfile, "vizfile", args_info->vizfile_orig, 0);
  if (args_info->verbose_given)
    write_into_file(outfile, "verbose", 0, 0 );
  if (args_info->progress_given)
    write_into_file(outfile, "progress", 0, 0 );
  if (args_info->batchmode_given)
    write_into_file(outfile, "batchmode", 0, 0 );
  if (args_info->noise_trace_given)
    write_into_file(outfile, "noise-trace", args_info->noise_trace_orig, 0);
  if (args_info->noise_cosched_given)
    write_into_file(outfile, "noise-cosched", 0, 0 );
  if (args_info->network_type_given)
    write_into_file(outfile, "network-type", args_info->network_type_orig, cmdline_parser_network_type_values);
  if (args_info->network_file_given)
    write_into_file(outfile, "network-file", args_info->network_file_orig, 0);
  

  i = EXIT_SUCCESS;
  return i;
}

int
cmdline_parser_file_save(const char *filename, struct gengetopt_args_info *args_info)
{
  FILE *outfile;
  int i = 0;

  outfile = fopen(filename, "w");

  if (!outfile)
    {
      fprintf (stderr, "%s: cannot open file for writing: %s\n", CMDLINE_PARSER_PACKAGE, filename);
      return EXIT_FAILURE;
    }

  i = cmdline_parser_dump(outfile, args_info);
  fclose (outfile);

  return i;
}

void
cmdline_parser_free (struct gengetopt_args_info *args_info)
{
  cmdline_parser_release (args_info);
}

/** @brief replacement of strdup, which is not standard */
char *
gengetopt_strdup (const char *s)
{
  char *result = 0;
  if (!s)
    return result;

  result = (char*)malloc(strlen(s) + 1);
  if (result == (char*)0)
    return (char*)0;
  strcpy(result, s);
  return result;
}

int
cmdline_parser (int argc, char **argv, struct gengetopt_args_info *args_info)
{

  return cmdline_parser2 (argc, argv, args_info, 0, 1, 1);
}

int
cmdline_parser_ext (int argc, char **argv, struct gengetopt_args_info *args_info,
                   struct cmdline_parser_params *params)
{
  int result;
  result = cmdline_parser_internal (argc, argv, args_info, params, 0);

  if (result == EXIT_FAILURE)
    {
      cmdline_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

int
cmdline_parser2 (int argc, char **argv, struct gengetopt_args_info *args_info, int override, int initialize, int check_required)
{
  int result;
  struct cmdline_parser_params params;
  
  params.override = override;
  params.initialize = initialize;
  params.check_required = check_required;
  params.check_ambiguity = 0;
  params.print_errors = 1;

  result = cmdline_parser_internal (argc, argv, args_info, &params, 0);

  if (result == EXIT_FAILURE)
    {
      cmdline_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

int
cmdline_parser_required (struct gengetopt_args_info *args_info, const char *prog_name)
{
  int result = EXIT_SUCCESS;

  if (cmdline_parser_required2(args_info, prog_name, 0) > 0)
    result = EXIT_FAILURE;

  if (result == EXIT_FAILURE)
    {
      cmdline_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

int
cmdline_parser_required2 (struct gengetopt_args_info *args_info, const char *prog_name, const char *additional_error)
{
  int error_occurred = 0;
  FIX_UNUSED (additional_error);

  /* checks for required options */
  /*if (! args_info->filename_given)
    {
      fprintf (stderr, "%s: '--filename' ('-f') option required%s\n", prog_name, (additional_error ? additional_error : ""));
      error_occurred = 1;
    }
  */
  
  /* checks for dependences among options */

  return error_occurred;
}


static char *package_name = 0;

/**
 * @brief updates an option
 * @param field the generic pointer to the field to update
 * @param orig_field the pointer to the orig field
 * @param field_given the pointer to the number of occurrence of this option
 * @param prev_given the pointer to the number of occurrence already seen
 * @param value the argument for this option (if null no arg was specified)
 * @param possible_values the possible values for this option (if specified)
 * @param default_value the default value (in case the option only accepts fixed values)
 * @param arg_type the type of this option
 * @param check_ambiguity @see cmdline_parser_params.check_ambiguity
 * @param override @see cmdline_parser_params.override
 * @param no_free whether to free a possible previous value
 * @param multiple_option whether this is a multiple option
 * @param long_opt the corresponding long option
 * @param short_opt the corresponding short option (or '-' if none)
 * @param additional_error possible further error specification
 */
static
int update_arg(void *field, char **orig_field,
               unsigned int *field_given, unsigned int *prev_given, 
               char *value, const char *possible_values[],
               const char *default_value,
               cmdline_parser_arg_type arg_type,
               int check_ambiguity, int override,
               int no_free, int multiple_option,
               const char *long_opt, char short_opt,
               const char *additional_error)
{
  char *stop_char = 0;
  const char *val = value;
  int found;
  char **string_field;
  FIX_UNUSED (field);

  stop_char = 0;
  found = 0;

  if (!multiple_option && prev_given && (*prev_given || (check_ambiguity && *field_given)))
    {
      if (short_opt != '-')
        fprintf (stderr, "%s: `--%s' (`-%c') option given more than once%s\n", 
               package_name, long_opt, short_opt,
               (additional_error ? additional_error : ""));
      else
        fprintf (stderr, "%s: `--%s' option given more than once%s\n", 
               package_name, long_opt,
               (additional_error ? additional_error : ""));
      return 1; /* failure */
    }

  if (possible_values && (found = check_possible_values((value ? value : default_value), possible_values)) < 0)
    {
      if (short_opt != '-')
        fprintf (stderr, "%s: %s argument, \"%s\", for option `--%s' (`-%c')%s\n", 
          package_name, (found == -2) ? "ambiguous" : "invalid", value, long_opt, short_opt,
          (additional_error ? additional_error : ""));
      else
        fprintf (stderr, "%s: %s argument, \"%s\", for option `--%s'%s\n", 
          package_name, (found == -2) ? "ambiguous" : "invalid", value, long_opt,
          (additional_error ? additional_error : ""));
      return 1; /* failure */
    }
    
  if (field_given && *field_given && ! override)
    return 0;
  if (prev_given)
    (*prev_given)++;
  if (field_given)
    (*field_given)++;
  if (possible_values)
    val = possible_values[found];

  switch(arg_type) {
  case ARG_FLAG:
    *((int *)field) = !*((int *)field);
    break;
  case ARG_INT:
    if (val) *((int *)field) = strtol (val, &stop_char, 0);
    break;
  case ARG_STRING:
    if (val) {
      string_field = (char **)field;
      if (!no_free && *string_field)
        free (*string_field); /* free previous string */
      *string_field = gengetopt_strdup (val);
    }
    break;
  default:
    break;
  };

  /* check numeric conversion */
  switch(arg_type) {
  case ARG_INT:
    if (val && !(stop_char && *stop_char == '\0')) {
      fprintf(stderr, "%s: invalid numeric value: %s\n", package_name, val);
      return 1; /* failure */
    }
    break;
  default:
    ;
  };

  /* store the original value */
  switch(arg_type) {
  case ARG_NO:
  case ARG_FLAG:
    break;
  default:
    if (value && orig_field) {
      if (no_free) {
        *orig_field = value;
      } else {
        if (*orig_field)
          free (*orig_field); /* free previous string */
        *orig_field = gengetopt_strdup (value);
      }
    }
  };

  return 0; /* OK */
}


int
cmdline_parser_internal (
  int argc, char **argv, struct gengetopt_args_info *args_info,
                        struct cmdline_parser_params *params, const char *additional_error)
{
  int c;	/* Character of the parsed option.  */

  int error_occurred = 0;
  struct gengetopt_args_info local_args_info;
  int override;
  int initialize;
  int check_required;
  int check_ambiguity;
  
  package_name = argv[0];
  
  override = params->override;
  initialize = params->initialize;
  check_required = params->check_required;
  check_ambiguity = params->check_ambiguity;

  if (initialize)
    cmdline_parser_init (args_info);

  cmdline_parser_init (&local_args_info);

  optarg = 0;
  optind = 0;
  opterr = params->print_errors;
  optopt = '?';

  while (1)
    {
      int option_index = 0;

      static struct option long_options[] = {
        { "help",	0, NULL, 'h' },
        { "version",	0, NULL, 0 },
        { "filename",	1, NULL, 'f' },
        { "save-mem",	0, NULL, 0 },
        { "LogGOPS_L",	1, NULL, 'L' },
        { "LogGOPS_o",	1, NULL, 'o' },
        { "LogGOPS_g",	1, NULL, 'g' },
        { "LogGOPS_G",	1, NULL, 'G' },
        { "LogGOPS_S",	1, NULL, 'S' },
        { "LogGOPS_O",	1, NULL, 'O' },
        { "vizfile",	1, NULL, 'V' },
        { "verbose",	0, NULL, 'v' },
        { "progress",	0, NULL, 0 },
        { "batchmode",	0, NULL, 'b' },
        { "noise-trace",	1, NULL, 0 },
        { "noise-cosched",	0, NULL, 0 },
        { "network-type",	1, NULL, 'n' },
        { "network-file",	1, NULL, 0 },
        { 0,  0, 0, 0 }
      };

      c = getopt_long (argc, argv, "hf:L:o:g:G:S:O:V:vbn:", long_options, &option_index);

      if (c == -1) break;	/* Exit from `while (1)' loop.  */

      switch (c)
        {
        case 'h':	/* Print help and exit.  */
          cmdline_parser_print_help ();
          cmdline_parser_free (&local_args_info);
          exit (EXIT_SUCCESS);

        case 'f':	/* Prefix for the filenames which contain the schedules.  */
        
        
          if (update_arg( (void *)&(args_info->filename_arg), 
               &(args_info->filename_orig), &(args_info->filename_given),
              &(local_args_info.filename_given), optarg, 0, 0, ARG_STRING,
              check_ambiguity, override, 0, 0,
              "filename", 'f',
              additional_error))
            goto failure;
        
          break;
        case 'L':	/* The latency parameter L in the LogGP model.  */
        
        
          if (update_arg( (void *)&(args_info->LogGOPS_L_arg), 
               &(args_info->LogGOPS_L_orig), &(args_info->LogGOPS_L_given),
              &(local_args_info.LogGOPS_L_given), optarg, 0, "2500", ARG_INT,
              check_ambiguity, override, 0, 0,
              "LogGOPS_L", 'L',
              additional_error))
            goto failure;
        
          break;
        case 'o':	/* The overhead parameter o in the LogGP model.  */
        
        
          if (update_arg( (void *)&(args_info->LogGOPS_o_arg), 
               &(args_info->LogGOPS_o_orig), &(args_info->LogGOPS_o_given),
              &(local_args_info.LogGOPS_o_given), optarg, 0, "1500", ARG_INT,
              check_ambiguity, override, 0, 0,
              "LogGOPS_o", 'o',
              additional_error))
            goto failure;
        
          break;
        case 'g':	/* The gap per message parameter g in the LogGP model.  */
        
        
          if (update_arg( (void *)&(args_info->LogGOPS_g_arg), 
               &(args_info->LogGOPS_g_orig), &(args_info->LogGOPS_g_given),
              &(local_args_info.LogGOPS_g_given), optarg, 0, "1000", ARG_INT,
              check_ambiguity, override, 0, 0,
              "LogGOPS_g", 'g',
              additional_error))
            goto failure;
        
          break;
        case 'G':	/* The gap per byte parameter G in the LogGP model.  */
        
        
          if (update_arg( (void *)&(args_info->LogGOPS_G_arg), 
               &(args_info->LogGOPS_G_orig), &(args_info->LogGOPS_G_given),
              &(local_args_info.LogGOPS_G_given), optarg, 0, "6", ARG_INT,
              check_ambiguity, override, 0, 0,
              "LogGOPS_G", 'G',
              additional_error))
            goto failure;
        
          break;
        case 'S':	/* Datasize at which we change from eager to rendezvous protocol.  */
        
        
          if (update_arg( (void *)&(args_info->LogGOPS_S_arg), 
               &(args_info->LogGOPS_S_orig), &(args_info->LogGOPS_S_given),
              &(local_args_info.LogGOPS_S_given), optarg, 0, "65535", ARG_INT,
              check_ambiguity, override, 0, 0,
              "LogGOPS_S", 'S',
              additional_error))
            goto failure;
        
          break;
        case 'O':	/* The overhead per byte in LogGOP.  */
        
        
          if (update_arg( (void *)&(args_info->LogGOPS_O_arg), 
               &(args_info->LogGOPS_O_orig), &(args_info->LogGOPS_O_given),
              &(local_args_info.LogGOPS_O_given), optarg, 0, "0", ARG_INT,
              check_ambiguity, override, 0, 0,
              "LogGOPS_O", 'O',
              additional_error))
            goto failure;
        
          break;
        case 'V':	/* Name of the output file for visualization data.  */
        
        
          if (update_arg( (void *)&(args_info->vizfile_arg), 
               &(args_info->vizfile_orig), &(args_info->vizfile_given),
              &(local_args_info.vizfile_given), optarg, 0, 0, ARG_STRING,
              check_ambiguity, override, 0, 0,
              "vizfile", 'V',
              additional_error))
            goto failure;
        
          break;
        case 'v':	/* Enable more verbose output.  */
        
        
          if (update_arg( 0 , 
               0 , &(args_info->verbose_given),
              &(local_args_info.verbose_given), optarg, 0, 0, ARG_NO,
              check_ambiguity, override, 0, 0,
              "verbose", 'v',
              additional_error))
            goto failure;
        
          break;
        case 'b':	/* enable batchmode (never print detailed host info).  */
        
        
          if (update_arg( 0 , 
               0 , &(args_info->batchmode_given),
              &(local_args_info.batchmode_given), optarg, 0, 0, ARG_NO,
              check_ambiguity, override, 0, 0,
              "batchmode", 'b',
              additional_error))
            goto failure;
        
          break;
        case 'n':	/* Network type (LogGP=no network congestion; simple=simple linear model).  */
        
        
          if (update_arg( (void *)&(args_info->network_type_arg), 
               &(args_info->network_type_orig), &(args_info->network_type_given),
              &(local_args_info.network_type_given), optarg, cmdline_parser_network_type_values, "LogGP", ARG_STRING,
              check_ambiguity, override, 0, 0,
              "network-type", 'n',
              additional_error))
            goto failure;
        
          break;

        case 0:	/* Long option with no short option */
          if (strcmp (long_options[option_index].name, "version") == 0) {
            cmdline_parser_print_version ();
            cmdline_parser_free (&local_args_info);
            exit (EXIT_SUCCESS);
          }

          /* Map the schedules as MAP_SHARED to enable processing of large schedule (larger than main memory). This will invalidate the schedules during simulation..  */
          if (strcmp (long_options[option_index].name, "save-mem") == 0)
          {
          
          
            if (update_arg((void *)&(args_info->save_mem_flag), 0, &(args_info->save_mem_given),
                &(local_args_info.save_mem_given), optarg, 0, 0, ARG_FLAG,
                check_ambiguity, override, 1, 0, "save-mem", '-',
                additional_error))
              goto failure;
          
          }
          /* print progress.  */
          else if (strcmp (long_options[option_index].name, "progress") == 0)
          {
          
          
            if (update_arg( 0 , 
                 0 , &(args_info->progress_given),
                &(local_args_info.progress_given), optarg, 0, 0, ARG_NO,
                check_ambiguity, override, 0, 0,
                "progress", '-',
                additional_error))
              goto failure;
          
          }
          /* Read Noise from trace <file>.  */
          else if (strcmp (long_options[option_index].name, "noise-trace") == 0)
          {
          
          
            if (update_arg( (void *)&(args_info->noise_trace_arg), 
                 &(args_info->noise_trace_orig), &(args_info->noise_trace_given),
                &(local_args_info.noise_trace_given), optarg, 0, 0, ARG_STRING,
                check_ambiguity, override, 0, 0,
                "noise-trace", '-',
                additional_error))
              goto failure;
          
          }
          /* Co-schedule noise (use same starttime on all processes).  */
          else if (strcmp (long_options[option_index].name, "noise-cosched") == 0)
          {
          
          
            if (update_arg((void *)&(args_info->noise_cosched_flag), 0, &(args_info->noise_cosched_given),
                &(local_args_info.noise_cosched_given), optarg, 0, 0, ARG_FLAG,
                check_ambiguity, override, 1, 0, "noise-cosched", '-',
                additional_error))
              goto failure;
          
          }
          /* Input file for network (annotated dot format).  */
          else if (strcmp (long_options[option_index].name, "network-file") == 0)
          {
          
          
            if (update_arg( (void *)&(args_info->network_file_arg), 
                 &(args_info->network_file_orig), &(args_info->network_file_given),
                &(local_args_info.network_file_given), optarg, 0, 0, ARG_STRING,
                check_ambiguity, override, 0, 0,
                "network-file", '-',
                additional_error))
              goto failure;
          
          }
          
          break;
        case '?':	/* Invalid option.  */
          /* `getopt_long' already printed an error message.  */
          goto failure;

        default:	/* bug: option not considered.  */
          fprintf (stderr, "%s: option unknown: %c%s\n", CMDLINE_PARSER_PACKAGE, c, (additional_error ? additional_error : ""));
          abort ();
        } /* switch */
    } /* while */



  if (check_required)
    {
      error_occurred += cmdline_parser_required2 (args_info, argv[0], additional_error);
    }

  cmdline_parser_release (&local_args_info);

  if ( error_occurred )
    return (EXIT_FAILURE);

  return 0;

failure:
  
  cmdline_parser_release (&local_args_info);
  return (EXIT_FAILURE);
}
