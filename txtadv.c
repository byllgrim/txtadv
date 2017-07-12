#include <stdio.h>

int
main(void)
{
	#ifdef _WIN32
		puts("Hello, Jalamer!");
	#elif __linux__
		puts("Hello, Rolber!");
	#elif __APPLE__
		puts("Hello, Galori!");
	#else
		puts("Hello... Who are you?");
	#endif

	return 0;
}
