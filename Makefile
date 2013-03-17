# http://paulmoore.mit-license.org

CC=gcc
CCFLAGS=-std=c99 -Ihiredis -Wall -Wextra -Wstrict-prototypes -Wwrite-strings
LDFLAGS=
HIREDIS=hiredis/libhiredis.a
EXEC=test
OBJECTS=test.o rdc_sem.o

all: .hiredis $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^ $(HIREDIS) $(LDFLAGS)

%.o: %.c
	$(CC) -c $< $(CCFLAGS)

.hiredis: .submodule
	cd hiredis && make

.submodule:
	git submodule update

clean:
	- $(RM) $(EXEC)
	- $(RM) $(OBJECTS)
	- cd hiredis && make clean
