#ifndef CORR_H
#define CORR_H

#include <regex.h> 

#include "args.h"

typedef struct
{
	args_t args;
	regex_t regex;
} corr_t;

void corr_init(corr_t* corr);
int corr_run(corr_t* corr, int argc, char* argv[]);
void corr_destroy(corr_t* corr);

#endif //CORR_H
