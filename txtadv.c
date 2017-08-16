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

static void
object_set_name(struct object *obj, char *name)
{
	strncpy(obj->name, name, sizeof(obj->name));
	printf("'%s' happen\n", obj->name);
}

static void
remove_newlines(char *str)
{
	char *occurence;

	while ( (occurence = strpbrk(str, "\r\n")) != NULL)
		occurence[0] = '\0';
}

static void
parse_objdata(FILE *file) /* here is where the obje parse */
{
	struct object obj = {0};
	char line[BUFSIZ];

	while (fgets(line, sizeof(line), file) != NULL) {
		remove_newlines(line);

		if (obj.name[0] == 0) {
			object_set_name(&obj, line);
		} else if (line[0] == '\0') {
			printf("This is a blank line\n");
		} else {
			printf("This isn't a blank line '%s'\n",line);
		}
	}
}

static void
parse_file(char *name)
{
	FILE *fs;

	fs = fopen(name, "r"); /* TODO check return */
	parse_objdata(fs); /* TODO differentiate .obj and .func*/

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
