//
// Created by xdbeef on 04.03.18.
//

#include <cstring>
#include <cstdlib>
#include <stdlib.h>
#include <GL/gl.h>

//https://stackoverflow.com/questions/11072244/c-multiple-definitions-of-a-variable

//TODO Move this function to math_utils
int floor_div(int a, int b) {
    div_t r = div(a, b);
    if (r.rem != 0 && ((a < 0) ^ (b < 0)))
        r.quot--;
    return r.quot;
}

