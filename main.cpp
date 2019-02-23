#include <stdlib.h>
#include <GL/glut.h>
#include <cmath>

#include "tiles/MapTileType.h"
// FIXME Probably shoudln't do this manually
#define SYS 1
#define LOGME 0

#include "glchk.h"
#include "input/InputHandler.h"
#include "timer/Timer.h"
#include "utils/Consts.h"

#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <stdlib.h>
#include <pspctrl.h>
#include <pspgu.h>
#include <pspgum.h>
#include <stdio.h>
#include <pspsystimer.h>
#include <time.h>
#include <angelscript.h>

PSP_MODULE_INFO("Orthographic Projection", 0x1000, 1, 1);

static Timer timer;
static Camera *camera = new Camera();
static InputHandler inputHandler(camera);

static void dumpmat(GLenum mat, const char *s) {
    float m[16];
    glGetFloatv(mat, m);
}

static
void reshape(int w, int h) {
    GLCHK(glViewport(0, 0, w, h));
    GLCHK(glMatrixMode(GL_PROJECTION));
    dumpmat(GL_PROJECTION_MATRIX, "fresh proj");
    GLCHK(glLoadIdentity());
    dumpmat(GL_PROJECTION_MATRIX, "ident proj");
    GLCHK(glOrtho(-8 * 1.7, 8 * 1.7, 8, -8, -8, 8));
    dumpmat(GL_PROJECTION_MATRIX, "ortho proj");
    GLCHK(glMatrixMode(GL_MODELVIEW));
    GLCHK(glLoadIdentity());
    dumpmat(GL_MODELVIEW_MATRIX, "ident modelview");
    dumpmat(GL_PROJECTION_MATRIX, "non-current proj");
    gluLookAt(camera->x, camera->y, 2.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    dumpmat(GL_MODELVIEW_MATRIX, "lookat modelview");
    glLoadIdentity();
}


static float delta = 1.0;

#define NTEX    1
static GLuint texid[NTEX];

extern unsigned char gfxcavebg_start[];
extern unsigned char logo_start[];

static
void display() {

    timer.update();

    int i;
    static GLfloat angle;
    static Timer timer;
    angle += delta;

    GLCHK(glShadeModel(GL_SMOOTH));

    GLCHK(glClear(GL_COLOR_BUFFER_BIT));

    GLCHK(glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE));
    GLCHK(glEnable(GL_BLEND));
    GLCHK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    inputHandler.handle();

    for(int a =0;a<5;a++) {
        GLCHK(glMatrixMode(GL_MODELVIEW));
        GLCHK(glLoadIdentity());
        GLCHK(glTranslatef(camera->x + a, camera->y, 0));
        dumpmat(GL_MODELVIEW_MATRIX, "trans modelview");
        //GLCHK(glRotatef(angle * 1.32f, 0.0f, 0.0f, 1.0f));
        //dumpmat(GL_MODELVIEW_MATRIX, "rot modelview");

        GLCHK(glBindTexture(GL_TEXTURE_2D, texid[0]));

        glBegin(GL_TRIANGLE_FAN);
        glColor3f(1, 0, 0);
        glTexCoord2f(0, 0);
        glVertex3f(0, 0, 0);

        glColor3f(0, 1, 0);
        glTexCoord2f(0, 1);
        glVertex3f(0, 1, 0);

        glColor3f(0, 0, 1);
        glTexCoord2f(1, 1);
        glVertex3f(1, 1, 0);

        glColor3f(1, 1, 1);
        glTexCoord2f(1, 0);
        glVertex3f(1, 0, 0);
        GLCHK(glEnd());
    }

    glutSwapBuffers();
    glutPostRedisplay();
//#if SYS
//    usleep(1000000/30);
//#endif
}

/*
   F E D C B A 9 8|7 6 5 4 3 2 1 0
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |R R R R R|G G G G G G|B B B B B| 565
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |R R R R R|G G G G G|B B B B B|A| 5551
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |R R R R|G G G G|B B B B|A A A A| 4444
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
#define RGB565(r, g, b)        ((((r) << 8) & 0xf800) | (((g) << 3) & 0x07E0) | (((b) >> 3) & 0x001f))
#define RGBA5551(r, g, b, a)    ((((r) << 8) & 0xf800) | (((g) << 3) & 0x07C0) | (((b) >> 2) & 0x003e) | (((a) >> 7) & 0x0001))
#define RGBA4444(r, g, b, a)    ((((r) << 8) & 0xf000) | (((g) << 4) & 0x0f00) | (((b)     ) & 0x00f0) | (((a) >> 4) & 0x000f))

int main(int argc, char *argv[]) {


    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    int i;
    unsigned short tex16[16 * 16];

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_W, SCREEN_H);
    glutReshapeFunc(reshape);
    glutCreateWindow(__FILE__);
    glutDisplayFunc(display);

    GLCHK(glGenTextures(NTEX, texid));
    GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

    unsigned int current_tile = static_cast<unsigned int>(STONE_BLOCK) - 1;
    unsigned int row = static_cast<unsigned int>(ceil((current_tile / 2) + 1));
    int offset = (row - 1) * 2 * 16 * 16;

    if (offset < 0)
        exit(0);

    bool row_offset = (current_tile % 2 == 1);

    unsigned int index = 0;
    if (row_offset) index += 16;

    bool first = false;

    for (i = 0; i < 16 * 16; i++) {
        if (i % 16 == 0 && first) {
            index += 16;
        }

        tex16[i] = RGBA4444(gfxcavebg_start[((offset + index) * 4) + 0],
                            gfxcavebg_start[((offset + index) * 4) + 1],
                            gfxcavebg_start[((offset + index) * 4) + 2],
                            gfxcavebg_start[((offset + index) * 4) + 3]);

        index++;
        first = true;
    }
    GLCHK(glBindTexture(GL_TEXTURE_2D, texid[0]));
    GLCHK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 16, 16, 0, GL_RGB, GL_UNSIGNED_SHORT_4_4_4_4, tex16));

    GLCHK(glEnable(GL_TEXTURE_2D));

    glutMainLoop();
    return 0;
}

