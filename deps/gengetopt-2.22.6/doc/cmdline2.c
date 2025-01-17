/*
  File autogenerated by gengetopt version 2.22.6
  generated with the following command:
  ../src/gengetopt --input=../../doc/sample2.ggo --func-name=my_cmdline_parser --file-name=cmdline2 --unamed-opts --no-handle-help --no-handle-version 

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

#include "cmdline2.h"

const char *gengetopt_args_info_purpose = "this is just a test program for gengetopt";

const char *gengetopt_args_info_usage = "Usage: " MY_CMDLINE_PARSER_PACKAGE " [OPTIONS]... [FILES]...";

const char *gengetopt_args_info_versiontext = "";

const char *gengetopt_args_info_description = "";

const char *gengetopt_args_info_full_help[] = {
  "  -h, --help                    Print help and exit",
  "      --full-help               Print help, including hidden options, and exit",
  "  -V, --version                 Print version and exit",
  "  -s, --str-opt=STRING          A string option",
  "  -i, --int-opt=INT             A int option",
  "  -S, --short-opt=SHORT         A short option",
  "  -l, --long-opt=LONG           A long option",
  "  -f, --float-opt=FLOAT         A float option",
  "  -d, --double-opt=DOUBLE       A double option",
  "  -L, --long-double-opt=LONGDOUBLE\n                                A long double option",
  "  -y, --long-long-opt=LONGLONG  A long long option",
  "  -F, --func-opt                A function option",
  "  -N, --hidden-opt              A hidden option",
  "  -x, --flag-opt                A flag option  (default=off)",
    0
};

static void
init_help_array(void)
{
  gengetopt_args_info_help[0] = gengetopt_args_info_full_help[0];
  gengetopt_args_info_help[1] = gengetopt_args_info_full_help[1];
  gengetopt_args_info_help[2] = gengetopt_args_info_full_help[2];
  gengetopt_args_info_help[3] = gengetopt_args_info_full_help[3];
  gengetopt_args_info_help[4] = gengetopt_args_info_full_help[4];
  gengetopt_args_info_help[5] = gengetopt_args_info_full_help[5];
  gengetopt_args_info_help[6] = gengetopt_args_info_full_help[6];
  gengetopt_args_info_help[7] = gengetopt_args_info_full_help[7];
  gengetopt_args_info_help[8] = gengetopt_args_info_full_help[8];
  gengetopt_args_info_help[9] = gengetopt_args_info_full_help[9];
  gengetopt_args_info_help[10] = gengetopt_args_info_full_help[10];
  gengetopt_args_info_help[11] = gengetopt_args_info_full_help[11];
  gengetopt_args_info_help[12] = gengetopt_args_info_full_help[13];
  gengetopt_args_info_help[13] = 0; 
  
}

const char *gengetopt_args_info_help[14];

typedef enum {ARG_NO
  , ARG_FLAG
  , ARG_STRING
  , ARG_INT
  , ARG_SHORT
  , ARG_LONG
  , ARG_FLOAT
  , ARG_DOUBLE
  , ARG_LONGDOUBLE
  , ARG_LONGLONG
} my_cmdline_parser_arg_type;

static
void clear_given (struct gengetopt_args_info *args_info);
static
void clear_args (struct gengetopt_args_info *args_info);

static int
my_cmdline_parser_internal (int argc, char **argv, struct gengetopt_args_info *args_info,
                        struct my_cmdline_parser_params *params, const char *additional_error);

static int
my_cmdline_parser_required2 (struct gengetopt_args_info *args_info, const char *prog_name, const char *additional_error);

static char *
gengetopt_strdup (const char *s);

static
void clear_given (struct gengetopt_args_info *args_info)
{
  args_info->help_given = 0 ;
  args_info->full_help_given = 0 ;
  args_info->version_given = 0 ;
  args_info->str_opt_given = 0 ;
  args_info->int_opt_given = 0 ;
  args_info->short_opt_given = 0 ;
  args_info->long_opt_given = 0 ;
  args_info->float_opt_given = 0 ;
  args_info->double_opt_given = 0 ;
  args_info->long_double_opt_given = 0 ;
  args_info->long_long_opt_given = 0 ;
  args_info->func_opt_given = 0 ;
  args_info->hidden_opt_given = 0 ;
  args_info->flag_opt_given = 0 ;
}

static
void clear_args (struct gengetopt_args_info *args_info)
{
  FIX_UNUSED (args_info);
  args_info->str_opt_arg = NULL;
  args_info->str_opt_orig = NULL;
  args_info->int_opt_orig = NULL;
  args_info->short_opt_orig = NULL;
  args_info->long_opt_orig = NULL;
  args_info->float_opt_orig = NULL;
  args_info->double_opt_orig = NULL;
  args_info->long_double_opt_orig = NULL;
  args_info->long_long_opt_orig = NULL;
  args_info->flag_opt_flag = 0;
  
}

static
void init_args_info(struct gengetopt_args_info *args_info)
{

  init_help_array(); 
  args_info->help_help = gengetopt_args_info_full_help[0] ;
  args_info->full_help_help = gengetopt_args_info_full_help[1] ;
  args_info->version_help = gengetopt_args_info_full_help[2] ;
  args_info->str_opt_help = gengetopt_args_info_full_help[3] ;
  args_info->int_opt_help = gengetopt_args_info_full_help[4] ;
  args_info->short_opt_help = gengetopt_args_info_full_help[5] ;
  args_info->long_opt_help = gengetopt_args_info_full_help[6] ;
  args_info->float_opt_help = gengetopt_args_info_full_help[7] ;
  args_info->double_opt_help = gengetopt_args_info_full_help[8] ;
  args_info->long_double_opt_help = gengetopt_args_info_full_help[9] ;
  args_info->long_long_opt_help = gengetopt_args_info_full_help[10] ;
  args_info->func_opt_help = gengetopt_args_info_full_help[11] ;
  args_info->hidden_opt_help = gengetopt_args_info_full_help[12] ;
  args_info->flag_opt_help = gengetopt_args_info_full_help[13] ;
  
}

void
my_cmdline_parser_print_version (void)
{
  printf ("%s %s\n",
     (strlen(MY_CMDLINE_PARSER_PACKAGE_NAME) ? MY_CMDLINE_PARSER_PACKAGE_NAME : MY_CMDLINE_PARSER_PACKAGE),
     MY_CMDLINE_PARSER_VERSION);

  if (strlen(gengetopt_args_info_versiontext) > 0)
    printf("\n%s\n", gengetopt_args_info_versiontext);
}

static void print_help_common(void) {
  my_cmdline_parser_print_version ();

  if (strlen(gengetopt_args_info_purpose) > 0)
    printf("\n%s\n", gengetopt_args_info_purpose);

  if (strlen(gengetopt_args_info_usage) > 0)
    printf("\n%s\n", gengetopt_args_info_usage);

  printf("\n");

  if (strlen(gengetopt_args_info_description) > 0)
    printf("%s\n\n", gengetopt_args_info_description);
}

void
my_cmdline_parser_print_help (void)
{
  int i = 0;
  print_help_common();
  while (gengetopt_args_info_help[i])
    printf("%s\n", gengetopt_args_info_help[i++]);
}

void
my_cmdline_parser_print_full_help (void)
{
  int i = 0;
  print_help_common();
  while (gengetopt_args_info_full_help[i])
    printf("%s\n", gengetopt_args_info_full_help[i++]);
}

void
my_cmdline_parser_init (struct gengetopt_args_info *args_info)
{
  clear_given (args_info);
  clear_args (args_info);
  init_args_info (args_info);

  args_info->inputs = 0;
  args_info->inputs_num = 0;
}

void
my_cmdline_parser_params_init(struct my_cmdline_parser_params *params)
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

struct my_cmdline_parser_params *
my_cmdline_parser_params_create(void)
{
  struct my_cmdline_parser_params *params = 
    (struct my_cmdline_parser_params *)malloc(sizeof(struct my_cmdline_parser_params));
  my_cmdline_parser_params_init(params);  
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
my_cmdline_parser_release (struct gengetopt_args_info *args_info)
{
  unsigned int i;
  free_string_field (&(args_info->str_opt_arg));
  free_string_field (&(args_info->str_opt_orig));
  free_string_field (&(args_info->int_opt_orig));
  free_string_field (&(args_info->short_opt_orig));
  free_string_field (&(args_info->long_opt_orig));
  free_string_field (&(args_info->float_opt_orig));
  free_string_field (&(args_info->double_opt_orig));
  free_string_field (&(args_info->long_double_opt_orig));
  free_string_field (&(args_info->long_long_opt_orig));
  
  
  for (i = 0; i < args_info->inputs_num; ++i)
    free (args_info->inputs [i]);

  if (args_info->inputs_num)
    free (args_info->inputs);

  clear_given (args_info);
}


static void
write_into_file(FILE *outfile, const char *opt, const char *arg, const char *values[])
{
  FIX_UNUSED (values);
  if (arg) {
    fprintf(outfile, "%s=\"%s\"\n", opt, arg);
  } else {
    fprintf(outfile, "%s\n", opt);
  }
}


int
my_cmdline_parser_dump(FILE *outfile, struct gengetopt_args_info *args_info)
{
  int i = 0;

  if (!outfile)
    {
      fprintf (stderr, "%s: cannot dump options to stream\n", MY_CMDLINE_PARSER_PACKAGE);
      return EXIT_FAILURE;
    }

  if (args_info->help_given)
    write_into_file(outfile, "help", 0, 0 );
  if (args_info->full_help_given)
    write_into_file(outfile, "full-help", 0, 0 );
  if (args_info->version_given)
    write_into_file(outfile, "version", 0, 0 );
  if (args_info->str_opt_given)
    write_into_file(outfile, "str-opt", args_info->str_opt_orig, 0);
  if (args_info->int_opt_given)
    write_into_file(outfile, "int-opt", args_info->int_opt_orig, 0);
  if (args_info->short_opt_given)
    write_into_file(outfile, "short-opt", args_info->short_opt_orig, 0);
  if (args_info->long_opt_given)
    write_into_file(outfile, "long-opt", args_info->long_opt_orig, 0);
  if (args_info->float_opt_given)
    write_into_file(outfile, "float-opt", args_info->float_opt_orig, 0);
  if (args_info->double_opt_given)
    write_into_file(outfile, "double-opt", args_info->double_opt_orig, 0);
  if (args_info->long_double_opt_given)
    write_into_file(outfile, "long-double-opt", args_info->long_double_opt_orig, 0);
  if (args_info->long_long_opt_given)
    write_into_file(outfile, "long-long-opt", args_info->long_long_opt_orig, 0);
  if (args_info->func_opt_given)
    write_into_file(outfile, "func-opt", 0, 0 );
  if (args_info->hidden_opt_given)
    write_into_file(outfile, "hidden-opt", 0, 0 );
  if (args_info->flag_opt_given)
    write_into_file(outfile, "flag-opt", 0, 0 );
  

  i = EXIT_SUCCESS;
  return i;
}

int
my_cmdline_parser_file_save(const char *filename, struct gengetopt_args_info *args_info)
{
  FILE *outfile;
  int i = 0;

  outfile = fopen(filename, "w");

  if (!outfile)
    {
      fprintf (stderr, "%s: cannot open file for writing: %s\n", MY_CMDLINE_PARSER_PACKAGE, filename);
      return EXIT_FAILURE;
    }

  i = my_cmdline_parser_dump(outfile, args_info);
  fclose (outfile);

  return i;
}

void
my_cmdline_parser_free (struct gengetopt_args_info *args_info)
{
  my_cmdline_parser_release (args_info);
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
my_cmdline_parser (int argc, char **argv, struct gengetopt_args_info *args_info)
{
  return my_cmdline_parser2 (argc, argv, args_info, 0, 1, 1);
}

int
my_cmdline_parser_ext (int argc, char **argv, struct gengetopt_args_info *args_info,
                   struct my_cmdline_parser_params *params)
{
  int result;
  result = my_cmdline_parser_internal (argc, argv, args_info, params, 0);

  if (result == EXIT_FAILURE)
    {
      my_cmdline_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

int
my_cmdline_parser2 (int argc, char **argv, struct gengetopt_args_info *args_info, int override, int initialize, int check_required)
{
  int result;
  struct my_cmdline_parser_params params;
  
  params.override = override;
  params.initialize = initialize;
  params.check_required = check_required;
  params.check_ambiguity = 0;
  params.print_errors = 1;

  result = my_cmdline_parser_internal (argc, argv, args_info, &params, 0);

  if (result == EXIT_FAILURE)
    {
      my_cmdline_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

int
my_cmdline_parser_required (struct gengetopt_args_info *args_info, const char *prog_name)
{
  int result = EXIT_SUCCESS;

  if (my_cmdline_parser_required2(args_info, prog_name, 0) > 0)
    result = EXIT_FAILURE;

  if (result == EXIT_FAILURE)
    {
      my_cmdline_parser_free (args_info);
      exit (EXIT_FAILURE);
    }
  
  return result;
}

int
my_cmdline_parser_required2 (struct gengetopt_args_info *args_info, const char *prog_name, const char *additional_error)
{
  int error_occurred = 0;
  FIX_UNUSED (additional_error);

  /* checks for required options */
  if (! args_info->int_opt_given)
    {
      fprintf (stderr, "%s: '--int-opt' ('-i') option required%s\n", prog_name, (additional_error ? additional_error : ""));
      error_occurred = 1;
    }
  
  if (! args_info->long_opt_given)
    {
      fprintf (stderr, "%s: '--long-opt' ('-l') option required%s\n", prog_name, (additional_error ? additional_error : ""));
      error_occurred = 1;
    }
  
  
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
 * @param check_ambiguity @see my_cmdline_parser_params.check_ambiguity
 * @param override @see my_cmdline_parser_params.override
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
               my_cmdline_parser_arg_type arg_type,
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

  FIX_UNUSED (default_value);
    
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
  case ARG_SHORT:
    if (val) *((short *)field) = (short)strtol (val, &stop_char, 0);
    break;
  case ARG_LONG:
    if (val) *((long *)field) = (long)strtol (val, &stop_char, 0);
    break;
  case ARG_FLOAT:
    if (val) *((float *)field) = (float)strtod (val, &stop_char);
    break;
  case ARG_DOUBLE:
    if (val) *((double *)field) = strtod (val, &stop_char);
    break;
  case ARG_LONGDOUBLE:
    if (val) *((long double *)field) = (long double)strtod (val, &stop_char);
    break;
  case ARG_LONGLONG:
