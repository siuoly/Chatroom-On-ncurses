
EXE = chatroom server
RUN = chatroom 
OBJ = windowFunc.o win.o socket.o \
			serverFunction.o

CFLAG= -std=c++11 -g -pedantic -Wall -Wextra -lncurses 

debug: CFALG+= -Wfatal-errors -w -DDEBUG 
debug: all

VPATH = include src 

all: ${OBJ} ${EXE} 

%.o: %.cc socket.h win.h
	c++ $< -I include ${CFLAG} -c

%: %.cc socket.h win.h
	c++ $< -I include ${OBJ} ${CFLAG} -o $@

run: all
	./${EXE}

clean:
	rm -f ${EXE} ${OBJ}

re: clean all
	
	
