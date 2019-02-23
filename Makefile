# Utilizing exisiting resource compiler


#%.raw: %.png
#		convert $< rgba:$@
#%-mono.raw: %.png
		#convert $< gray:$@
#%.o: %.raw
#	    (sym=`echo $(notdir $*) | tr '-' '_'`; \
#		 echo -e ".data\n.global $${sym}_start\n$${sym}_start:\n\t.incbin \"$<\"" | $(AS) -o $@)


TARGET = sprite
OBJS := main.o gfxcavebg.o

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
