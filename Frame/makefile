EXE = main

CFLAG= -std=c++11 -g -pedantic -Wall -Wextra -lncurses 


debug: CFALG+= -Wfatal-errors -w -DDEBUG 
debug: all

all: ${EXE}
	
main: windowTemplate.cc win.h
	c++ $< ${CFLAG} -o $@

run: all
	./${EXE}

clean:
	rm -f ${EXE}

re: clean all
	
	
