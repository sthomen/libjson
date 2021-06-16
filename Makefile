#!smake

include version.mk

CC ?= cc

RM ?= rm -f
LN ?= ln -sf

INSTALL ?= install

LEMON ?= lemon

CCARGS ?= -fPIC
CFLAGS += $(CCARGS) -I.

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
	$(RM) $(LIB)
	$(RM) $(OBJS)
	$(RM) src/grammar.c src/grammar.h src/grammar.out
	$(RM) test $(LIB_SO)

install: $(LIB)
	$(INSTALL) -d $(PREFIX)/include
	$(INSTALL) -m 644 json.h $(PREFIX)/include
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
	$(LN) $(LIB) $(LIB_SO)
	$(CC) $(CMOCKA_CFLAGS) $(TEST_CFLAGS) -o $@ src/test.c $(TEST_LDFLAGS) $(CMOCKA_LDFLAGS)
	./test
