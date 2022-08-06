void exec_echo (char ** input_strs, int input_strs_cnt);
void exec_exit (char ** input_strs, int input_strs_cnt);

void cleanup_strs_mem (char ** input_strs, int input_strs_cnt);


void
exec_exit (char ** input_strs, int input_strs_cnt)
{
	cleanup_strs_mem (input_strs, input_strs_cnt);
	exit (0);
}

void 
exec_echo (char ** input_strs, int input_strs_cnt)
{
	char * echo_options[] = {"-u", "-l", "-r"};
	printf ("echo active.\n");
}
