LIBS=../arguments-mini/libarguments-mini.a

# non configurable..
DIR_PER_LIB=$(dir $(LIBS))
NOTDIR_PER_LIB=$(patsubst lib%.a,%,$(notdir $(LIBS)))

arguments-mini-test: main.o $(LIBS)
	gcc -Wl,--wrap=malloc,--wrap=free -o arguments-mini-test main.o $(addprefix -L,$(DIR_PER_LIB)) $(addprefix -l,$(NOTDIR_PER_LIB))

# always make..
.PHONY: $(LIBS)
$(LIBS):
	make -C $(dir $@)

main.o: main.c
	gcc -c main.c $(addprefix -I,$(DIR_PER_LIB))

#******************************************************************************

andrun: arguments-mini-test
	./arguments-mini-test

clean:
	rm -f main.o
	rm -f arguments-mini-test

