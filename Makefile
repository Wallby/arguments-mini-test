arguments-mini-test: main.o
	make -C ../arguments-mini
	gcc -Wl,--wrap=malloc,--wrap=free -o arguments-mini-test.exe main.o -L../arguments-mini/ -larguments-mini

main.o: main.c
	gcc -c main.c -I../arguments-mini/

clean:
	if exist main.o del main.o
	if exist arguments-mini-test.exe del arguments-mini-test.exe