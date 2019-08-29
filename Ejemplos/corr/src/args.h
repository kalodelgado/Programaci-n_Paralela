#ifndef ARGS_H
#define ARGS_H

#include <stdbool.h>

typedef struct
{
	bool extended;
	bool ignore_case;
	bool new_line;
	const char* pattern;
	
} args_t;

void args_init(args_t* args);
int args_analyze(args_t* args, int argc, char* argv[]);
int args_print_help(void);
void args_destroy(args_t* args);

#endif //ARGS_H
