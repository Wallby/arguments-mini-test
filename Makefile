arguments-mini-test: main.o
	make -C ../arguments-mini
	gcc -Wl,--wrap=malloc,--wrap=free -o arguments-mini-test main.o -L../arguments-mini -larguments-mini

main.o: main.c
	gcc -c main.c -I../arguments-mini/

clean:
	rm -f main.o
	rm -f arguments-mini-test

