#include <stdio.h>
#include "mysh.h"
#include "parse.h"
#include "execute.h"


void execute(struct parsed_line *p, char **path, int pathsize)
{
    if (p->inputfile)
	printf("redirecting input from %s\n", p->inputfile);
    if (p->outputfile)
	printf("redirecting output to %s\n", p->outputfile);
    if (p->argv)
	printf("executing %s...\n", p->argv[0]);
}
