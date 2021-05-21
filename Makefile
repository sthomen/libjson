#!smake

CC ?= cc

LEMON ?= lemon

CCARGS ?= -fPIC
CFLAGS += $(CCARGS) -Iinclude

LDARGS ?= -lm -shared
LDFLAGS += $(LDARGS)

RM ?= rm -f

# complicate this later
PROGRAM = libjson.so

SRCS =\
	src/number.c \
	src/object.c \
	src/list.c \
	src/json.c \
	src/encode.c \
	src/decode.c

OBJS = ${SRCS:S/.c/.o/g}

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

src/decode.o: src/grammar.c

src/grammar.c: src/grammar.y
	$(LEMON) $<

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(PROGRAM)
	$(RM) $(OBJS)
	$(RM) src/grammar.c src/grammar.h src/grammar.out
	$(RM) test

install:
	@echo "TODO: Install too"

TEST_CFLAGS ?= $(CFLAGS)
TEST_LDFLAGS ?= -L. -Wl,-rpath,. -ljson

CMOCKA_PREFIX ?= /usr/pkg
CMOCKA_CFLAGS = -I$(CMOCKA_PREFIX)/include
CMOCKA_LDFLAGS = -L$(CMOCKA_PREFIX)/lib -Wl,-rpath,$(CMOCKA_PREFIX)/lib -lcmocka

test: $(PROGRAM) src/test.c
	$(CC) $(CMOCKA_CFLAGS) $(TEST_CFLAGS) -o $@ src/test.c $(TEST_LDFLAGS) $(CMOCKA_LDFLAGS)
	./test
