SOURCES		:=	. camera common input rooms tiles timer utils
CFILES		:=	$(foreach dir,$(SOURCES), $(wildcard $(dir)/*.c))
CPPFILES	:=	$(foreach dir,$(SOURCES), $(wildcard $(dir)/*.cpp))
PRECOMPILED	:=	$(foreach dir,$(SOURCES), $(wildcard $(dir)/*.o))

#OBJS := main.o gfxcavebg.o tiles/Level.o

OBJS := $(addsuffix .o,$(BINFILES)) \
					$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(PRECOMPILED:.c=.o)

INCDIR =
CFLAGS = -G0 -Wall -O2
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti -std=c++14
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
LIBS= -lpspgum -lpspgu -lm -lglut -lGLU -lGL -lm -lc -lpsputility -lpspdebug -lpspge -lpspdisplay -lpspctrl -lpspsdk -lpspvfpu -lpsplibc -lpspuser -lpspkernel -lpsprtc -lstdc++

TARGET            = Spelunky
EXTRA_TARGETS     = EBOOT.PBP
VERSION           = 1.00
#PSP_LARGE_MEMORY = 1 #to use all 64MB on a PSP-2000 or later
PSP_EBOOT_PIC1    = pic1.png
PSP_EBOOT_ICON    = icon.png
PSP_EBOOT_TITLE   = Spelunky PSP

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
