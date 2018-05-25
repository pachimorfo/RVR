CC=g++
CFLAGS=-g -I. -I./SFML/include -L./SFML/lib
DEPS = Socket.h UDPServer.h Game.h 
OBJ = Socket.o UDPServer.o Game.o
LIBS=-lpthread -lsfml-graphics -lsfml-window -lsfml-system

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: gs gc

gs: $(OBJ) GameServer.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

gc: $(OBJ) GameClient.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f *.o gs gc

