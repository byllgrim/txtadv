#include <lua.h>
#include <lualib.h>
#include <stdio.h>
#include <stdlib.h>

static void *
l_alloc(void *ud, void *ptr, size_t osize, size_t nsize)
{
	(void)ud;  (void)osize;  /* not used */
	if (nsize == 0) {
		free(ptr);
		return NULL;
	} else {
		return realloc(ptr, nsize);
	}
}

int
main(int argc, char *argv[])
{
	int i;
	struct lua_State *L;

	for (i = 1; i < argc; i++) {
		printf("TODO open and read %s\n", argv[i]);
	}

	L = lua_newstate(l_alloc, (void *)0); /* TODO check return */
	luaL_openlibs(L);

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
