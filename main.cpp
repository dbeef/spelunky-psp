#include <stdlib.h>
#include <GL/glut.h>
#include <cmath>


#include "tiles/LevelGenerator.hpp"
#include "tiles/Level.hpp"
#include "rooms/ExitRooms.hpp"
#include "tiles/MapTileType.h"

// FIXME Probably shoudln't do this manually
#define SYS 1
#define LOGME 0

#include "glchk.h"
#include "input/InputHandler.h"
#include "timer/Timer.h"
#include "utils/Consts.h"
#include "tiles/LevelRenderingUtils.hpp"

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
static Level *level = new Level(camera);
static InputHandler inputHandler(camera);


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
static GLuint texture_indexes[NTEX];

static void display() {

    timer.update();

    static GLfloat angle;
    static Timer timer;
    angle += delta;

    GLCHK(glShadeModel(GL_SMOOTH));

    GLCHK(glClear(GL_COLOR_BUFFER_BIT));

    GLCHK(glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE));
    GLCHK(glEnable(GL_BLEND));
    GLCHK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    inputHandler.handle();

    level->write_tiles_to_map();

    glutSwapBuffers();

    glutPostRedisplay();

//#if SYS
//    usleep(1000000/30);
//#endif
}


int main(int argc, char *argv[]) {

    level->init_map_tiles();
    level->generate_frame();
    generate_new_level_layout(level);
    level->initialise_tiles_from_room_layout();

    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_W, SCREEN_H);
    glutReshapeFunc(reshape);
    glutCreateWindow(__FILE__);
    glutDisplayFunc(display);

    GLCHK(glGenTextures(NTEX, texture_indexes));
    GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

    glutMainLoop();
    return 0;
}

