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

	/* read game files */
	for (i = 1; i < argc; i++) {
		printf("TODO open and read %s\n", argv[i]);
	}

	/* initializing lua */
	L = lua_newstate(l_alloc, (void *)0); /* TODO check return */
	luaL_openlibs(L);

	/* using lua 'print()' */
	lua_getglobal(L, "print");
	lua_pushliteral(L, "Hello, Lua!");
	lua_call(L, 1, 0);

	#ifdef _WIN32
		puts("Hello, MacroHard Wangblows(tm)!");
	#elif __linux__
		puts("Hello, Linux_user!");
	#elif __APPLE__
		puts("Hello, Steve Jobs' cock!!");
	#else
		puts("Hello... god?");
	#endif

	/* cleanup and exit */
	lua_close(L);
	return EXIT_SUCCESS;
}
