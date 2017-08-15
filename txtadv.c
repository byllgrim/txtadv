#include <stdio.h>

int
main(void)
{
	#ifdef _WIN32
		puts("Hello, Jalamer!");
	#elif __linux__
		puts("Hello, Linux_user!");
	#elif __APPLE__
		puts("Hello, Galori!");
	#else
		puts("Hello... Who are you?");
	#endif

	return 0;
}
