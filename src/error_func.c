#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "error_func.h"

void err_msg(char *file_name, char *func, int line, int err)
{
	fprintf(stderr, "[ERROR] [%s/%s/%d] : %s\n",
		file_name, func, line, strerror(err));
}
