#include <lua.h>
#include <lualib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
	NAMELEN = 256,
	MAXOBJ = 32,
	MAXFUNX = 32
};

struct object {
	char name[NAMELEN];
	char *objects[MAXOBJ];
	char *functions[MAXFUNX];
};

static void *
l_alloc(void *ud, void *ptr, size_t osize, size_t nsize)
{
	(void)ud;
	(void)osize;

	if (nsize == 0) {
		free(ptr);
		return (void *)0;
	} else {
		return realloc(ptr, nsize);
	}
}

static const char *
string_reader(lua_State *L,
              void *data,
              size_t *size)
{
	static int i = 1;
	(void)L;

	if (!i)
		return (void *)(*size = 0);
	else
		i = 0;

	*size = strlen(data); /* TODO don't be so naive */
	return data;
}

void
parse_objdata(FILE *file) /* here is where the obje parse */
{
	struct object obj;
	char line[BUFSIZ];
	char *ret;

	ret = fgets(line, sizeof(line), file);
	if (ret == NULL)
		return;
	strcpy(obj.name, line);
	printf("%s happen\n", obj.name);

	fgets(line, sizeof(line), file);
	if (strlen(line) == 2) {
		printf("This is a blank line\n");
	} else {
		printf("This isn't a blank line '%s'\n",line);
	}
}

static void
parse_file(char *name)
{
	FILE *fs;
	char buf[BUFSIZ];

	fs = fopen(name, "r"); /* TODO check return */
	parse_objdata(fs); /* TODO differentiate .obj and .func*/
	printf("--------\nfile %s contains:\n%s\n", name, buf);

	fclose(fs);
}

int
main(int argc, char *argv[])
{
	int i;
	struct lua_State *L;

	/* read game files */
	for (i = 1; i < argc; i++) {
		parse_file(argv[i]);
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

	/* cleanup and exit */
	lua_close(L);
	return EXIT_SUCCESS;
}
