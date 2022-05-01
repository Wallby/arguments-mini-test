ifndef OS # linux
EXECUTABLE_EXTENSION=
LIBRARY_EXTENSION=.a
RM=rm -f $(1)
else ifeq ($(OS), Windows_NT) # windows
LIBRARY_EXTENSION=.lib
EXECUTABLE_EXTENSION=.exe
RM=if exist $(1) del $(1)
else
$(error os not supported)
endif

LIBS=../arguments-mini/libarguments-mini$(LIBRARY_EXTENSION)

# non configurable..
DIR_PER_LIB=$(dir $(LIBS))
NOTDIR_PER_LIB=$(patsubst lib%$(LIBRARY_EXTENSION),%,$(notdir $(LIBS)))

arguments-mini-test$(EXECUTABLE_EXTENSION): main.o $(LIBS)
	gcc -Wl,--wrap=malloc,--wrap=free -o arguments-mini-test$(EXECUTABLE_EXTENSION) main.o $(addprefix -L,$(DIR_PER_LIB)) $(addprefix -l,$(NOTDIR_PER_LIB))

# always make..
.PHONY: $(LIBS)
$(LIBS):
	make -C $(dir $@)

main.o: main.c
	gcc -c main.c $(addprefix -I,$(DIR_PER_LIB))

#******************************************************************************

andrun: arguments-mini-test$(EXECEUTABLE_EXTENSION)
	./arguments-mini-test$(EXECUTABLE_EXTENSION)

clean:
	$(call RM,main.o)
	$(call RM,arguments-mini-test)
	$(call RM,arguments-mini-test.exe)