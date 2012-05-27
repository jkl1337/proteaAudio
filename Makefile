#--- generic settings ----------------------------------------------
# settings for C++ compiler:
C      = gcc
CC     = g++
CFLAGS = -O2 -Wall # -D_DEBUG -g
INCDIR = -Irtaudio -Irtaudio/include -I../lua/src

# linker settings:
LCC    = ar
LFLAGS = -rcs
LNAME = libproAudio.a
LIB    = $(LNAME)
LIBDIR =

# settings for optional libSDL backend:
INCDIR += -I../archive/baseCode/include
SDLLIB = -lSDLmain -lSDL
SDLDIR = -L/usr/lib -L../archive/baseCode/lib

#--- platform specific settings ------------------------------------
ARCH = $(shell uname -s)
ifeq ($(ARCH),Linux)
  LIBS          = $(LIBDIR) $(LIB) -lpthread -lasound
  LUALIB      = -llua -ldl
  CFLAGS += -DHAVE_GETTIMEOFDAY -D__LINUX_ALSA__ #-D__LINUX_OSS__
  DLLFLAGS =  -fPIC -shared
  DLLSUFFIX = .so
  EXESUFFIX =
  
else  
  ifeq ($(ARCH),Darwin) # MacOSX
    LIBS          = $(LIBDIR) $(LIB)
    LUALIB      = -L/usr/local/lib -llua
    CFLAGS += -DHAVE_GETTIMEOFDAY -D__MACOSX_CORE__
    DLLFLAGS = -bundle 
    DLLSUFFIX = .so
    EXESUFFIX = .app
    
  else  # windows, MinGW
    LIBS          = $(LIBDIR) $(LIB) -lole32 -ldsound -lwinmm -mconsole -s
    LUALIB      = -L../lua/src -llua51
    SDLLIB = -lmingw32 -lSDLmain -lSDL -mconsole -s
    CFLAGS += -D__WINDOWS_DS__
    DLLFLAGS =  -shared
    DLLSUFFIX = .dll
    EXESUFFIX = .exe
  endif
endif

#--- make targets and rules ----------------------------------------

# by default, proteaAudio makes use of the included rtAudio backend
rtaudio: $(LNAME) example$(EXESUFFIX) playAudioRt$(EXESUFFIX)

# the make all target additionally builds the Lua frontend and SDL backend, and therefore has additional dependencies
ALL = $(LNAME) example$(EXESUFFIX) playAudioRt$(EXESUFFIX) proAudioRt$(DLLSUFFIX) playAudioSdl$(EXESUFFIX)
all: $(ALL)

# static library
OBJ = proAudio.o proAudioRt.o stb_vorbis.o rtaudio/RtAudio.o
$(LNAME) : $(OBJ)
	$(LCC) $(LFLAGS) $@ $^

# minimal example
example$(EXESUFFIX) : example.o
	$(CC) $^ $(LIBS) -o $@

# flexible example
playAudioRt$(EXESUFFIX) : playAudioRt.o
	$(CC) $^ $(LIBS) -o $@

# optional Lua frontend
lua: proAudioRt$(DLLSUFFIX)

proAudioRt$(DLLSUFFIX): proAudioRt_lua.o
	$(CC) -o $@ $(DLLFLAGS) $^ $(LIBS) $(LUALIB)

# example for optional libSDL backend
sdl: playAudioSdl$(EXESUFFIX)

playAudioSdl$(EXESUFFIX): playAudioSdl.o proAudio.o proAudioSdl.o stb_vorbis.o
	$(CC) $(CFLAGS) $^ $(SDLDIR) $(SDLLIB) -o $@

# generic rules
.c.o:
	$(C) $(CFLAGS) $(INCDIR) -c $< -o $@
.cpp.o:
	$(CC) $(CFLAGS) $(INCDIR) -c $< -o $@
clean:
	rm -f *.o *~ $(OBJ) $(ALL)

#--- project specific dependencies ---------------------------------
HDR = proAudio.h proAudioRt.h
playAudioRt.o: playAudioRt.cpp $(HDR)
serverAudioRt.o: serverAudioRt.cpp $(HDR)
proAudioRt_lua.o:  proAudioRt_lua.cpp $(HDR)
proAudio.o: proAudio.cpp proAudio.h 
proAudioRt.o: proAudioRt.cpp $(HDR)
stb_vorbis.o: stb_vorbis.c
rtaudio/RtAudio.o: rtaudio/RtAudio.cpp rtaudio/RtAudio.h rtaudio/RtError.h
example.o: example.cpp $(HDR)
playAudioSdl.o: playAudioSdl.cpp proAudioSdl.h proAudio.h
proAudioSdl.o: proAudioSdl.cpp proAudioSdl.h proAudio.h
