#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[])
{
	if ( argc != 4 )
		return fprintf(stderr, "usage: generator rows cols x_percent\n");

	const long rows = atol(argv[1]);
	const long cols = atol(argv[2]);
	const long area = rows * cols;
	const double x_percent = atof(argv[3]) / 100.0;

	srand( time(NULL) );

	printf("%ld %ld\n", rows, cols);
	for ( long row = 0; row < rows; ++row )
	{
		for ( long col = 0; col < cols; ++col )
			putchar( rand() % area < x_percent * area ? 'x' : '.' );
		putchar('\n');
	}

	return 0;
}