#if defined(HAVE_LONG_LONG) || defined(HAVE_LONG_LONG_INT)
    if (val) *((long long int*)field) = (long long int) strtoll (val, &stop_char, 0);
#else
    if (val) *((long *)field) = (long)strtol (val, &stop_char, 0);
#endif
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
  case ARG_SHORT:
  case ARG_LONG:
  case ARG_FLOAT:
  case ARG_DOUBLE:
  case ARG_LONGDOUBLE:
  case ARG_LONGLONG:
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
my_cmdline_parser_internal (
  int argc, char **argv, struct gengetopt_args_info *args_info,
                        struct my_cmdline_parser_params *params, const char *additional_error)
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
    my_cmdline_parser_init (args_info);

  my_cmdline_parser_init (&local_args_info);

  optarg = 0;
  optind = 0;
  opterr = params->print_errors;
  optopt = '?';

  while (1)
    {
      int option_index = 0;

      static struct option long_options[] = {
        { "help",	0, NULL, 'h' },
        { "full-help",	0, NULL, 0 },
        { "version",	0, NULL, 'V' },
        { "str-opt",	1, NULL, 's' },
        { "int-opt",	1, NULL, 'i' },
        { "short-opt",	1, NULL, 'S' },
        { "long-opt",	1, NULL, 'l' },
        { "float-opt",	1, NULL, 'f' },
        { "double-opt",	1, NULL, 'd' },
        { "long-double-opt",	1, NULL, 'L' },
        { "long-long-opt",	1, NULL, 'y' },
        { "func-opt",	0, NULL, 'F' },
        { "hidden-opt",	0, NULL, 'N' },
        { "flag-opt",	0, NULL, 'x' },
        { 0,  0, 0, 0 }
      };

      c = getopt_long (argc, argv, "hVs:i:S:l:f:d:L:y:FNx", long_options, &option_index);

      if (c == -1) break;	/* Exit from `while (1)' loop.  */

      switch (c)
        {
        case 'h':	/* Print help and exit.  */
        
        
          if (update_arg( 0 , 
               0 , &(args_info->help_given),
              &(local_args_info.help_given), optarg, 0, 0, ARG_NO,
              check_ambiguity, override, 0, 0,
              "help", 'h',
              additional_error))
            goto failure;
          my_cmdline_parser_free (&local_args_info);
          return 0;
        
          break;
        case 'V':	/* Print version and exit.  */
        
        
          if (update_arg( 0 , 
               0 , &(args_info->version_given),
              &(local_args_info.version_given), optarg, 0, 0, ARG_NO,
              check_ambiguity, override, 0, 0,
              "version", 'V',
              additional_error))
            goto failure;
          my_cmdline_parser_free (&local_args_info);
          return 0;
        
          break;
        case 's':	/* A string option.  */
        
        
          if (update_arg( (void *)&(args_info->str_opt_arg), 
               &(args_info->str_opt_orig), &(args_info->str_opt_given),
              &(local_args_info.str_opt_given), optarg, 0, 0, ARG_STRING,
              check_ambiguity, override, 0, 0,
              "str-opt", 's',
              additional_error))
            goto failure;
        
          break;
        case 'i':	/* A int option.  */
        
        
          if (update_arg( (void *)&(args_info->int_opt_arg), 
               &(args_info->int_opt_orig), &(args_info->int_opt_given),
              &(local_args_info.int_opt_given), optarg, 0, 0, ARG_INT,
              check_ambiguity, override, 0, 0,
              "int-opt", 'i',
              additional_error))
            goto failure;
        
          break;
        case 'S':	/* A short option.  */
        
        
          if (update_arg( (void *)&(args_info->short_opt_arg), 
               &(args_info->short_opt_orig), &(args_info->short_opt_given),
              &(local_args_info.short_opt_given), optarg, 0, 0, ARG_SHORT,
              check_ambiguity, override, 0, 0,
              "short-opt", 'S',
              additional_error))
            goto failure;
        
          break;
        case 'l':	/* A long option.  */
        
        
          if (update_arg( (void *)&(args_info->long_opt_arg), 
               &(args_info->long_opt_orig), &(args_info->long_opt_given),
              &(local_args_info.long_opt_given), optarg, 0, 0, ARG_LONG,
              check_ambiguity, override, 0, 0,
              "long-opt", 'l',
              additional_error))
            goto failure;
        
          break;
        case 'f':	/* A float option.  */
        
        
          if (update_arg( (void *)&(args_info->float_opt_arg), 
               &(args_info->float_opt_orig), &(args_info->float_opt_given),
              &(local_args_info.float_opt_given), optarg, 0, 0, ARG_FLOAT,
              check_ambiguity, override, 0, 0,
              "float-opt", 'f',
              additional_error))
            goto failure;
        
          break;
        case 'd':	/* A double option.  */
        
        
          if (update_arg( (void *)&(args_info->double_opt_arg), 
               &(args_info->double_opt_orig), &(args_info->double_opt_given),
              &(local_args_info.double_opt_given), optarg, 0, 0, ARG_DOUBLE,
              check_ambiguity, override, 0, 0,
              "double-opt", 'd',
              additional_error))
            goto failure;
        
          break;
        case 'L':	/* A long double option.  */
        
        
          if (update_arg( (void *)&(args_info->long_double_opt_arg), 
               &(args_info->long_double_opt_orig), &(args_info->long_double_opt_given),
              &(local_args_info.long_double_opt_given), optarg, 0, 0, ARG_LONGDOUBLE,
              check_ambiguity, override, 0, 0,
              "long-double-opt", 'L',
              additional_error))
            goto failure;
        
          break;
        case 'y':	/* A long long option.  */
        
        
          if (update_arg( (void *)&(args_info->long_long_opt_arg), 
               &(args_info->long_long_opt_orig), &(args_info->long_long_opt_given),
              &(local_args_info.long_long_opt_given), optarg, 0, 0, ARG_LONGLONG,
              check_ambiguity, override, 0, 0,
              "long-long-opt", 'y',
              additional_error))
            goto failure;
        
          break;
        case 'F':	/* A function option.  */
        
        
          if (update_arg( 0 , 
               0 , &(args_info->func_opt_given),
              &(local_args_info.func_opt_given), optarg, 0, 0, ARG_NO,
              check_ambiguity, override, 0, 0,
              "func-opt", 'F',
              additional_error))
            goto failure;
        
          break;
        case 'N':	/* A hidden option.  */
        
        
          if (update_arg( 0 , 
               0 , &(args_info->hidden_opt_given),
              &(local_args_info.hidden_opt_given), optarg, 0, 0, ARG_NO,
              check_ambiguity, override, 0, 0,
              "hidden-opt", 'N',
              additional_error))
            goto failure;
        
          break;
        case 'x':	/* A flag option.  */
        
        
          if (update_arg((void *)&(args_info->flag_opt_flag), 0, &(args_info->flag_opt_given),
              &(local_args_info.flag_opt_given), optarg, 0, 0, ARG_FLAG,
              check_ambiguity, override, 1, 0, "flag-opt", 'x',
              additional_error))
            goto failure;
        
          break;

        case 0:	/* Long option with no short option */
          /* Print help, including hidden options, and exit.  */
          if (strcmp (long_options[option_index].name, "full-help") == 0)
          {
          
          
            if (update_arg( 0 , 
                 0 , &(args_info->full_help_given),
                &(local_args_info.full_help_given), optarg, 0, 0, ARG_NO,
                check_ambiguity, override, 0, 0,
                "full-help", '-',
                additional_error))
              goto failure;
            my_cmdline_parser_free (&local_args_info);
            return 0;
          
          }
          
          break;
        case '?':	/* Invalid option.  */
          /* `getopt_long' already printed an error message.  */
          goto failure;

        default:	/* bug: option not considered.  */
          fprintf (stderr, "%s: option unknown: %c%s\n", MY_CMDLINE_PARSER_PACKAGE, c, (additional_error ? additional_error : ""));
          abort ();
        } /* switch */
    } /* while */



  if (check_required)
    {
      error_occurred += my_cmdline_parser_required2 (args_info, argv[0], additional_error);
    }

  my_cmdline_parser_release (&local_args_info);

  if ( error_occurred )
    return (EXIT_FAILURE);

  if (optind < argc)
    {
      int i = 0 ;
      int found_prog_name = 0;
      /* whether program name, i.e., argv[0], is in the remaining args
         (this may happen with some implementations of getopt,
          but surely not with the one included by gengetopt) */

      i = optind;
      while (i < argc)
        if (argv[i++] == argv[0]) {
          found_prog_name = 1;
          break;
        }
      i = 0;

      args_info->inputs_num = argc - optind - found_prog_name;
      args_info->inputs =
        (char **)(malloc ((args_info->inputs_num)*sizeof(char *))) ;
      while (optind < argc)
        if (argv[optind++] != argv[0])
          args_info->inputs[ i++ ] = gengetopt_strdup (argv[optind-1]) ;
    }

  return 0;

failure:
  
  my_cmdline_parser_release (&local_args_info);
  return (EXIT_FAILURE);
}
