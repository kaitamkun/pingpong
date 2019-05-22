COMPILER		 = clang++
CFLAGS			:= -std=c++17 -g -O3 -Wall -Wextra -fdiagnostics-color=always
LDFLAGS			:=
CC				 = $(COMPILER) $(CFLAGS) $(CHECKFLAGS)
CHECKFLAGS		:=
MKBUILD			:= mkdir -p build

CHECK			:= asan

ifeq ($(CHECK), asan)
	CHECKFLAGS += -fsanitize=address -fno-common
else ifeq ($(CHECK), msan)
	CHECKFLAGS += -fsanitize=memory -fno-common
endif

.PHONY: all test clean depend mkbuild
all: Makefile
	@echo $(COMMONOBJ)

# Peter Miller, "Recursive Make Considered Harmful" (http://aegis.sourceforge.net/auug97.pdf)
MODULES			:= src/core src/commands src/messages src/lib src/test
COMMONSRC		:=
CFLAGS			+= -Iinclude -I../include -I.
LIBS			:=
SRC				:=
include $(patsubst %,%/module.mk,$(MODULES))
SRC				+= $(COMMONSRC)
COMMONOBJ		:= $(patsubst src/%.cpp,build/%.o, $(filter %.cpp,$(COMMONSRC)))
OBJ				:= $(patsubst src/%.cpp,build/%.o, $(filter %.cpp,$(SRC)))
sinclude $(patsubst %,%/targets.mk,$(MODULES))

include conan.mk

all: $(COMMONOBJ)

test: build/tests
	./build/tests

grind: build/tests
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --show-reachable=yes ./build/tests

clean:
	rm -rf build
	rm -rf *.o **/*.o

build/%.o: src/%.cpp
	@ mkdir -p "$(shell dirname "$@")"
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

DEPFILE  = .dep
DEPTOKEN = "\# MAKEDEPENDS"
DEPFLAGS = -f $(DEPFILE) -s $(DEPTOKEN)

depend:
	@ echo $(DEPTOKEN) > $(DEPFILE)
	makedepend $(DEPFLAGS) -- $(CC) -- $(SRC)
	@ rm $(DEPFILE).bak

sinclude $(DEPFILE)
