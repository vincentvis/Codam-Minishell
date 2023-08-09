
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

static int fd = -1;
static int first = 0;

void __attribute__((constructor,no_instrument_function)) setup_call_trace_file(void)
{
	fd = open("./source_files/debug/call_tree/CALL_TREE_JSON.json", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
	dprintf(fd, "[");
}

void __attribute__((destructor,no_instrument_function)) finish_call_trace_file(void)
{
	dprintf(fd, "]");
	close(fd);
}

/** According to gcc documentation: called upon function entry */
void __attribute__((no_instrument_function)) __cyg_profile_func_enter(void *this_fn, void *call_site)
{

	if (fd == -1)
		return ;
	if (first == 0)
	{
		dprintf(fd, "{\"action\": \"enter\", \"id\": \"%p\"}", this_fn);
		first++;
	}
	else
	{
		dprintf(fd, ",\n{\"action\": \"enter\", \"id\": \"%p\"}", this_fn);
	}
	(void)call_site;
}

/** According to gcc documentation: called upon function exit */
void __attribute__((no_instrument_function)) __cyg_profile_func_exit(void *this_fn, void *call_site)
{
	if (fd == -1)
		return ;
	if (first == 0)
	{
		dprintf(fd, "{\"action\": \"exit\", \"id\": \"%p\"}", this_fn);
		first++;
	}
	else
		dprintf(fd, ",\n{\"action\": \"exit\", \"id\": \"%p\"}", this_fn);
	(void)call_site;
}
