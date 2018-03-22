all: test-panel

test-panel.o: test-panel.cpp
	g++ -c -Wall -I/usr/local/include test-panel.cpp -o test-panel.o

test-panel: test-panel.o
	g++ -Wall -L/usr/local/lib -lallegro -lallegro_main -lallegro_primitives test-panel.o -o test-panel

clean:
	rm -f *.o test-panel
