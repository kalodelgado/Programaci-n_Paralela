#include "corr.h"

int main(int argc, char* argv[])
{
	corr_t corr;
	corr_init(&corr);
	int result = corr_run(&corr, argc, argv);
	corr_destroy(&corr);
	
	return result;
}
