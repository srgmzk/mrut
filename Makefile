CC=cc
CFLAGS=-g
LIBS=-lpthread -L ./CommandParser -lcli
OBJS=gluethread/glthread.o	\
		utils.o				\
		net.o				\
		graph.o				\
		topologies.o		\
		nwcli.o				\


testApp:testapp.o ${OBJS} CommandParser/libcli.a
	${CC} ${CFLAGS} testapp.o ${OBJS} -o testApp ${LIBS} 
	ctags -R --exclude=./course/* ./*

testapp.o:testapp.c
	${CC} ${CFLAGS} -c testapp.c -o testapp.o

gluethread/glthread.o:gluethread/glthread.c
	${CC} ${CFLAGS} -c -I gluethread gluethread/glthread.c -o gluethread/glthread.o
utils.o:utils.c
	${CC} ${CFLAGS} -c -I . utils.c -o utils.o
net.o:net.c
	${CC} ${CFLAGS} -c -I . net.c -o net.o
graph.o:graph.c
	${CC} ${CFLAGS} -c -I . graph.c -o graph.o
topologies.o:topologies.c
	${CC} ${CFLAGS} -c -I . topologies.c -o topologies.o
CommandParser/libcli.a:
	(cd CommandParser; make)
nwcli.o:nwcli.c
	${CC} ${CFLAGS} -c -I . nwcli.c -o nwcli.o

tags:
	ctags -R --exclude=./course/* ./*

flow:
	
	
clean:
	rm *.o
	rm gluethread/glthread.o
	rm testApp 
	rm tags
	(cd CommandParser; make clean)

all:
	ctags -R --exclude=./course/* ./*
	make
	(cd CommandParser; make)
