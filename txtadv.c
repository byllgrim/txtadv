#include <lua.h>
#include <lualib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

const char *
string_reader(lua_State *L,
              void *data,
              size_t *size)
{
	static int i = 1;

	if (!i)
		return (void *)(*size = 0);
	else
		i = 0;

	(void)L;
	*size = strlen(data); /* TODO don't be so naive */
	return data;
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

	/* create a new lua function */
	lua_load(L,
	         string_reader,
	         "print(\"Our own lua function!\")",
	         "hello_world",
	         "t");
	lua_call(L, 0, 0);

	/* using lua 'print()' */
	lua_getglobal(L, "print");
	lua_pushliteral(L, "Hello, Lua!");
	lua_call(L, 1, 0);

	#ifdef _WIN32
		puts("Hello, MacroHard Wangblows(tm)!");
	#elif __linux__
		puts("Hello, Linux_sux_user!");
	#elif __APPLE__
		puts("Hello, Steve Jobs' cock!!");
	#else
		puts("Hello... god?");
	#endif

	/* cleanup and exit */
	lua_close(L);
	return EXIT_SUCCESS;
}
