void exec_echo (struct Param_types param_types);
void exec_exit (struct Param_types param_types);

void cleanup_strs (char ** strs, int strs_cnt);

void
exec_exit (struct Param_types param_types)
{
	cleanup_strs (param_types.input_strs_mem, param_types.input_count_mem);
	cleanup_strs (param_types.option_strs_mem, param_types.option_count_mem);
	exit (0);
}

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


void 
exec_echo (struct Param_types param_types)
{
	bool reverse_order = false;

	for (int i = 0; i < param_types.option_count_mem; i++)
	{
		// Apply uppercase
		if (strcmp (param_types.option_strs_mem[i], "-u") == 0)
			param_types = upper_input (param_types);
		else if (strcmp (param_types.option_strs_mem[i], "-l") == 0)
			param_types = lower_input (param_types);
		else if (strcmp (param_types.option_strs_mem[i], "-r") == 0)
			reverse_order = true;
	}

	printf ("echo active.\n");
}
