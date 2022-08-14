#include <stdlib.h>
#include "random.h"

/* a very dumb way to generate random ip address.
 * i'm going to find a better way to do this */
unsigned int rand_addr()
{
	return rand();
}

unsigned short rand_port()
{
	return rand() % 65535;
}
