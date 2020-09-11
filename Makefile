CC=cc
CFLAGS=-g
LIBS=-lpthread -L ./CommandParser -lcli
OBJS=gluethread/glthread.o	\
		utils.o				\
		net.o				\
		graph.o				\
		topologies.o		\
		comm.c				\
		nwcli.o				\
		Layer2/layer2.o		\
		Layer2/l2switch.o	\
		pkt_dump.o			\


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
comm.o:comm.c
	${CC} ${CFLAGS} -c -I . comm.c -o comm.o
Layer2/layer2.o:Layer2/layer2.c
	${CC} ${CFLAGS} -c -I Layer2 Layer2/l2switch.c -o Layer2/l2switch.o
Layer2/layer2.o:Layer2/layer2.c
	${CC} ${CFLAGS} -c -I Layer2 Layer2/layer2.c -o Layer2/layer2.o
pkt_dump.o:pkt_dump.c
	${CC} ${CFLAGS} -c -I . pkt_dump.c -o pkt_dump.o



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
	rm Layer2/layer2.o
	rm testApp 
	rm tags
	(cd CommandParser; make clean)

all:
	ctags -R --exclude=./course/* ./*
	make
	(cd CommandParser; make)
