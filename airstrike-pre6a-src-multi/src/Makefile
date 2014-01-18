
#headfile
CFLAGS:= -g -O2 -Wall -Wno-unused $(OPTIONS)
CFLAGS += $(shell sdl-config --cflags) -Isprite -I. -Isupport  -DINLINE=inline -lm
#lib
CLIB:= -lSDL_image -lSDL_mixer -lm -lzmq -pthread
#C Files
CFILES:= $(shell find . -name '*.c') 
COBJECTS:= $(CFILES:.c=.o)

airstrike: $(COBJECTS) $(CPPOBJECTS)
	gcc -o $@ $(CFLAGS) $(COBJECTS) $(CPPOBJECTS) `sdl-config --libs` $(CLIB)

airstrike-sound: $(OBJECTS)
	gcc -o $@ $(CFLAGS) $(OBJECTS) `sdl-config --libs` -lSDL_image -lSDL_mixer -lm -lzmq -pthread

#$(COBJECTS): %.o: %.c
#	gcc $(CFLAGS) -c $< -o $@

# Use the rule below when coding new sprite types
#sprites_autogen.h: sprite_types/*.c
#	../utils/genspritelist.sh $^

clean:
	rm -f `find . -name '*.[od]' -or -name '*~' -or -name '*.bb' -or -name '*.bbg'` airstrike

dryclean: 
	rm -f `find . -name '*.[^ch]' -type f` airstrike

#Automatic dependency handling. This is not needed for a simple compile.

#include $(CFILES:.c=.d)
#
#%.d: %.c
#	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
#	sed 's,$(*F)\.o[ :]*,$(@:.d=.o) $@ : ,g' < $@.$$$$ > $@; \
#	rm -f $@.$$$$
