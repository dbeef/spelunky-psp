#include "video/Video.hpp"
#include "graphics_utils/DebugGlCall.hpp"
#include "glad/glad.h"
#include "logger/log.h"

#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <linux/vt.h>
#include <GL/gl.h>

extern "C"
{
//#include <GL/glfbdev.h>
#include "/home/dbeef/MiyooSDK/mesa/include/GL/glfbdev.h"
}

#include <math.h>

static    GLFBDevContextPtr ctx;
static    GLFBDevBufferPtr buf;
static    GLFBDevVisualPtr vis;
static    int bytes, r, g, b, a;
static    float ang;
static    int i;
//static    const int XRes = 320, YRes = 240, Hz = 70;
static    int DesiredDepth = 16;
static    int NumFrames = 100;
static    struct fb_fix_screeninfo FixedInfo;
static    struct fb_var_screeninfo VarInfo, OrigVarInfo;
static    int OriginalVT = -1;
static    int ConsoleFD = -1;
static    int FrameBufferFD = -1;
static    caddr_t FrameBuffer = (caddr_t) -1;
static    caddr_t MMIOAddress = (caddr_t) -1;

namespace
{
    const int attribs[] = {
            GLFBDEV_DOUBLE_BUFFER,
            GLFBDEV_DEPTH_SIZE, 16,
            GLFBDEV_NONE
    };

    void signal_handler(int signumber)
    {
        signal(signumber, SIG_IGN); /* prevent recursion! */
        fprintf(stderr, "error: got signal %d (exiting)\n", signumber);
        exit(1);
    }

    void print_fixed_info(const struct fb_fix_screeninfo *fixed, const char *s)
    {
        static const char *visuals[] = {
                "MONO01", "MONO10", "TRUECOLOR", "PSEUDOCOLOR",
                "DIRECTCOLOR", "STATIC_PSEUDOCOLOR"
        };

        printf("%s info -----------------------\n", s);
        printf("id = %16s\n", fixed->id);
        printf("smem_start = 0x%lx\n", fixed->smem_start);
        printf("smem_len = %d (0x%x)\n", fixed->smem_len, fixed->smem_len);
        printf("type = 0x%x\n", fixed->type);
        printf("type_aux = 0x%x\n", fixed->type_aux);
        printf("visual = 0x%x (%s)\n", fixed->visual, visuals[fixed->visual]);
        printf("xpanstep = %d\n", fixed->xpanstep);
        printf("ypanstep = %d\n", fixed->ypanstep);
        printf("ywrapstep = %d\n", fixed->ywrapstep);
        printf("line_length = %d\n", fixed->line_length);
        printf("mmio_start = 0x%lx\n", fixed->mmio_start);
        printf("mmio_len = %d (0x%x)\n", fixed->mmio_len, fixed->mmio_len);
        printf("accel = 0x%x\n", fixed->accel);
    }

    void print_var_info(const struct fb_var_screeninfo *var, const char *s)
    {
        printf("%s info -----------------------\n", s);
        printf("xres = %d\n", var->xres);
        printf("yres = %d\n", var->yres);
        printf("xres_virtual = %d\n", var->xres_virtual);
        printf("yres_virtual = %d\n", var->yres_virtual);
        printf("xoffset = %d\n", var->xoffset);
        printf("yoffset = %d\n", var->yoffset);
        printf("bits_per_pixel = %d\n", var->bits_per_pixel);
        printf("grayscale = %d\n", var->grayscale);

        printf("red.offset = %d  length = %d  msb_right = %d\n",
               var->red.offset, var->red.length, var->red.msb_right);
        printf("green.offset = %d  length = %d  msb_right = %d\n",
               var->green.offset, var->green.length, var->green.msb_right);
        printf("blue.offset = %d  length = %d  msb_right = %d\n",
               var->blue.offset, var->blue.length, var->blue.msb_right);
        printf("transp.offset = %d  length = %d  msb_right = %d\n",
               var->transp.offset, var->transp.length, var->transp.msb_right);

        printf("nonstd = %d\n", var->nonstd);
        printf("activate = %d\n", var->activate);
        printf("height = %d mm\n", var->height);
        printf("width = %d mm\n", var->width);
        printf("accel_flags = 0x%x\n", var->accel_flags);
        printf("pixclock = %d\n", var->pixclock);
        printf("left_margin = %d\n", var->left_margin);
        printf("right_margin = %d\n", var->right_margin);
        printf("upper_margin = %d\n", var->upper_margin);
        printf("lower_margin = %d\n", var->lower_margin);
        printf("hsync_len = %d\n", var->hsync_len);
        printf("vsync_len = %d\n", var->vsync_len);
        printf("sync = %d\n", var->sync);
        printf("vmode = %d\n", var->vmode);
    }

