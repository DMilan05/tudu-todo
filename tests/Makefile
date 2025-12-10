CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I.. -I../include
PKG_GL = $(shell pkg-config --cflags --libs glib-2.0 gtk4)
LDFLAGS = -lcriterion $(PKG_GL)

# Ne építsük külön a stubs fájlt és a main tesztet
SOURCES := $(filter-out test_stubs.c test_main.c,$(wildcard *.c))
TARGETS := $(patsubst %.c,%,$(SOURCES))

STUBS = test_stubs.c
PROJECT_SRCS = ../src/extras.c ../src/io.c ../src/model.c ../src/search.c ../src/view.c
MAIN_OBJ = main.o

all: $(TARGETS)

# minden teszt cél linkeljen a stubbokkal és a projekthez tartozó forrásokkal
%: %.c $(STUBS) $(PROJECT_SRCS)
	$(CC) $(CFLAGS) -o $@ $< $(STUBS) $(PROJECT_SRCS) $(LDFLAGS)

# compile main.c with -Dstatic= so static functions become visible
$(MAIN_OBJ): ../src/main.c
	$(CC) $(CFLAGS) -Dstatic= $(PKG_GL) -c $< -o $@

# special rule for test_main to include the main object
test_main: test_main.c $(STUBS) $(PROJECT_SRCS) $(MAIN_OBJ)
	$(CC) $(CFLAGS) -o $@ $< $(STUBS) $(MAIN_OBJ) $(PROJECT_SRCS) $(LDFLAGS)

run: all
	./run_tests

clean:
	rm -f $(TARGETS)