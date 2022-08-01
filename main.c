#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "builtins.h"

#define BUFF_SIZE 500
#define INIT_STRS_NUM 100

const int builtin_count = 4;
const char * builtins_arr[] = {"echo", "reverse", "upper", "lower"};

/* TODO
 * char ** Cleanup
 * implement realloc
 */

void
msg_n_abort (const char * msg)
{
	fprintf (stderr, msg);
	abort ();
}

void
exec_input (char ** input_strs)
{
	int builtin_index = -1;	

	for (int i = 0; i < builtin_count; i++)
	{
		if (strcmp(input_strs[0], builtins_arr[i]) == 0)
		{
			builtin_index = i;
			break;
		}
	}

	if (builtin_index == -1)
	{
		printf ("%s: builtin not found\n", input_strs[0]);
	} else
	{
		switch (builtin_index)
		{
			case (0) :
				exec_echo (input_strs);	
				break;
		}	
	}
}

char *
write_to_input_strs (char * buffer, int buff_offset, int input_strlen)
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

char **
tokenize_input_buffer (char * buffer)
{
	int strs_num = INIT_STRS_NUM;
	int i = 0, input_strs_index = 0, input_strlen = 0;

	char ** input_strs = malloc (strs_num * sizeof (char *));
	if (!input_strs)
		msg_n_abort ("char ** malloc failed.\n");
	
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
				input_strs[input_strs_index++] = write_to_input_strs (buffer, i, input_strlen);
				// realloc here
			}

			input_strlen = 0;
		}
	} while (buffer[i++]);

	free (buffer);
	buffer = NULL;	

	return input_strs;
}

char *
get_input_line ()
{
	char ** input_strs = NULL;
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

int
main (int argc, char ** argv)
{
	char ** input_strs = NULL;
	char * buffer = NULL;

	while (true)
	{
		printf ("$ ");

		buffer = get_input_line ();
		input_strs = tokenize_input_buffer (buffer);
		exec_input (input_strs);
	}

    return 0;
}