    void initialize_gl()
    {
        static const int attribs[] = {
                GLFBDEV_DOUBLE_BUFFER,
                GLFBDEV_DEPTH_SIZE, 16,
                GLFBDEV_NONE
        };

        printf("GLFBDEV_VENDOR = %s\n", glFBDevGetString(GLFBDEV_VENDOR));
        printf("GLFBDEV_VERSION = %s\n", glFBDevGetString(GLFBDEV_VERSION));

        /* framebuffer size */
        bytes = VarInfo.xres_virtual * VarInfo.yres_virtual * VarInfo.bits_per_pixel / 8;

	log_info("glFBDevCreateVisual");
        vis = glFBDevCreateVisual( &FixedInfo, &VarInfo, attribs );
        assert(vis);

	log_info("glFBDevCreateBuffer");
        buf = glFBDevCreateBuffer( &FixedInfo, &VarInfo, vis, FrameBuffer, NULL, bytes );
        assert(buf);

	log_info("glFBDevCreateContext");
        ctx = glFBDevCreateContext( vis, NULL );
        assert(buf);

	log_info("glFBDevMakeCurrent");
        b = glFBDevMakeCurrent( ctx, buf, buf );
        assert(b);
    }

    void initialize_fbdev( void )
    {
        char ttystr[1000];
        int fd, vtnumber, ttyfd;
        int sz;

        (void) sz;

        if (geteuid()) {
            fprintf(stderr, "error: you need to be root\n");
            exit(1);
        }

 #if 1
           log_info("Opening framebuffer device...");

        /* open the framebuffer device */
        FrameBufferFD = open("/dev/fb0", O_RDWR);
        if (FrameBufferFD < 0) {
            fprintf(stderr, "Error opening /dev/fb0: %s\n", strerror(errno));
            exit(1);
        }
#endif

        log_info("Opening /dev/tty0...");

        /* open /dev/tty0 and get the vt number */
        if ((fd = open("/dev/tty0", O_WRONLY, 0)) < 0) {
            fprintf(stderr, "error opening /dev/tty0\n");
            exit(1);
        }

        log_info("Getting vt number...");
        
        if (ioctl(fd, VT_OPENQRY, &vtnumber) < 0 || vtnumber < 0) {
            fprintf(stderr, "error: couldn't get a free vt\n");
            exit(1);
        }
        close(fd);

        log_info("Opening console tty...");

        /* open the console tty */
        sprintf(ttystr, "/dev/tty%d", vtnumber);  /* /dev/tty1-64 */
        ConsoleFD = open(ttystr, O_RDWR | O_NDELAY, 0);
        if (ConsoleFD < 0) {
            fprintf(stderr, "error couldn't open console fd\n");
            exit(1);
        }

        /* save current vt number */
        {
            struct vt_stat vts;
            if (ioctl(ConsoleFD, VT_GETSTATE, &vts) == 0)
                OriginalVT = vts.v_active;
        }

        log_info("Disconnecting from controlling tty...");

        /* disconnect from controlling tty */
        ttyfd = open("/dev/tty", O_RDWR);
        if (ttyfd >= 0) {
            ioctl(ttyfd, TIOCNOTTY, 0);
            close(ttyfd);
        }

#if 0
          log_info("Magic...");

        /* some magic to restore the vt when we exit */
        {
            struct vt_mode vt;
            if (ioctl(ConsoleFD, VT_ACTIVATE, vtnumber) != 0)
                printf("ioctl VT_ACTIVATE: %s\n", strerror(errno));
            if (ioctl(ConsoleFD, VT_WAITACTIVE, vtnumber) != 0)
                printf("ioctl VT_WAITACTIVE: %s\n", strerror(errno));

            if (ioctl(ConsoleFD, VT_GETMODE, &vt) < 0) {
                fprintf(stderr, "error: ioctl VT_GETMODE: %s\n", strerror(errno));
                exit(1);
            }

            vt.mode = VT_PROCESS;
            vt.relsig = SIGUSR1;
            vt.acqsig = SIGUSR1;
            if (ioctl(ConsoleFD, VT_SETMODE, &vt) < 0) {
                fprintf(stderr, "error: ioctl(VT_SETMODE) failed: %s\n",
                        strerror(errno));
                exit(1);
            }
        }
#endif

        log_info("Going into graphics mode...");


        /* go into graphics mode */
        if (ioctl(ConsoleFD, KDSETMODE, KD_GRAPHICS) < 0) {
            fprintf(stderr, "error: ioctl(KDSETMODE, KD_GRAPHICS) failed: %s\n",
                    strerror(errno));
            exit(1);
        }


#if 0
        /* open the framebuffer device */
   FrameBufferFD = open("/dev/fb0", O_RDWR);
   if (FrameBufferFD < 0) {
      fprintf(stderr, "Error opening /dev/fb0: %s\n", strerror(errno));
      exit(1);
   }
#endif

        /* Get the fixed screen info */
        if (ioctl(FrameBufferFD, FBIOGET_FSCREENINFO, &FixedInfo)) {
            fprintf(stderr, "error: ioctl(FBIOGET_FSCREENINFO) failed: %s\n",
                    strerror(errno));
            exit(1);
        }

        print_fixed_info(&FixedInfo, "Fixed");


        /* get the variable screen info */
        if (ioctl(FrameBufferFD, FBIOGET_VSCREENINFO, &OrigVarInfo)) {
            fprintf(stderr, "error: ioctl(FBIOGET_VSCREENINFO) failed: %s\n",
                    strerror(errno));
            exit(1);
        }

        print_var_info(&OrigVarInfo, "Orig Var");

        /* operate on a copy */
        VarInfo = OrigVarInfo;

        /* set the depth, resolution, etc */
        if (DesiredDepth)
            VarInfo.bits_per_pixel = DesiredDepth;

        if (VarInfo.bits_per_pixel == 16) {
            VarInfo.red.offset = 11;
            VarInfo.green.offset = 5;
            VarInfo.blue.offset = 0;
            VarInfo.red.length = 5;
            VarInfo.green.length = 6;
            VarInfo.blue.length = 5;
            VarInfo.transp.offset = 0;
            VarInfo.transp.length = 0;
        }
        else if (VarInfo.bits_per_pixel == 32) {
            VarInfo.red.offset = 16;
            VarInfo.green.offset = 8;
            VarInfo.blue.offset = 0;
            VarInfo.transp.offset = 24;
            VarInfo.red.length = 8;
            VarInfo.green.length = 8;
            VarInfo.blue.length = 8;
            VarInfo.transp.length = 8;
        }

        VarInfo.xoffset = 0;
        VarInfo.yoffset = 0;
        VarInfo.nonstd = 0;
        VarInfo.vmode &= ~FB_VMODE_YWRAP; /* turn off scrolling */

        log_info("Setting new variable screen info.");

        /* set new variable screen info */
        if (ioctl(FrameBufferFD, FBIOPUT_VSCREENINFO, &VarInfo)) {
            fprintf(stderr, "ioctl(FBIOPUT_VSCREENINFO failed): %s\n",
                    strerror(errno));
            exit(1);
        }

        print_var_info(&VarInfo, "New Var");

        if (FixedInfo.visual != FB_VISUAL_TRUECOLOR &&
            FixedInfo.visual != FB_VISUAL_DIRECTCOLOR) {
            fprintf(stderr, "non-TRUE/DIRECT-COLOR visuals (0x%x) not supported by this demo.\n", FixedInfo.visual);
            exit(1);
        }

        /* initialize colormap */
        if (FixedInfo.visual == FB_VISUAL_DIRECTCOLOR) {
            struct fb_cmap cmap;
            unsigned short red[256], green[256], blue[256];
            int i;

            /* we're assuming 256 entries here */
            printf("initializing directcolor colormap\n");
            cmap.start = 0;
            cmap.len = 256;
            cmap.red   = red;
            cmap.green = green;
            cmap.blue  = blue;
            cmap.transp = NULL;
            for (i = 0; i < cmap.len; i++) {
                red[i] = green[i] = blue[i] = (i << 8) | i;
            }
            if (ioctl(FrameBufferFD, FBIOPUTCMAP, (void *) &cmap) < 0) {
                fprintf(stderr, "ioctl(FBIOPUTCMAP) failed [%d]\n", i);
            }
        }

        /*
         * fbdev says the frame buffer is at offset zero, and the mmio region
         * is immediately after.
         */

        /* mmap the framebuffer into our address space */
        FrameBuffer = (caddr_t) mmap(0, /* start */
                                     FixedInfo.smem_len, /* bytes */
                                     PROT_READ | PROT_WRITE, /* prot */
                                     MAP_SHARED, /* flags */
                                     FrameBufferFD, /* fd */
                                     0 /* offset */);
        if (FrameBuffer == (caddr_t) - 1) {
            fprintf(stderr, "error: unable to mmap framebuffer: %s\n",
                    strerror(errno));
            exit(1);
        }
        printf("FrameBuffer = %p\n", FrameBuffer);

#if 1
        /* mmap the MMIO region into our address space */
        MMIOAddress = (caddr_t) mmap(0, /* start */
                                     FixedInfo.mmio_len, /* bytes */
                                     PROT_READ | PROT_WRITE, /* prot */
                                     MAP_SHARED, /* flags */
                                     FrameBufferFD, /* fd */
                                     FixedInfo.smem_len /* offset */);
        if (MMIOAddress == (caddr_t) - 1) {
            fprintf(stderr, "error: unable to mmap mmio region: %s\n",
                    strerror(errno));
        }
        printf("MMIOAddress = %p\n", MMIOAddress);

        /* try out some simple MMIO register reads */
        if (0)
        {
            typedef unsigned int CARD32;
            typedef unsigned char CARD8;
#define RADEON_CONFIG_MEMSIZE               0x00f8
#define RADEON_MEM_SDRAM_MODE_REG           0x0158
#define MMIO_IN32(base, offset) \
	*(volatile CARD32 *)(void *)(((CARD8*)(base)) + (offset))
#define INREG(addr)         MMIO_IN32(MMIOAddress, addr)
            int sz, type;
            const char *typeStr[] = {"SDR", "DDR", "64-bit SDR"};
            sz = INREG(RADEON_CONFIG_MEMSIZE);
            type = INREG(RADEON_MEM_SDRAM_MODE_REG);
            printf("RADEON_CONFIG_MEMSIZE = %d (%d MB)\n", sz, sz / 1024 / 1024);
            printf("RADEON_MEM_SDRAM_MODE_REG >> 30 = %d (%s)\n",
                   type >> 30, typeStr[type>>30]);
        }
#endif
    }

