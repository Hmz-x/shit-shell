/*
char * exec_echo (struct Param_types param_types);
void exec_exit (struct Param_types param_types);
*/

void cleanup_strs (char ** strs, int strs_cnt);
void msg_n_abort (const char * msg);

void
exec_exit (struct Param_types param_types)
{
	cleanup_strs (param_types.input_strs_mem, param_types.input_count_mem);
	cleanup_strs (param_types.option_strs_mem, param_types.option_count_mem);
	exit (0);
}

/*
struct Param_types
upper_input (struct Param_types param_types)
{
	for (int i = 1; i < param_types.input_count_mem; i++)
	{
		for (int j = 0; j < strlen (param_types.input_strs_mem[i]); j++)
		{
			param_types.input_strs_mem[i][j] = 
				toupper (param_types.input_strs_mem[i][j]);
		}
	}

	return param_types;
}

struct Param_types
lower_input (struct Param_types param_types)
{
	for (int i = 1; i < param_types.input_count_mem; i++)
	{
		for (int j = 0; j < strlen (param_types.input_strs_mem[i]); j++)
		{
			param_types.input_strs_mem[i][j] = 
				tolower (param_types.input_strs_mem[i][j]);
		}
	}

	return param_types;
}
struct Param_types
reverse_input (struct Param_types param_types)
{
	for (int i = 1; i < param_types.input_count_mem; i++)
	{
		for (int j = 0; j < strlen (param_types.input_strs_mem[i]); j++)
		{
			param_types.input_strs_mem[i][j] = 
				tolower (param_types.input_strs_mem[i][j]);
		}
	}

	return param_types;
}
*/



char * 
exec_echo (struct Param_types param_types)
{

	for (int i = 0; i < param_types.option_count_mem; i++)
	{
		/*
		// Apply uppercase
		if (strcmp (param_types.option_strs_mem[i], "-u") == 0)
			param_types = upper_input (param_types);
		// Apply lowercase
		else if (strcmp (param_types.option_strs_mem[i], "-l") == 0)
			param_types = lower_input (param_types);
		// Apply reverse order
		else if (strcmp (param_types.option_strs_mem[i], "-r") == 0)
			param_types = reverse_input (param_types);	
		*/
	}

	printf ("echo active.\n");
	return NULL;
}

char *
exec_asgn (struct Param_types param_types, struct Variables vars)
{
	char * output = NULL;
	const char * syntax_str = "syntax: asgn VARIABLE VALUE\n";
	const char * example_str = 
	"assignment example: asgn my_var 5\n"
	"increment example: asgn my_var +4\n"
	"decrement example: asgn my_var -20\n";
	
	// Show help string if '-h' is passed
	for (int i = 0; i < param_types.option_count_mem; i++)
	{
		if (strcmp (param_types.option_strs_mem[i], "-h") == 0)
		{
			output = malloc (strlen (syntax_str) + strlen (example_str) + 1);
			if (!output)
				msg_n_abort ("char * malloc failed.\n");

			output[0] = 0;
			strcat (output, syntax_str);
			strcat (output, example_str);

			return output;
		}
	}
	
	// Show syntax upon incorrect use	
	if (param_types.input_count_mem != 2)
	{
		output = malloc (strlen (syntax_str) + 1);
		if (!output)
			msg_n_abort ("char * malloc failed.\n");

		strcpy (output, syntax_str);
	}

	char * passed_var = param_types.input_strs_mem[1];
	int var_cnt = vars.var_count_mem;

	// Check if passed variable name is previously used
	int var_exists = -1;
	for (int i = 0; i < vars.var_count_mem; i++)
	{
		if (strcmp (passed_var, vars.var_names_mem[i]) == 0)
		{
			var_exists = i;
			break;	
		}	
	}

	if (var_exists == -1)
	{
	 	vars.var_names_mem[var_cnt] = malloc (strlen (passed_var) + 1);
		if (!vars.var_names_mem[var_cnt])
			msg_n_abort ("char * malloc failed.\n");

		// Assign passed variable name to var_names_mem[var_cnt]
		strcpy (vars.var_names_mem[var_cnt], passed_var);
		// 
		vars.var_count_mem++;
	}
	
	return output;
}
