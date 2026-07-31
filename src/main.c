#include "codexion.h"
#include <stdlib.h>

int main(int argc, char **argv)
{
	if (argc != 9)
		exit(1);
	if (ft_isdigit(*argv[1]) == 0)
	{
		printf("ERROR: number_of_coders parameter must be a number.\n");
		exit(1);
	}
}
