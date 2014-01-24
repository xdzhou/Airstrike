
#headfile
CFLAGS:= -g -O2 -Wall -Wno-unused $(OPTIONS)
CFLAGS += $(shell sdl-config --cflags) -Isprite -I. -Isupport  -DINLINE=inline -lm 
#CFLAGS += -L ~/Tool/zlib-1.2.8/
#CFLAGS += -L ~/Tool/libpng-1.6.8/.libs/
CFLAGS += -I ~/Tool/zeromq-4.0.3/include/ -L ~/Tool/zeromq-4.0.3/src/.libs
CFLAGS += -I ~/Tool/SDL_image-1.2.12/ -L ~/Tool/SDL_image-1.2.12/.libs
CFLAGS += -I ~/Tool/SDL_mixer-1.2.12/ -L ~/Tool/SDL_mixer-1.2.12/build/.libs
#lib
CLIB:= -lSDL_image -lSDL_mixer -lm -lzmq -lpthread
#C Files
CFILES:= $(shell find . -name '*.c') 
COBJECTS:= $(CFILES:.c=.o)

airstrike: $(COBJECTS)
	gcc -o $@ $(CFLAGS) $(COBJECTS) `sdl-config --libs` $(CLIB)

airstrike-sound: $(OBJECTS)
	gcc -o $@ $(CFLAGS) $(OBJECTS) `sdl-config --libs` $(CLIB)

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
