
EXE = chatroom server
RUN = chatroom 

CFLAG= -std=c++11 -g -pedantic -Wall -Wextra -lncurses 

INCLUDE=-I ~/projects/c++/Tool/ 

debug: CFALG+= -Wfatal-errors -w -DDEBUG 
debug: all

run: all
	./${EXE}

all: ${EXE}
	
%: %.cc socket.h win.h
	c++ $< ${CFLAG} -o $@


clean:
	rm -f ${EXE}

re: clean all
	
	
