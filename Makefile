TARG = txtadv
SRC = ${TARG}.c
OBJ = ${SRC:.c=.o}
CPPFLAGS = -DLUA_USE_C89
CFLAGS = -Os -pedantic -std=c89 -Wall -Wextra ${CPPFLAGS}
LIB = -L/usr/local/lib
LDFLAGS  = ${LIB} -llua -lm -s -static

${TARG}: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

.c.o:
	${CC} -c ${CFLAGS} $<

clean:
	rm -f ${TARG} ${OBJ}
