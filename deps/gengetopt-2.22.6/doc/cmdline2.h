/** @file cmdline2.h
 *  @brief The header file for the command line option parser
 *  generated by GNU Gengetopt version 2.22.6
 *  http://www.gnu.org/software/gengetopt.
 *  DO NOT modify this file, since it can be overwritten
 *  @author GNU Gengetopt by Lorenzo Bettini */

#ifndef CMDLINE2_H
#define CMDLINE2_H

/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h> /* for FILE */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef MY_CMDLINE_PARSER_PACKAGE
/** @brief the program name (used for printing errors) */
#define MY_CMDLINE_PARSER_PACKAGE PACKAGE
#endif

#ifndef MY_CMDLINE_PARSER_PACKAGE_NAME
/** @brief the complete program name (used for help and version) */
#ifdef PACKAGE_NAME
#define MY_CMDLINE_PARSER_PACKAGE_NAME PACKAGE_NAME
#else
#define MY_CMDLINE_PARSER_PACKAGE_NAME PACKAGE
#endif
#endif

#ifndef MY_CMDLINE_PARSER_VERSION
/** @brief the program version */
#define MY_CMDLINE_PARSER_VERSION VERSION
#endif

/** @brief Where the command line options are stored */
struct gengetopt_args_info
{
  const char *help_help; /**< @brief Print help and exit help description.  */
  const char *full_help_help; /**< @brief Print help, including hidden options, and exit help description.  */
  const char *version_help; /**< @brief Print version and exit help description.  */
  char * str_opt_arg;	/**< @brief A string option.  */
  char * str_opt_orig;	/**< @brief A string option original value given at command line.  */
  const char *str_opt_help; /**< @brief A string option help description.  */
  int int_opt_arg;	/**< @brief A int option.  */
  char * int_opt_orig;	/**< @brief A int option original value given at command line.  */
  const char *int_opt_help; /**< @brief A int option help description.  */
  short short_opt_arg;	/**< @brief A short option.  */
  char * short_opt_orig;	/**< @brief A short option original value given at command line.  */
  const char *short_opt_help; /**< @brief A short option help description.  */
  long long_opt_arg;	/**< @brief A long option.  */
  char * long_opt_orig;	/**< @brief A long option original value given at command line.  */
  const char *long_opt_help; /**< @brief A long option help description.  */
  float float_opt_arg;	/**< @brief A float option.  */
  char * float_opt_orig;	/**< @brief A float option original value given at command line.  */
  const char *float_opt_help; /**< @brief A float option help description.  */
  double double_opt_arg;	/**< @brief A double option.  */
  char * double_opt_orig;	/**< @brief A double option original value given at command line.  */
  const char *double_opt_help; /**< @brief A double option help description.  */
  long double long_double_opt_arg;	/**< @brief A long double option.  */
  char * long_double_opt_orig;	/**< @brief A long double option original value given at command line.  */
  const char *long_double_opt_help; /**< @brief A long double option help description.  */
  #if defined(HAVE_LONG_LONG) || defined(HAVE_LONG_LONG_INT)
  long long int long_long_opt_arg;	/**< @brief A long long option.  */
  #else
  long long_long_opt_arg;	/**< @brief A long long option.  */
  #endif
  char * long_long_opt_orig;	/**< @brief A long long option original value given at command line.  */
  const char *long_long_opt_help; /**< @brief A long long option help description.  */
  const char *func_opt_help; /**< @brief A function option help description.  */
  const char *hidden_opt_help; /**< @brief A hidden option help description.  */
  int flag_opt_flag;	/**< @brief A flag option (default=off).  */
  const char *flag_opt_help; /**< @brief A flag option help description.  */
  
  unsigned int help_given ;	/**< @brief Whether help was given.  */
  unsigned int full_help_given ;	/**< @brief Whether full-help was given.  */
  unsigned int version_given ;	/**< @brief Whether version was given.  */
  unsigned int str_opt_given ;	/**< @brief Whether str-opt was given.  */
  unsigned int int_opt_given ;	/**< @brief Whether int-opt was given.  */
  unsigned int short_opt_given ;	/**< @brief Whether short-opt was given.  */
  unsigned int long_opt_given ;	/**< @brief Whether long-opt was given.  */
  unsigned int float_opt_given ;	/**< @brief Whether float-opt was given.  */
  unsigned int double_opt_given ;	/**< @brief Whether double-opt was given.  */
  unsigned int long_double_opt_given ;	/**< @brief Whether long-double-opt was given.  */
  unsigned int long_long_opt_given ;	/**< @brief Whether long-long-opt was given.  */
  unsigned int func_opt_given ;	/**< @brief Whether func-opt was given.  */
  unsigned int hidden_opt_given ;	/**< @brief Whether hidden-opt was given.  */
  unsigned int flag_opt_given ;	/**< @brief Whether flag-opt was given.  */

