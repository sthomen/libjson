#!smake

CC ?= cc

CCARGS ?= -fPIC
CFLAGS += $(CCARGS) -Iinclude

LDARGS ?= -shared
LDFLAGS += $(LDARGS)

RM ?= rm -f

# complicate this later
PROGRAM = libjson.so

SRCS =\
	src/json.c

OBJS = ${SRCS:S/.c/.o/g}

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(PROGRAM)
	$(RM) $(OBJS)

install:
	@echo "TODO: Install too"

tests:
	@echo "TODO: Add cmocka"
