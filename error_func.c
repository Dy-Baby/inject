#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "error_func.h"

void err_msg(char *err_text, int err)
{
	fprintf(stderr, "[ERROR] [%s] : %s\n", err_text, strerror(err));
}
