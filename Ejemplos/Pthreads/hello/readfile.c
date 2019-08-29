#include <stdio.h>

int main(void)
{
	const char* filename = "hello.c";
	FILE* file=fopen(filename, "r");	
	if(!file)
		return (void)fprintf(stderr, "error: could not open. %s\n", filename), 1;
		
	int ch = 0;
	while ( (ch = fgetc(file)) != EOF ) // fgetc = file get char
		fputc(ch, stdout);
	fclose(file);
}
