#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "builtins.h"

#define BUFF_SIZE 500
#define INIT_STRS_NUM 100

const int builtin_count = 3;
const char * builtins_arr[] = {"exit", "echo", "asgn"};

struct Param_types
{
	char ** option_strs_mem;
	char ** input_strs_mem;
};

/* TODO
 * char ** Cleanup
 * implement realloc
 */

void
cleanup_strs_mem (char ** readline_strs, int readline_strs_cnt)
{
	for (int i = readline_strs_cnt-1; i > -1; i--)
	{
		free (readline_strs[i]);	
		readline_strs[i] = NULL;
	}	
}

void
msg_n_abort (const char * msg)
{
	fprintf (stderr, msg);
	abort ();
}

void
exec_input (char ** readline_strs, int readline_strs_cnt)
{
	int builtin_index = -1;	
	
	int i = 0;
	while (builtins_arr[i])	
	{
		if (strcmp(readline_strs[0], builtins_arr[i]) == 0)
		{
			builtin_index = i;
			break;
		}
		i++;
	}

	if (builtin_index == -1)
	{
		printf ("%s: builtin not found\n", readline_strs[0]);
	} else
	{
		switch (builtin_index)
		{
			case (0) :
				exec_exit (readline_strs, readline_strs_cnt);
				break;
			case (1) :
				exec_echo (readline_strs, readline_strs_cnt);	
				break;
		}	
	}
}

void
parse_readline_strs (char ** readline_strs, int readline_strs_cnt, char ** option_strs,
		char ** input_strs)
{
	// syntax: command -option1 -option2 -option3 input input
	
	int option_count = 0;
	for (int i = 1; i < readline_strs_cnt; i++)
	{
		if (readline_strs[i][0] == '-')
			option_count++;	
		else
			break;
	}
	
	option_strs = malloc (option_count * sizeof (char *));	
	if (!option_strs)
		msg_n_abort ("char ** malloc failed.\n");
	
	for (int i = 1, j = 0; i < option_count+1; i++, j++)
	{
		//option_strs[j] = malloc (strlen (readline_strs[i])+1);
		option_strs[j] = readline_strs[i];
	}

	input_strs = malloc ((readline_strs_cnt-option_count) * sizeof (char *));	
	if (!input_strs)
		msg_n_abort ("char ** malloc failed.\n");

	// Point the command name (aka the first element in readline_strs) to the first 
	// element in the input_strs
	input_strs[0] = readline_strs[0];
	for (int i = option_count+1, j = 1; i < readline_strs_cnt; i++, j++)
	{
		input_strs[j] = readline_strs[i];
	}

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
		if (!isspace(buffer[i]) && buffer[i] != '\0')
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
input_debug (char ** option_strs, char ** input_strs)
{

	printf ("command: %s\n", input_strs[0]);

	int i = 0;
	while (option_strs[i])
	{
		printf ("option: %s\n", option_strs[i++]);	
	}

	i = 0;
	while (input_strs[i])
	{
		printf ("input: %s\n", option_strs[i++]);	
	}
}

int
main (int argc, char ** argv)
{
	char ** readline_strs = malloc (INIT_STRS_NUM * sizeof (char *));
	if (!readline_strs)
		msg_n_abort ("char ** malloc failed.\n");

	char * buffer = NULL;
	struct Param_types param_types;
	int readline_strs_cnt = 0;
/*
	while (true)
	{
		printf ("$ ");

		buffer = get_input_line ();
		readline_strs_cnt = tokenize_input_buffer (readline_strs, buffer);
		if (readline_strs_cnt == 0)
			continue;

		parse_readline_strs (readline_strs, readline_strs_cnt, option_strs, input_strs);
		input_debug (option_strs, input_strs);
		exec_input (readline_strs, readline_strs_cnt);

		cleanup_strs_mem (readline_strs, readline_strs_cnt);
	}
*/
    return 0;
}
