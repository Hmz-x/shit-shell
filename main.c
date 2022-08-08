#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "param_types.h"
#include "variables.h"
#include "builtins.h"

#define BUFF_SIZE 500
#define INIT_STRS_NUM 100

const int builtin_count = 3;
const char * builtins_arr[] = {"exit", "echo", "asgn"};

/* TODO
 * char ** Cleanup
 * implement realloc
 */

void
cleanup_strs (char ** strs, int strs_cnt)
{
	for (int i = strs_cnt-1; i > -1; i--)
	{
		free (strs[i]);	
		strs[i] = NULL;
	}	
}

void
msg_n_abort (const char * msg)
{
	fprintf (stderr, msg);
	abort ();
}

void
exec_input (struct Param_types param_types, struct Variables vars)
{
	int builtin_index = -1;	
	char * output = NULL;
	
	int i = 0;
	while (builtins_arr[i])	
	{
		if (strcmp (param_types.input_strs_mem[0], builtins_arr[i]) == 0)
		{
			builtin_index = i;
			break;
		}
		i++;
	}

	if (builtin_index == -1)
	{
		printf ("%s: builtin not found\n", param_types.input_strs_mem[0]);
	} else
	{
		switch (builtin_index)
		{
			case (0) :
				exec_exit (param_types);
				break;
			case (1) :
				output = exec_echo (param_types);	
				break;
			case (2) :
				output = exec_asgn (param_types, vars);	
				break;
		}	

		printf ("output: %s", output);

		free (output);
		output = NULL;
	}
}

struct Param_types
parse_readline_strs (char ** readline_strs, int readline_strs_cnt, 
		struct Param_types param_types)
{
	// syntax: command -option1 -option2 -option3 input input

	// Get the number of options passed	
	int option_count = 0;
	for (int i = 1; i < readline_strs_cnt; i++)
	{
		if (readline_strs[i][0] == '-')
			option_count++;	
		else
			break;
	}
	
	param_types.option_strs_mem = malloc (option_count * sizeof (char *));	
	if (!param_types.option_strs_mem)
		msg_n_abort ("char ** malloc failed.\n");
	
	// Copy options from readline_strs to option_strs_mem
	for (int i = 1, j = 0; i < option_count+1; i++, j++)
	{
		param_types.option_strs_mem[j] = malloc (strlen (readline_strs[i]) + 1);
		if (!param_types.option_strs_mem[j])
			msg_n_abort ("char * malloc failed.\n");

		strcpy (param_types.option_strs_mem[j], readline_strs[i]);
	}
	param_types.option_count_mem = option_count;


	param_types.input_strs_mem = malloc ((readline_strs_cnt-option_count) * 
			sizeof (char *));	
	if (!param_types.input_strs_mem)
		msg_n_abort ("char ** malloc failed.\n");

	// Copy the command name (aka the first element in readline_strs) to the first 
	// element in param_types.input_strs_mem
	param_types.input_strs_mem[0] = malloc (strlen (readline_strs[0]) + 1);
	if (!param_types.input_strs_mem[0])
		msg_n_abort ("char * malloc failed.\n");

	strcpy (param_types.input_strs_mem[0], readline_strs[0]);

	// Copy input from readline_strs to input_strs_mem
	for (int i = option_count+1, j = 1; i < readline_strs_cnt; i++, j++)
	{
		param_types.input_strs_mem[j] = malloc (strlen (readline_strs[i]) + 1);
		if (!param_types.input_strs_mem[j])
			msg_n_abort ("char * malloc failed.\n");

		strcpy (param_types.input_strs_mem[j], readline_strs[i]);
	}
	param_types.input_count_mem = readline_strs_cnt - option_count;

	return param_types;
}

