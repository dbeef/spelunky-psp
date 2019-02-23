//
// Created by xdbeef on 04.03.18.
//

#ifndef SPELUNKYDS_MAPUTILS_H
#define SPELUNKYDS_MAPUTILS_H

int floor_div(int a, int b);

static void dumpmat(GLenum mat, const char *s) {
    float m[16];
    glGetFloatv(mat, m);
}

#endif //SPELUNKYDS_MAPUTILS_H