CPPFLAGS=-std=c++14 -c -g -Wall -I/usr/local/include
all: test-panel

udp-listener.o: udp-listener.cpp udp-listener.h
	g++ $(CPPFLAGS) udp-listener.cpp -o udp-listener.o

remote-grid.o: remote-grid.cpp grid.h remote-grid.h
	g++ $(CPPFLAGS) remote-grid.cpp -o remote-grid.o

on_air.o: on_air.cpp on_air.h grid.h
	g++ $(CPPFLAGS) on_air.cpp -o on_air.o

piles.o: piles.cpp piles.h grid.h
	g++ $(CPPFLAGS) piles.cpp -o piles.o

grid.o: grid.cpp grid.h
	g++ $(CPPFLAGS) grid.cpp -o grid.o

sim-panel.o: sim-panel.cpp grid.h
	g++ $(CPPFLAGS) sim-panel.cpp -o sim-panel.o

test-panel.o: test-panel.cpp *.h
	g++ $(CPPFLAGS) test-panel.cpp -o test-panel.o

test-panel: test-panel.o sim-panel.o grid.o piles.o on_air.o remote-grid.o udp-listener.o
	g++ -Wall -g -L/usr/local/lib -lallegro -lallegro_main -lallegro_primitives test-panel.o grid.o sim-panel.o piles.o on_air.o remote-grid.o udp-listener.o -o test-panel

clean:
	rm -f *.o test-panel
