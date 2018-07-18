all: x_language

OBJS = main.o

LLVMCONFIG = llvm-config
CPPFLAGS = `$(LLVMCONFIG) --cppflags` -std=c++11
LDFLAGS = `$(LLVMCONFIG) --ldflags` -lpthread -ldl -lz -lncurses -rdynamic
LIBS = `$(LLVMCONFIG) --libs`

CC=g++-7
clean :
	rm -rf parser.cc parser.h parser tokens.cc $(OBJS)

parser.cc : toylang.y
	bison -d -o $@ $^

parser.h : parser.cc

tokens.cc : toylang.l parser.h
	flex  -o $@ $^

parser.o:parser.cc
	$(CC) -c $(CPPFLAGS) -o $@ $<  
main.o:main.cc 
	$(CC) -c $(CPPFLAGS) -o $@ $<

x_language : 
	$(CC) -o $@  $(OBJS) $(LIBS) $(LDFLAGS)

