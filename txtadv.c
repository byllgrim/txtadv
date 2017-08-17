#include <lua.h>
#include <lualib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
	NAMELEN = 256,
	MAXCONTENT = 32, /* TODO rename */
	MAXOBJ = 128
};

struct object {
	char name[NAMELEN];
	char *objects[MAXCONTENT];
	char *functions[MAXCONTENT];
};

struct obj_list {
	struct object objs[MAXOBJ]; /* TODO make dynamic? */
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
string_reader(lua_State *L, void *data, size_t *size)
{
	static int i = 1;
	(void)L;

	if (!i) {
		return (void *)(*size = 0);
	} else {
		i = 0;
	}

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

	while ( (occurence = strpbrk(str, "\r\n")) != NULL) {
		occurence[0] = '\0';
	}
}

static void
parse_objdata(struct obj_list *ol, FILE *file)
{
	int i;
	struct object *objs = ol->objs;
	char line[BUFSIZ];

	/* here is where the obje parse */
	for (i = 0; fgets(line, sizeof(line), file) != NULL; ) {
		/* TODO index bounds */

		remove_newlines(line);

		if (objs[i].name[0] == 0) {
			object_set_name(&objs[i], line);
		} else if (line[0] == 0) {
			i++;
			printf("New object\n");
		} else {
			printf("TODO add content '%s'\n",line);
		}
	}
}

static void
parse_file(struct obj_list *ol, char *name)
{
	FILE *file;

	file = fopen(name, "r"); /* TODO check return */
	parse_objdata(ol, file); /* TODO differentiate .obj and .func */

	fclose(file);
}

int
main(int argc, char *argv[])
{
	int i;
	struct obj_list ol = {0};
	struct lua_State *L;

	/* read game files */
	for (i = 1; i < argc; i++) {
		parse_file(&ol, argv[i]);
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