  char **inputs ; /**< @brief unamed options (options without names) */
  unsigned inputs_num ; /**< @brief unamed options number */
} ;

/** @brief The additional parameters to pass to parser functions */
struct my_cmdline_parser_params
{
  int override; /**< @brief whether to override possibly already present options (default 0) */
  int initialize; /**< @brief whether to initialize the option structure gengetopt_args_info (default 1) */
  int check_required; /**< @brief whether to check that all required options were provided (default 1) */
  int check_ambiguity; /**< @brief whether to check for options already specified in the option structure gengetopt_args_info (default 0) */
  int print_errors; /**< @brief whether getopt_long should print an error message for a bad option (default 1) */
} ;

/** @brief the purpose string of the program */
extern const char *gengetopt_args_info_purpose;
/** @brief the usage string of the program */
extern const char *gengetopt_args_info_usage;
/** @brief the description string of the program */
extern const char *gengetopt_args_info_description;
/** @brief all the lines making the help output */
extern const char *gengetopt_args_info_help[];
/** @brief all the lines making the full help output (including hidden options) */
extern const char *gengetopt_args_info_full_help[];

/**
 * The command line parser
 * @param argc the number of command line options
 * @param argv the command line options
 * @param args_info the structure where option information will be stored
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int my_cmdline_parser (int argc, char **argv,
  struct gengetopt_args_info *args_info);

/**
 * The command line parser (version with additional parameters - deprecated)
 * @param argc the number of command line options
 * @param argv the command line options
 * @param args_info the structure where option information will be stored
 * @param override whether to override possibly already present options
 * @param initialize whether to initialize the option structure my_args_info
 * @param check_required whether to check that all required options were provided
 * @return 0 if everything went fine, NON 0 if an error took place
 * @deprecated use my_cmdline_parser_ext() instead
 */
int my_cmdline_parser2 (int argc, char **argv,
  struct gengetopt_args_info *args_info,
  int override, int initialize, int check_required);

/**
 * The command line parser (version with additional parameters)
 * @param argc the number of command line options
 * @param argv the command line options
 * @param args_info the structure where option information will be stored
 * @param params additional parameters for the parser
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int my_cmdline_parser_ext (int argc, char **argv,
  struct gengetopt_args_info *args_info,
  struct my_cmdline_parser_params *params);

/**
 * Save the contents of the option struct into an already open FILE stream.
 * @param outfile the stream where to dump options
 * @param args_info the option struct to dump
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int my_cmdline_parser_dump(FILE *outfile,
  struct gengetopt_args_info *args_info);

/**
 * Save the contents of the option struct into a (text) file.
 * This file can be read by the config file parser (if generated by gengetopt)
 * @param filename the file where to save
 * @param args_info the option struct to save
 * @return 0 if everything went fine, NON 0 if an error took place
 */
int my_cmdline_parser_file_save(const char *filename,
  struct gengetopt_args_info *args_info);

/**
 * Print the help
 */
void my_cmdline_parser_print_help(void);
/**
 * Print the full help (including hidden options)
 */
void my_cmdline_parser_print_full_help(void);
/**
 * Print the version
 */
void my_cmdline_parser_print_version(void);

/**
 * Initializes all the fields a my_cmdline_parser_params structure 
 * to their default values
 * @param params the structure to initialize
 */
void my_cmdline_parser_params_init(struct my_cmdline_parser_params *params);

/**
 * Allocates dynamically a my_cmdline_parser_params structure and initializes
 * all its fields to their default values
 * @return the created and initialized my_cmdline_parser_params structure
 */
struct my_cmdline_parser_params *my_cmdline_parser_params_create(void);

/**
 * Initializes the passed gengetopt_args_info structure's fields
 * (also set default values for options that have a default)
 * @param args_info the structure to initialize
 */
void my_cmdline_parser_init (struct gengetopt_args_info *args_info);
/**
 * Deallocates the string fields of the gengetopt_args_info structure
 * (but does not deallocate the structure itself)
 * @param args_info the structure to deallocate
 */
void my_cmdline_parser_free (struct gengetopt_args_info *args_info);

/**
 * Checks that all the required options were specified
 * @param args_info the structure to check
 * @param prog_name the name of the program that will be used to print
 *   possible errors
 * @return
 */
int my_cmdline_parser_required (struct gengetopt_args_info *args_info,
  const char *prog_name);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CMDLINE2_H */
