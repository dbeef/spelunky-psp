#include <stdlib.h>
#include <GL/glut.h>
#include <cmath>
#include <GLES/egl.h>


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

#include "common/callbacks.h"
#include "common/vram.h"


PSP_MODULE_INFO("Spelunky", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);


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
    // 480 / 272 = ~1.7
    GLCHK(glOrtho(-8 * 1.7, 8 * 1.7, 8, -8, -8, 8));
    dumpmat(GL_PROJECTION_MATRIX, "ortho proj");
    GLCHK(glMatrixMode(GL_MODELVIEW));
    GLCHK(glLoadIdentity());
    dumpmat(GL_MODELVIEW_MATRIX, "ident modelview");
    dumpmat(GL_PROJECTION_MATRIX, "non-current proj");
    gluLookAt(camera->x, camera->y, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    dumpmat(GL_MODELVIEW_MATRIX, "lookat modelview");
    glLoadIdentity();
}



#define NTEX    1
static GLuint texture_indexes[NTEX];
bool passed = true;

static void display() {

    static Timer timer;
    timer.update();
    static float delta = 0;
    delta = timer.last_delta;

    // limiting to 30 fps?
//    if(!passed) {
//        if (delta < 0.017f) {
//            glutSwapBuffers();
//            glutPostRedisplay();
//            passed = true;
//            return;
//        }
//    }
//
//    passed = false;

//    GLCHK(glShadeModel(GL_CLAMP));
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
//    level->initialise_tiles_from_splash_screen(MAIN_MENU_UPPER);
//    level->initialise_tiles_from_splash_screen(MAIN_MENU_LOWER);

    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_W, SCREEN_H);
    glutReshapeFunc(reshape);
    int window = glutCreateWindow(__FILE__);
    glutDisplayFunc(display);

    GLCHK(glGenTextures(NTEX, texture_indexes));
    GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
    GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCHK(glEnable(GL_TEXTURE_2D));
    level->upload_tilesheet();

    eglSwapInterval(reinterpret_cast<void *>(window), 1);

//    std::string textureFragmentShaderSource(mvp_texture_fragment_shader)
//    std::string textureVertexShaderSource(mvp_texture_vertex_shader);
//
//    Shader textureVertexShader(textureVertexShaderSource, GL_VERTEX_SHADER);
//    Shader textureFragmentShader(textureFragmentShaderSource, GL_FRAGMENT_SHADER);
//
//    ShaderProgram textureShader;
//    textureShader.init(textureFragmentShader, textureVertexShader);
//
    glutMainLoop();
    return 0;
}