    void shutdown_fbdev()
    {
        struct vt_mode VT;

        printf("cleaning up...\n");
        /* restore original variable screen info */
        if (ioctl(FrameBufferFD, FBIOPUT_VSCREENINFO, &OrigVarInfo)) {
            fprintf(stderr, "ioctl(FBIOPUT_VSCREENINFO failed): %s\n",
                    strerror(errno));
            exit(1);
        }

        munmap(MMIOAddress, FixedInfo.mmio_len);
        munmap(FrameBuffer, FixedInfo.smem_len);
        close(FrameBufferFD);

        /* restore text mode */
        ioctl(ConsoleFD, KDSETMODE, KD_TEXT);

        /* set vt */
        if (ioctl(ConsoleFD, VT_GETMODE, &VT) != -1) {
            VT.mode = VT_AUTO;
            ioctl(ConsoleFD, VT_SETMODE, &VT);
        }

        /* restore original vt */
        if (OriginalVT >= 0) {
            ioctl(ConsoleFD, VT_ACTIVATE, OriginalVT);
            OriginalVT = -1;
        }

        close(ConsoleFD);
    }
}

struct PlatformSpecific
{
};

Video::~Video() = default; // For pimpl idiom.
Video::Video() : _timestep(60) {}; // For pimpl idiom.

