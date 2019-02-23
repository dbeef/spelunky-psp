TARGET = sprite

SOURCES		:=	. camera common input rooms tiles timer utils
CFILES		:=	$(foreach dir,$(SOURCES), $(wildcard $(dir)/*.c))
CPPFILES	:=	$(foreach dir,$(SOURCES), $(wildcard $(dir)/*.cpp))
PRECOMPILED	:=	$(foreach dir,$(SOURCES), $(wildcard $(dir)/*.o))

#OBJS := main.o gfxcavebg.o tiles/Level.o

OBJS := $(addsuffix .o,$(BINFILES)) \
					$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(PRECOMPILED:.c=.o)

INCDIR =
CFLAGS = -G0 -Wall -O2
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
LIBS= -lpspgum -lpspgu -lm -lglut -lGLU -lGL -lm -lc -lpsputility -lpspdebug -lpspge -lpspdisplay -lpspctrl -lpspsdk -lpspvfpu -lpsplibc -lpspuser -lpspkernel -lpsprtc -lstdc++

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Sprite Sample

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
