#include <stdio.h>

#include "corr.h"

#define LINE_CAPACITY 1024

int convert_cflags(const corr_t* corr);
void corr_search_file(corr_t* corr, FILE* input);

void corr_init(corr_t* corr)
{
	args_init( &corr->args );
}

int corr_run(corr_t* corr, int argc, char* argv[])
{
	int error = args_analyze( &corr->args, argc, argv );
	if ( error )
		return error;
	
	if( corr->args.pattern == NULL)
		return args_print_help();
		
	int cflags = convert_cflags(corr);
	if( regcomp(&corr->regex, corr->args.pattern, cflags) )
		return fprintf(stderr, "erro: invalid regular expression: %s\n", corr->args.pattern), 1;
	
	corr_search_file(corr, stdin);
	
	regfree(&corr->regex);
	
	return 0;
}

void corr_destroy(corr_t* corr)
{
	args_destroy(&corr->args);
}

int convert_cflags(const corr_t* corr)
{
	int cflags = REG_NOSUB;
	
	if(corr->args.extended)
		cflags |= REG_EXTENDED;
	
	if(corr->args.ignore_case)
		cflags |= REG_ICASE;
	
	if(corr->args.new_line)
		cflags |= REG_NEWLINE;
	
	return cflags;
}

void corr_search_file(corr_t* corr, FILE* input)
{
	int eflags = 0;
	char line[LINE_CAPACITY];	;
	//regmatch_t matches[32];
	
	while ( fgets(line, LINE_CAPACITY, input) )
		if ( regexec(&corr->regex, line, /*32*/ 0, /*matches*/ NULL, eflags) == 0)
			printf("%s", line);
}
