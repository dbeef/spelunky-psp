TARGET = sprite
OBJS = tex.o firefox.o
#psp-setup-tex.o firefox.o
#common/callbacks.o common/vram.o

INCDIR =
CFLAGS = -G0 -Wall -O2
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
LIBS= -lpspgum -lpspgu -lm -lglut -lGLU -lGL -lm -lc -lpsputility -lpspdebug -lpspge -lpspdisplay -lpspctrl -lpspsdk -lpspvfpu -lpsplibc -lpspuser -lpspkernel -lpsprtc

#psp-setup-tex.o: psp-setup.c
#	$(CC) $(CFLAGS) -o $@ -c -DMODULE_NAME="$(patsubst psp-setup-tex.o,tex,$@)" psp-setup.c

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Sprite Sample

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak


