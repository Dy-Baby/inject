#include <stdio.h>
#include <errno.h>
#include "error_func.h"

void err_msg(char *err_text, int err)
{
	fprintf(stderr, "[ERROR] [%s] : %s", err_text, strerror(err));
}
