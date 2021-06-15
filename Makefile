#!smake

include version.mk

CC ?= cc

RM ?= rm -f
LN ?= ln -s

INSTALL ?= install

LEMON ?= lemon

CCARGS ?= -fPIC
CFLAGS += $(CCARGS) -Iinclude

LDARGS ?= -lm -shared
LDFLAGS += $(LDARGS)

PREFIX ?= /usr/local

LIB_SUFFIX ?= .so
LIB_SO = libjson$(LIB_SUFFIX)

LIB_SO_MAJOR = $(LIB_SO).$(MAJOR)
LIB = $(LIB_SO).$(MAJOR).$(MINOR)

SRCS =\
	src/number.c \
	src/object.c \
	src/list.c \
	src/json.c \
	src/encode.c \
	src/decode.c

OBJS = ${SRCS:S/.c/.o/g}

$(LIB): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

src/decode.o: src/grammar.c

src/grammar.c: src/grammar.y
	$(LEMON) $<

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(LIB) $(LIB_SO) $(LIB_SO_MAJOR)
	$(RM) $(OBJS)
	$(RM) src/grammar.c src/grammar.h src/grammar.out
	$(RM) test

install: $(LIB)
	$(INSTALL) -d $(PREFIX)/include
	$(INSTALL) -m 644 include/json.h $(PREFIX)/include
	$(INSTALL) -d $(PREFIX)/lib
	$(INSTALL) -m 755 $(LIB) $(PREFIX)/lib
	$(LN) $(LIB) $(PREFIX)/lib/$(LIB_SO)
	$(LN) $(LIB) $(PREFIX)/lib/$(LIB_SO_MAJOR)

TEST_CFLAGS ?= $(CFLAGS)
TEST_LDFLAGS ?= -L. -Wl,-rpath,. -ljson

CMOCKA_PREFIX ?= /usr/pkg
CMOCKA_CFLAGS = -I$(CMOCKA_PREFIX)/include
CMOCKA_LDFLAGS = -L$(CMOCKA_PREFIX)/lib -Wl,-rpath,$(CMOCKA_PREFIX)/lib -lcmocka

test: $(LIB) src/test.c
	$(CC) $(CMOCKA_CFLAGS) $(TEST_CFLAGS) -o $@ src/test.c $(TEST_LDFLAGS) $(CMOCKA_LDFLAGS)
	./test
