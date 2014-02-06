.PHONY: clean

SRC = interpret.c eval.c tree.c stack.c var.c function.c operators.c
OBJS = $(SRC:%.c=%.o)

LIBS = -lm
CFLAGS = -O2 -g -Wall -Wextra -Wwrite-strings -Wstrict-prototypes 
CFLAGS += -Wunreachable-code -Wuninitialized 
#CFLAGS += -DDEBUG

EXE = interpret

all: $(OBJS) global.h Makefile
	@ gcc $(CFLAGS) $(LIBS) $(OBJS) -o $(EXE)
	@ echo '  LINK'

%.o:%.c
	@ gcc $(CFLAGS) -c $<
	@ echo '  CC ' $<

clean:
	@ rm $(OBJS) $(EXE)
	@ echo '  CLEAN'