char *
write_to_readline_strs (char * buffer, int buff_offset, int input_strlen)
{
	char * output_str = malloc (input_strlen + 1);
	if (!output_str)
		msg_n_abort ("char * malloc failed.\n");
	int buffer_index = 0;

	for (int i = 0; i < input_strlen; i++)
	{
		buffer_index = buff_offset-input_strlen+i;
		output_str[i] = buffer[buffer_index];
	}
	output_str[input_strlen] = '\0';

	return output_str;
}

int
tokenize_input_buffer (char ** readline_strs, char * buffer)
{
	int strs_num = INIT_STRS_NUM;
	int i = 0, readline_strs_index = 0, input_strlen = 0;

	// Tokenize each string within the buffer aka remove whitespace	
	do 
	{
		if (!isspace (buffer[i]) && buffer[i] != '\0')
		{
			input_strlen++;	
		} else
		{
			if (input_strlen > 0)
			{
				readline_strs[readline_strs_index++] = write_to_readline_strs (buffer, i, input_strlen);
				// realloc here
			}

			input_strlen = 0;
		}
	} while (buffer[i++]);

	free (buffer);
	buffer = NULL;	

	return readline_strs_index;
}

char *
get_input_line ()
{
	char ** readline_strs = NULL;
	char * buffer = malloc (BUFF_SIZE);
	if (!buffer)
		msg_n_abort ("char * malloc failed.\n");
	buffer[0] = 0;

	fgets (buffer, BUFF_SIZE, stdin);
	// remove \n
	if (strlen(buffer) > 0)
		buffer[strlen(buffer)-1] = '\0';

	return buffer;
}

void
input_debug (struct Param_types param_types)
{

	printf ("option count: %d - input count: %d\n", param_types.option_count_mem, 
			param_types.input_count_mem);
	printf ("command: %s\n", param_types.input_strs_mem[0]);

	for (int i = 0; i < param_types.option_count_mem; i++)
	{
		printf ("option: %s\n", param_types.option_strs_mem[i]);
	}	

	for (int i = 1; i < param_types.input_count_mem; i++)
	{
		printf ("input: %s\n", param_types.input_strs_mem[i]);
	}
}

void
vars_debug (struct Variables vars)
{
	for (int i = 0; i < vars.var_count_mem; i++)
	{
		printf ("[%d] - name: %s - type: %s - value: %s\n", i, vars.var_names_mem[i],
				vars.var_types_mem[i], vars.var_values_mem[i]);
	}
}

int
main (int argc, char ** argv)
{
	char ** readline_strs = malloc (INIT_STRS_NUM * sizeof (char *));
	if (!readline_strs)
		msg_n_abort ("char ** malloc failed.\n");

	char * buffer = NULL;
	int readline_strs_cnt = 0;

	struct Param_types param_types;
	struct Variables vars;
	vars.var_count_mem = 0;
	vars.var_names_mem = malloc (INIT_STRS_NUM * sizeof (char *));
	if (!vars.var_names_mem)
		msg_n_abort ("char ** malloc failed.\n");
	vars.var_types_mem = malloc (INIT_STRS_NUM * sizeof (char *));
	if (!vars.var_types_mem)
		msg_n_abort ("char ** malloc failed.\n");
	vars.var_values_mem = malloc (INIT_STRS_NUM * sizeof (char *));
	if (!vars.var_values_mem)
		msg_n_abort ("char ** malloc failed.\n");



	while (true)
	{
		printf ("$ ");

		buffer = get_input_line ();
		readline_strs_cnt = tokenize_input_buffer (readline_strs, buffer);
		if (readline_strs_cnt == 0)
			continue;

		param_types = parse_readline_strs (readline_strs, readline_strs_cnt, param_types);
		//input_debug (param_types);
		exec_input (param_types, vars);
		vars_debug (vars);
		
		// Cleanup
		cleanup_strs (param_types.input_strs_mem, param_types.input_count_mem);
		param_types.input_count_mem = 0;
		cleanup_strs (param_types.option_strs_mem, param_types.option_count_mem);
		param_types.option_count_mem = 0;
		cleanup_strs (readline_strs, readline_strs_cnt);
	}

    return 0;
}