void Video::tear_down_gl()
{
    /* clean up */
    b = glFBDevMakeCurrent( NULL, NULL, NULL);
    assert(b);

    glFBDevDestroyContext(ctx);
    glFBDevDestroyBuffer(buf);
    glFBDevDestroyVisual(vis);
}

void Video::swap_buffers() const
{
    glFBDevSwapBuffers(buf);
}

bool Video::setup_gl()
{
    signal(SIGUSR1, signal_handler);  /* exit if someone tries a vt switch */
    signal(SIGSEGV, signal_handler);  /* catch segfaults */

    log_info("Entered Video::setup_gl.");
    log_info("Initializing fbdev");
    initialize_fbdev();
    log_info("Initializing GL");
    initialize_gl();

    _platform_specific = std::make_unique<PlatformSpecific>();

    if (!gladLoadGLLoader((GLADloadproc) glFBDevGetProcAddress))
    {
        log_error("Error while loading ptrs to OpenGL functions");
        return false;
    }

        log_info("Now GLAD should take on...");
        /*printf("GL_EXTENSIONS: %s\n", glGetString(GL_EXTENSIONS));*/
        glGetIntegerv(GL_RED_BITS, &r);
        glGetIntegerv(GL_GREEN_BITS, &g);
        glGetIntegerv(GL_BLUE_BITS, &b);
        glGetIntegerv(GL_ALPHA_BITS, &a);
        printf("RED_BITS=%d GREEN_BITS=%d BLUE_BITS=%d ALPHA_BITS=%d\n",
               r, g, b, a);


    _viewport = std::make_shared<Viewport>(320, 240);

    DebugGlCall(glEnable(GL_TEXTURE_2D));
    DebugGlCall(glShadeModel(GL_SMOOTH));
    DebugGlCall(glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE));

    DebugGlCall(glEnable(GL_BLEND));
    DebugGlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    DebugGlCall(glEnableClientState(GL_VERTEX_ARRAY)); // For glVertexPointer
    DebugGlCall(glEnableClientState(GL_TEXTURE_COORD_ARRAY)); // For glTexCoordPointer

    // Disable, as it is not used and may affect performance:
    DebugGlCall(glDisableClientState(GL_COLOR_ARRAY));
    DebugGlCall(glDisableClientState(GL_EDGE_FLAG_ARRAY));
    DebugGlCall(glDisableClientState(GL_NORMAL_ARRAY));
    DebugGlCall(glDisableClientState(GL_INDEX_ARRAY));

    DebugGlCall(glDisable(GL_FOG));
    DebugGlCall(glDisable(GL_LIGHTING));
    DebugGlCall(glDisable(GL_CULL_FACE));
    DebugGlCall(glDisable(GL_ALPHA_TEST));
    DebugGlCall(glDisable(GL_COLOR_LOGIC_OP));
    DebugGlCall(glDisable(GL_DITHER));
    DebugGlCall(glDisable(GL_STENCIL_TEST));
    DebugGlCall(glDisable(GL_DEPTH_TEST));
    DebugGlCall(glDisable(GL_POINT_SMOOTH));
    DebugGlCall(glDisable(GL_LINE_SMOOTH));
    DebugGlCall(glDisable(GL_SCISSOR_TEST));
    DebugGlCall(glDisable(GL_COLOR_MATERIAL));
    DebugGlCall(glDisable(GL_NORMALIZE));
    DebugGlCall(glDisable(GL_RESCALE_NORMAL));

    log_info("Exiting Video::setup_gl, success.");
    return true;
}
