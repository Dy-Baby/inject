#include <stdlib.h>
#include "random.h"

unsigned int rand_addr();

unsigned short rand_port()
{
	return rand() % 65535;
}
