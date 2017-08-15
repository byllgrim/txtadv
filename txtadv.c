#include <stdio.h>

int
main(int argc, char *argv[])
{
	int i;

	for (i = 1; i < argc; i++) {
		printf("TODO open and read %s\n", argv[i]);
	}

	#ifdef _WIN32
		puts("Hello, MacroHard Wangblows(tm)!");
	#elif __linux__
		puts("Hello, Linux_user!");
	#elif __APPLE__
		puts("Hello, Steve Jobs' cock!!");
	#else
		puts("Hello... god?");
	#endif

	return 0;
}
