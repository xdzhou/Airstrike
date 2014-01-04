LIB_PATH=../PhotonSDK
#headfile
CFLAGS:= -g -O2 -Wall -Wno-unused $(OPTIONS)
CFLAGS += $(shell sdl-config --cflags) -Isprite -I$(LIB_PATH) -I. -Isupport -I../enet/include -DINLINE=inline -lm
#lib
CLIB:= -L$(LIB_PATH)/Common-cpp -L$(LIB_PATH)/Photon-cpp -lSDL_image -lSDL_mixer -lm -lenet -pthread -lPhotonRelease32 -lCommonRelease32
#C Files
CFILES:= $(shell find . -name '*.c') 
COBJECTS:= $(CFILES:.c=.o)
#cpp files
CPPFILES:= $(shell find . -name '*.cpp')
CPPOBJECTS:= $(CPPFILES:.cpp=.o)

airstrike: $(COBJECTS) $(CPPOBJECTS)
	g++ -o $@ $(CFLAGS) $(COBJECTS) $(CPPOBJECTS) `sdl-config --libs` $(CLIB)

airstrike-sound: $(OBJECTS)
	g++ -o $@ $(CFLAGS) $(OBJECTS) `sdl-config --libs` -lSDL_image -lSDL_mixer -lm -lenet -pthread -lPhotonRelease32 -lCommonRelease32

$(COBJECTS): %.o: %.c
	gcc $(CFLAGS) -c $< -o $@

$(CPPOBJECTS): %.o: %.cpp
	g++ -c $(CFLAGS) $< -o $@

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
