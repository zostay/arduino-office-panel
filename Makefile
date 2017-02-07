all: test-panel

test-panel.o: test-panel.c
	gcc -c -Wall -I/usr/local/include test-panel.c -o test-panel.o

test-panel: test-panel.o
	gcc -Wall -L/usr/local/lib -lallegro -lallegro_main -lallegro_primitives test-panel.o -o test-panel

clean:
	rm -f *.o test-panel
