#!smake

CC ?= cc

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
	src/json.c

OBJS = ${SRCS:S/.c/.o/g}

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(PROGRAM)
	$(RM) $(OBJS)
	$(RM) test

install:
	@echo "TODO: Install too"

TEST_CFLAGS ?= $(CFLAGS)
TEST_LDFLAGS ?= -L. -Wl,-rpath,. -ljson

CMOCKA_CFLAGS ?= -I/usr/pkg/include
CMOCKA_LDFLAGS ?= -L/usr/pkg/lib -Wl,-rpath,/usr/pkg/lib -lcmocka

test: $(PROGRAM) src/test.c
	$(CC) $(CMOCKA_CFLAGS) $(TEST_CFLAGS) -o $@ src/test.c $(TEST_LDFLAGS) $(CMOCKA_LDFLAGS)
	./test
