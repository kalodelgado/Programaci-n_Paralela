#include <stdio.h>

int main(void)
{
	size_t result = 1ull << 46;
	//unsigned long long result = 1ull << 48; %llu
	//printf("Result = %d\n", result); // No cabe 2^48 en un d
	printf("Result = %zx\n", result);
	return 0;
}
