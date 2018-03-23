all: test-panel

on_air.o: on_air.cpp
	g++ -c -g -Wall -I/usr/local/include on_air.cpp -o on_air.o

piles.o: piles.cpp
	g++ -c -g -Wall -I/usr/local/include piles.cpp -o piles.o

grid.o: grid.cpp
	g++ -c -g -Wall -I/usr/local/include grid.cpp -o grid.o

sim-panel.o: sim-panel.cpp
	g++ -c -g -Wall -I/usr/local/include sim-panel.cpp -o sim-panel.o

test-panel.o: test-panel.cpp
	g++ -c -g -Wall -I/usr/local/include test-panel.cpp -o test-panel.o

test-panel: test-panel.o sim-panel.o grid.o piles.o on_air.o
	g++ -Wall -g -L/usr/local/lib -lallegro -lallegro_main -lallegro_primitives test-panel.o grid.o sim-panel.o piles.o on_air.o -o test-panel

clean:
	rm -f *.o test-panel
