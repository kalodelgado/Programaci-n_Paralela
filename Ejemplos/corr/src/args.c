#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include "args.h"

const char* const help =
	"usage: corr [-ein] regexp\n"
	"\n"
	"options:\n"
	"	-e extended\n"
	"	-i ignore case\n"
	"	-n new line chars separate strings\n"
	;

void args_init(args_t* args)
{
	args->extended = false;
	args->ignore_case = false;
	args->new_line = false;
	args->pattern = NULL;
}

int args_analyze(args_t* args, int argc, char* argv[])
{
	for( int index = 1; index < argc; ++index)
	{
		if( *argv[index] == '-' )
		{
			for ( const char* option = argv[index] + 1; *option; ++option)//argv[index] + 1 porque argv[index][0] es igual a -
				switch ( *option ) 
				{
					case 'e': args->extended = true; break;
					case 'i': args->ignore_case = true; break;
					case 'n': args->new_line = true; break;
					default: return fprintf(stderr, "error: invalid option: %s\n", argv[index]), EXIT_FAILURE;
				}
		}
		else
		{
			if(args->pattern == NULL)
				args->pattern = argv[index];
			else
				return fprintf(stderr, "error: multiple regular expressions: %s\n", argv[index]), EXIT_FAILURE;
		}
	}
	
	return 0;//SUCCESS
}

int args_print_help(void)
{
	printf("%s", help);
	return 0;
}

void args_destroy(args_t* args)
{
	
}

