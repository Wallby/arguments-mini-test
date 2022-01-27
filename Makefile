LIBS=../arguments-mini/libarguments-mini.lib

# non configurable..
DIR_PER_LIB=$(dir $(LIBS))
NOTDIR_PER_LIB=$(patsubst lib%.lib,%,$(notdir $(LIBS)))

arguments-mini-test.exe: main.o $(LIBS)
	gcc -Wl,--wrap=malloc,--wrap=free -o arguments-mini-test.exe main.o $(addprefix -L,$(DIR_PER_LIB))/ $(addprefix -l,$(NOTDIR_PER_LIB))

# always make..
.PHONY: $(LIBS)
$(LIBS):
	make -C $(dir $@)

main.o: main.c
	gcc -c main.c -I../arguments-mini/

#******************************************************************************

andrun: arguments-mini-test.exe
	./arguments-mini-test.exe

clean:
	if exist main.o del main.o
	if exist arguments-mini-test.exe del arguments-mini-test.exe