/*

    OpenGL ES loader generated by glad 0.1.31 on Sun Jul  7 12:10:51 2019.

    Language/Generator: C/C++
    Specification: gl
    APIs: gles1=1.0
    Profile: compatibility
    Extensions:
        
    Loader: True
    Local files: False
    Omit khrplatform: False
    Reproducible: False

    Commandline:
        --profile="compatibility" --api="gles1=1.0" --generator="c" --spec="gl" --extensions=""
    Online:
        https://glad.dav1d.de/#profile=compatibility&language=c&specification=gl&loader=on&api=gles1%3D1.0
*/


#ifndef __glad_h_
#define __glad_h_

#ifdef __gl_h_
#error OpenGL ES 1 header already included, remove this include, glad already provides it
#endif
#define __gl_h_

#if defined(_WIN32) && !defined(APIENTRY) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
#define APIENTRY __stdcall
#endif

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif

#ifndef GLAPIENTRY
#define GLAPIENTRY APIENTRY
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct gladGLversionStruct {
    int major;
    int minor;
};

typedef void* (* GLADloadproc)(const char *name);

#ifndef GLAPI
# if defined(GLAD_GLAPI_EXPORT)
#  if defined(_WIN32) || defined(__CYGWIN__)
#   if defined(GLAD_GLAPI_EXPORT_BUILD)
#    if defined(__GNUC__)
#     define GLAPI __attribute__ ((dllexport)) extern
#    else
#     define GLAPI __declspec(dllexport) extern
#    endif
#   else
#    if defined(__GNUC__)
#     define GLAPI __attribute__ ((dllimport)) extern
#    else
#     define GLAPI __declspec(dllimport) extern
#    endif
#   endif
#  elif defined(__GNUC__) && defined(GLAD_GLAPI_EXPORT_BUILD)
#   define GLAPI __attribute__ ((visibility ("default"))) extern
#  else
#   define GLAPI extern
#  endif
# else
#  define GLAPI extern
# endif
#endif

GLAPI struct gladGLversionStruct GLVersion;
GLAPI int gladLoadGLES1Loader(GLADloadproc);

#include <KHR/khrplatform.h>
typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef void GLvoid;
typedef khronos_int8_t GLbyte;
typedef khronos_uint8_t GLubyte;
typedef khronos_int16_t GLshort;
typedef khronos_uint16_t GLushort;
typedef int GLint;
typedef unsigned int GLuint;
typedef khronos_int32_t GLclampx;
typedef int GLsizei;
typedef khronos_float_t GLfloat;
typedef khronos_float_t GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void *GLeglClientBufferEXT;
typedef void *GLeglImageOES;
typedef char GLchar;
typedef char GLcharARB;
#ifdef __APPLE__
typedef void *GLhandleARB;
#else
typedef unsigned int GLhandleARB;
#endif
typedef khronos_uint16_t GLhalf;
typedef khronos_uint16_t GLhalfARB;
typedef khronos_int32_t GLfixed;
typedef khronos_intptr_t GLintptr;
typedef khronos_intptr_t GLintptrARB;
typedef khronos_ssize_t GLsizeiptr;
typedef khronos_ssize_t GLsizeiptrARB;
typedef khronos_int64_t GLint64;
typedef khronos_int64_t GLint64EXT;
typedef khronos_uint64_t GLuint64;
typedef khronos_uint64_t GLuint64EXT;
typedef struct __GLsync *GLsync;
struct _cl_context;
struct _cl_event;
typedef void (APIENTRY *GLDEBUGPROC)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef void (APIENTRY *GLDEBUGPROCARB)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef void (APIENTRY *GLDEBUGPROCKHR)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef void (APIENTRY *GLDEBUGPROCAMD)(GLuint id,GLenum category,GLenum severity,GLsizei length,const GLchar *message,void *userParam);
typedef unsigned short GLhalfNV;
typedef GLintptr GLvdpauSurfaceNV;
typedef void (APIENTRY *GLVULKANPROCNV)(void);
#define GL_VERSION_ES_CL_1_0 1
#define GL_VERSION_ES_CM_1_1 1
#define GL_VERSION_ES_CL_1_1 1
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_STENCIL_BUFFER_BIT 0x00000400
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_FALSE 0
#define GL_TRUE 1
#define GL_POINTS 0x0000
#define GL_LINES 0x0001
#define GL_LINE_LOOP 0x0002
#define GL_LINE_STRIP 0x0003
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN 0x0006
#define GL_NEVER 0x0200
#define GL_LESS 0x0201
#define GL_EQUAL 0x0202
#define GL_LEQUAL 0x0203
#define GL_GREATER 0x0204
#define GL_NOTEQUAL 0x0205
#define GL_GEQUAL 0x0206
#define GL_ALWAYS 0x0207
#define GL_ZERO 0
#define GL_ONE 1
#define GL_SRC_COLOR 0x0300
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_DST_ALPHA 0x0304
#define GL_ONE_MINUS_DST_ALPHA 0x0305
#define GL_DST_COLOR 0x0306
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_SRC_ALPHA_SATURATE 0x0308
#define GL_CLIP_PLANE0 0x3000
#define GL_CLIP_PLANE1 0x3001
#define GL_CLIP_PLANE2 0x3002
#define GL_CLIP_PLANE3 0x3003
#define GL_CLIP_PLANE4 0x3004
#define GL_CLIP_PLANE5 0x3005
#define GL_FRONT 0x0404
#define GL_BACK 0x0405
#define GL_FRONT_AND_BACK 0x0408
#define GL_FOG 0x0B60
#define GL_LIGHTING 0x0B50
#define GL_TEXTURE_2D 0x0DE1
#define GL_CULL_FACE 0x0B44
#define GL_ALPHA_TEST 0x0BC0
#define GL_BLEND 0x0BE2
#define GL_COLOR_LOGIC_OP 0x0BF2
#define GL_DITHER 0x0BD0
#define GL_STENCIL_TEST 0x0B90
#define GL_DEPTH_TEST 0x0B71
#define GL_POINT_SMOOTH 0x0B10
#define GL_LINE_SMOOTH 0x0B20
#define GL_SCISSOR_TEST 0x0C11
#define GL_COLOR_MATERIAL 0x0B57
#define GL_NORMALIZE 0x0BA1
#define GL_RESCALE_NORMAL 0x803A
#define GL_VERTEX_ARRAY 0x8074
#define GL_NORMAL_ARRAY 0x8075
#define GL_COLOR_ARRAY 0x8076
#define GL_TEXTURE_COORD_ARRAY 0x8078
#define GL_MULTISAMPLE 0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE 0x809E
#define GL_SAMPLE_ALPHA_TO_ONE 0x809F
#define GL_SAMPLE_COVERAGE 0x80A0
#define GL_NO_ERROR 0
#define GL_INVALID_ENUM 0x0500
#define GL_INVALID_VALUE 0x0501
#define GL_INVALID_OPERATION 0x0502
#define GL_STACK_OVERFLOW 0x0503
#define GL_STACK_UNDERFLOW 0x0504
#define GL_OUT_OF_MEMORY 0x0505
#define GL_EXP 0x0800
#define GL_EXP2 0x0801
#define GL_FOG_DENSITY 0x0B62
#define GL_FOG_START 0x0B63
#define GL_FOG_END 0x0B64
#define GL_FOG_MODE 0x0B65
#define GL_FOG_COLOR 0x0B66
#define GL_CW 0x0900
#define GL_CCW 0x0901
#define GL_CURRENT_COLOR 0x0B00
#define GL_CURRENT_NORMAL 0x0B02
#define GL_CURRENT_TEXTURE_COORDS 0x0B03
#define GL_POINT_SIZE 0x0B11
#define GL_POINT_SIZE_MIN 0x8126
#define GL_POINT_SIZE_MAX 0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE 0x8128
#define GL_POINT_DISTANCE_ATTENUATION 0x8129
#define GL_SMOOTH_POINT_SIZE_RANGE 0x0B12
#define GL_LINE_WIDTH 0x0B21
#define GL_SMOOTH_LINE_WIDTH_RANGE 0x0B22
#define GL_ALIASED_POINT_SIZE_RANGE 0x846D
#define GL_ALIASED_LINE_WIDTH_RANGE 0x846E
#define GL_CULL_FACE_MODE 0x0B45
#define GL_FRONT_FACE 0x0B46
#define GL_SHADE_MODEL 0x0B54
#define GL_DEPTH_RANGE 0x0B70
#define GL_DEPTH_WRITEMASK 0x0B72
#define GL_DEPTH_CLEAR_VALUE 0x0B73
#define GL_DEPTH_FUNC 0x0B74
#define GL_STENCIL_CLEAR_VALUE 0x0B91
#define GL_STENCIL_FUNC 0x0B92
#define GL_STENCIL_VALUE_MASK 0x0B93
#define GL_STENCIL_FAIL 0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL 0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS 0x0B96
#define GL_STENCIL_REF 0x0B97
#define GL_STENCIL_WRITEMASK 0x0B98
#define GL_MATRIX_MODE 0x0BA0
#define GL_VIEWPORT 0x0BA2
#define GL_MODELVIEW_STACK_DEPTH 0x0BA3
#define GL_PROJECTION_STACK_DEPTH 0x0BA4
#define GL_TEXTURE_STACK_DEPTH 0x0BA5
#define GL_MODELVIEW_MATRIX 0x0BA6
#define GL_PROJECTION_MATRIX 0x0BA7
#define GL_TEXTURE_MATRIX 0x0BA8
#define GL_ALPHA_TEST_FUNC 0x0BC1
#define GL_ALPHA_TEST_REF 0x0BC2
#define GL_BLEND_DST 0x0BE0
#define GL_BLEND_SRC 0x0BE1
#define GL_LOGIC_OP_MODE 0x0BF0
#define GL_SCISSOR_BOX 0x0C10
#define GL_COLOR_CLEAR_VALUE 0x0C22
#define GL_COLOR_WRITEMASK 0x0C23
#define GL_MAX_LIGHTS 0x0D31
#define GL_MAX_CLIP_PLANES 0x0D32
#define GL_MAX_TEXTURE_SIZE 0x0D33
#define GL_MAX_MODELVIEW_STACK_DEPTH 0x0D36
#define GL_MAX_PROJECTION_STACK_DEPTH 0x0D38
#define GL_MAX_TEXTURE_STACK_DEPTH 0x0D39
#define GL_MAX_VIEWPORT_DIMS 0x0D3A
#define GL_MAX_TEXTURE_UNITS 0x84E2
#define GL_SUBPIXEL_BITS 0x0D50
#define GL_RED_BITS 0x0D52
#define GL_GREEN_BITS 0x0D53
#define GL_BLUE_BITS 0x0D54
#define GL_ALPHA_BITS 0x0D55
#define GL_DEPTH_BITS 0x0D56
#define GL_STENCIL_BITS 0x0D57
#define GL_POLYGON_OFFSET_UNITS 0x2A00
#define GL_POLYGON_OFFSET_FILL 0x8037
#define GL_POLYGON_OFFSET_FACTOR 0x8038
#define GL_TEXTURE_BINDING_2D 0x8069
#define GL_VERTEX_ARRAY_SIZE 0x807A
#define GL_VERTEX_ARRAY_TYPE 0x807B
#define GL_VERTEX_ARRAY_STRIDE 0x807C
#define GL_NORMAL_ARRAY_TYPE 0x807E
#define GL_NORMAL_ARRAY_STRIDE 0x807F
#define GL_COLOR_ARRAY_SIZE 0x8081
#define GL_COLOR_ARRAY_TYPE 0x8082
#define GL_COLOR_ARRAY_STRIDE 0x8083
#define GL_TEXTURE_COORD_ARRAY_SIZE 0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE 0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE 0x808A
#define GL_VERTEX_ARRAY_POINTER 0x808E
#define GL_NORMAL_ARRAY_POINTER 0x808F
#define GL_COLOR_ARRAY_POINTER 0x8090
#define GL_TEXTURE_COORD_ARRAY_POINTER 0x8092
#define GL_SAMPLE_BUFFERS 0x80A8
#define GL_SAMPLES 0x80A9
#define GL_SAMPLE_COVERAGE_VALUE 0x80AA
#define GL_SAMPLE_COVERAGE_INVERT 0x80AB
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS 0x86A3
#define GL_DONT_CARE 0x1100
#define GL_FASTEST 0x1101
#define GL_NICEST 0x1102
#define GL_PERSPECTIVE_CORRECTION_HINT 0x0C50
#define GL_POINT_SMOOTH_HINT 0x0C51
#define GL_LINE_SMOOTH_HINT 0x0C52
#define GL_FOG_HINT 0x0C54
#define GL_GENERATE_MIPMAP_HINT 0x8192
#define GL_LIGHT_MODEL_AMBIENT 0x0B53
#define GL_LIGHT_MODEL_TWO_SIDE 0x0B52
#define GL_AMBIENT 0x1200
#define GL_DIFFUSE 0x1201
#define GL_SPECULAR 0x1202
#define GL_POSITION 0x1203
#define GL_SPOT_DIRECTION 0x1204
#define GL_SPOT_EXPONENT 0x1205
#define GL_SPOT_CUTOFF 0x1206
#define GL_CONSTANT_ATTENUATION 0x1207
#define GL_LINEAR_ATTENUATION 0x1208
#define GL_QUADRATIC_ATTENUATION 0x1209
#define GL_BYTE 0x1400
#define GL_UNSIGNED_BYTE 0x1401
#define GL_SHORT 0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_FLOAT 0x1406
#define GL_FIXED 0x140C
#define GL_CLEAR 0x1500
#define GL_AND 0x1501
#define GL_AND_REVERSE 0x1502
#define GL_COPY 0x1503
#define GL_AND_INVERTED 0x1504
#define GL_NOOP 0x1505
#define GL_XOR 0x1506
#define GL_OR 0x1507
#define GL_NOR 0x1508
#define GL_EQUIV 0x1509
#define GL_INVERT 0x150A
#define GL_OR_REVERSE 0x150B
#define GL_COPY_INVERTED 0x150C
#define GL_OR_INVERTED 0x150D
#define GL_NAND 0x150E
#define GL_SET 0x150F
#define GL_EMISSION 0x1600
#define GL_SHININESS 0x1601
#define GL_AMBIENT_AND_DIFFUSE 0x1602
#define GL_MODELVIEW 0x1700
#define GL_PROJECTION 0x1701
#define GL_TEXTURE 0x1702
#define GL_ALPHA 0x1906
#define GL_RGB 0x1907
#define GL_RGBA 0x1908
#define GL_LUMINANCE 0x1909
#define GL_LUMINANCE_ALPHA 0x190A
#define GL_UNPACK_ALIGNMENT 0x0CF5
#define GL_PACK_ALIGNMENT 0x0D05
#define GL_UNSIGNED_SHORT_4_4_4_4 0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1 0x8034
#define GL_UNSIGNED_SHORT_5_6_5 0x8363
#define GL_FLAT 0x1D00
#define GL_SMOOTH 0x1D01
#define GL_KEEP 0x1E00
#define GL_REPLACE 0x1E01
#define GL_INCR 0x1E02
#define GL_DECR 0x1E03
#define GL_VENDOR 0x1F00
#define GL_RENDERER 0x1F01
#define GL_VERSION 0x1F02
#define GL_EXTENSIONS 0x1F03
#define GL_MODULATE 0x2100
#define GL_DECAL 0x2101
#define GL_ADD 0x0104
#define GL_TEXTURE_ENV_MODE 0x2200
#define GL_TEXTURE_ENV_COLOR 0x2201
#define GL_TEXTURE_ENV 0x2300
#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_LINEAR_MIPMAP_NEAREST 0x2701
#define GL_NEAREST_MIPMAP_LINEAR 0x2702
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_GENERATE_MIPMAP 0x8191
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE3 0x84C3
#define GL_TEXTURE4 0x84C4
#define GL_TEXTURE5 0x84C5
#define GL_TEXTURE6 0x84C6
#define GL_TEXTURE7 0x84C7
#define GL_TEXTURE8 0x84C8
#define GL_TEXTURE9 0x84C9
#define GL_TEXTURE10 0x84CA
#define GL_TEXTURE11 0x84CB
#define GL_TEXTURE12 0x84CC
#define GL_TEXTURE13 0x84CD
#define GL_TEXTURE14 0x84CE
#define GL_TEXTURE15 0x84CF
#define GL_TEXTURE16 0x84D0
#define GL_TEXTURE17 0x84D1
#define GL_TEXTURE18 0x84D2
#define GL_TEXTURE19 0x84D3
#define GL_TEXTURE20 0x84D4
#define GL_TEXTURE21 0x84D5
#define GL_TEXTURE22 0x84D6
#define GL_TEXTURE23 0x84D7
#define GL_TEXTURE24 0x84D8
#define GL_TEXTURE25 0x84D9
#define GL_TEXTURE26 0x84DA
#define GL_TEXTURE27 0x84DB
#define GL_TEXTURE28 0x84DC
#define GL_TEXTURE29 0x84DD
#define GL_TEXTURE30 0x84DE
#define GL_TEXTURE31 0x84DF
#define GL_ACTIVE_TEXTURE 0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE 0x84E1
#define GL_REPEAT 0x2901
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_LIGHT0 0x4000
#define GL_LIGHT1 0x4001
#define GL_LIGHT2 0x4002
#define GL_LIGHT3 0x4003
#define GL_LIGHT4 0x4004
#define GL_LIGHT5 0x4005
#define GL_LIGHT6 0x4006
#define GL_LIGHT7 0x4007
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_ARRAY_BUFFER_BINDING 0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING 0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING 0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING 0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING 0x8898
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING 0x889A
#define GL_STATIC_DRAW 0x88E4
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_BUFFER_SIZE 0x8764
#define GL_BUFFER_USAGE 0x8765
#define GL_SUBTRACT 0x84E7
#define GL_COMBINE 0x8570
#define GL_COMBINE_RGB 0x8571
#define GL_COMBINE_ALPHA 0x8572
#define GL_RGB_SCALE 0x8573
#define GL_ADD_SIGNED 0x8574
#define GL_INTERPOLATE 0x8575
#define GL_CONSTANT 0x8576
#define GL_PRIMARY_COLOR 0x8577
#define GL_PREVIOUS 0x8578
#define GL_OPERAND0_RGB 0x8590
#define GL_OPERAND1_RGB 0x8591
#define GL_OPERAND2_RGB 0x8592
#define GL_OPERAND0_ALPHA 0x8598
#define GL_OPERAND1_ALPHA 0x8599
#define GL_OPERAND2_ALPHA 0x859A
#define GL_ALPHA_SCALE 0x0D1C
#define GL_SRC0_RGB 0x8580
#define GL_SRC1_RGB 0x8581
#define GL_SRC2_RGB 0x8582
#define GL_SRC0_ALPHA 0x8588
#define GL_SRC1_ALPHA 0x8589
#define GL_SRC2_ALPHA 0x858A
#define GL_DOT3_RGB 0x86AE
#define GL_DOT3_RGBA 0x86AF
#ifndef GL_VERSION_ES_CM_1_0
#define GL_VERSION_ES_CM_1_0 1
GLAPI int GLAD_GL_VERSION_ES_CM_1_0;
typedef void (APIENTRYP PFNGLALPHAFUNCPROC)(GLenum func, GLfloat ref);
GLAPI PFNGLALPHAFUNCPROC glad_glAlphaFunc;
#define glAlphaFunc glad_glAlphaFunc
typedef void (APIENTRYP PFNGLCLEARCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GLAPI PFNGLCLEARCOLORPROC glad_glClearColor;
#define glClearColor glad_glClearColor
typedef void (APIENTRYP PFNGLCLEARDEPTHFPROC)(GLfloat d);
GLAPI PFNGLCLEARDEPTHFPROC glad_glClearDepthf;
#define glClearDepthf glad_glClearDepthf
typedef void (APIENTRYP PFNGLCLIPPLANEFPROC)(GLenum p, const GLfloat *eqn);
GLAPI PFNGLCLIPPLANEFPROC glad_glClipPlanef;
#define glClipPlanef glad_glClipPlanef
typedef void (APIENTRYP PFNGLCOLOR4FPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GLAPI PFNGLCOLOR4FPROC glad_glColor4f;
#define glColor4f glad_glColor4f
typedef void (APIENTRYP PFNGLDEPTHRANGEFPROC)(GLfloat n, GLfloat f);
GLAPI PFNGLDEPTHRANGEFPROC glad_glDepthRangef;
#define glDepthRangef glad_glDepthRangef
typedef void (APIENTRYP PFNGLFOGFPROC)(GLenum pname, GLfloat param);
GLAPI PFNGLFOGFPROC glad_glFogf;
#define glFogf glad_glFogf
typedef void (APIENTRYP PFNGLFOGFVPROC)(GLenum pname, const GLfloat *params);
GLAPI PFNGLFOGFVPROC glad_glFogfv;
#define glFogfv glad_glFogfv
typedef void (APIENTRYP PFNGLFRUSTUMFPROC)(GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f);
GLAPI PFNGLFRUSTUMFPROC glad_glFrustumf;
#define glFrustumf glad_glFrustumf
typedef void (APIENTRYP PFNGLGETCLIPPLANEFPROC)(GLenum plane, GLfloat *equation);
GLAPI PFNGLGETCLIPPLANEFPROC glad_glGetClipPlanef;
#define glGetClipPlanef glad_glGetClipPlanef
typedef void (APIENTRYP PFNGLGETFLOATVPROC)(GLenum pname, GLfloat *data);
GLAPI PFNGLGETFLOATVPROC glad_glGetFloatv;
#define glGetFloatv glad_glGetFloatv
typedef void (APIENTRYP PFNGLGETLIGHTFVPROC)(GLenum light, GLenum pname, GLfloat *params);
GLAPI PFNGLGETLIGHTFVPROC glad_glGetLightfv;
#define glGetLightfv glad_glGetLightfv
typedef void (APIENTRYP PFNGLGETMATERIALFVPROC)(GLenum face, GLenum pname, GLfloat *params);
GLAPI PFNGLGETMATERIALFVPROC glad_glGetMaterialfv;
#define glGetMaterialfv glad_glGetMaterialfv
typedef void (APIENTRYP PFNGLGETTEXENVFVPROC)(GLenum target, GLenum pname, GLfloat *params);
GLAPI PFNGLGETTEXENVFVPROC glad_glGetTexEnvfv;
#define glGetTexEnvfv glad_glGetTexEnvfv
typedef void (APIENTRYP PFNGLGETTEXPARAMETERFVPROC)(GLenum target, GLenum pname, GLfloat *params);
GLAPI PFNGLGETTEXPARAMETERFVPROC glad_glGetTexParameterfv;
#define glGetTexParameterfv glad_glGetTexParameterfv
typedef void (APIENTRYP PFNGLLIGHTMODELFPROC)(GLenum pname, GLfloat param);
GLAPI PFNGLLIGHTMODELFPROC glad_glLightModelf;
#define glLightModelf glad_glLightModelf
typedef void (APIENTRYP PFNGLLIGHTMODELFVPROC)(GLenum pname, const GLfloat *params);
GLAPI PFNGLLIGHTMODELFVPROC glad_glLightModelfv;
#define glLightModelfv glad_glLightModelfv
typedef void (APIENTRYP PFNGLLIGHTFPROC)(GLenum light, GLenum pname, GLfloat param);
GLAPI PFNGLLIGHTFPROC glad_glLightf;
#define glLightf glad_glLightf
typedef void (APIENTRYP PFNGLLIGHTFVPROC)(GLenum light, GLenum pname, const GLfloat *params);
GLAPI PFNGLLIGHTFVPROC glad_glLightfv;
#define glLightfv glad_glLightfv
typedef void (APIENTRYP PFNGLLINEWIDTHPROC)(GLfloat width);
GLAPI PFNGLLINEWIDTHPROC glad_glLineWidth;
#define glLineWidth glad_glLineWidth
typedef void (APIENTRYP PFNGLLOADMATRIXFPROC)(const GLfloat *m);
GLAPI PFNGLLOADMATRIXFPROC glad_glLoadMatrixf;
#define glLoadMatrixf glad_glLoadMatrixf
typedef void (APIENTRYP PFNGLMATERIALFPROC)(GLenum face, GLenum pname, GLfloat param);
GLAPI PFNGLMATERIALFPROC glad_glMaterialf;
#define glMaterialf glad_glMaterialf
typedef void (APIENTRYP PFNGLMATERIALFVPROC)(GLenum face, GLenum pname, const GLfloat *params);
GLAPI PFNGLMATERIALFVPROC glad_glMaterialfv;
#define glMaterialfv glad_glMaterialfv
typedef void (APIENTRYP PFNGLMULTMATRIXFPROC)(const GLfloat *m);
GLAPI PFNGLMULTMATRIXFPROC glad_glMultMatrixf;
#define glMultMatrixf glad_glMultMatrixf
typedef void (APIENTRYP PFNGLMULTITEXCOORD4FPROC)(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
GLAPI PFNGLMULTITEXCOORD4FPROC glad_glMultiTexCoord4f;
#define glMultiTexCoord4f glad_glMultiTexCoord4f
typedef void (APIENTRYP PFNGLNORMAL3FPROC)(GLfloat nx, GLfloat ny, GLfloat nz);
GLAPI PFNGLNORMAL3FPROC glad_glNormal3f;
#define glNormal3f glad_glNormal3f
typedef void (APIENTRYP PFNGLORTHOFPROC)(GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f);
GLAPI PFNGLORTHOFPROC glad_glOrthof;
#define glOrthof glad_glOrthof
typedef void (APIENTRYP PFNGLORTHOPROC)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);

// Added manually!
GLAPI PFNGLORTHOPROC glad_glOrtho;
#define glOrtho glad_glOrtho

typedef void (APIENTRYP PFNGLPOINTPARAMETERFPROC)(GLenum pname, GLfloat param);
GLAPI PFNGLPOINTPARAMETERFPROC glad_glPointParameterf;
#define glPointParameterf glad_glPointParameterf
typedef void (APIENTRYP PFNGLPOINTPARAMETERFVPROC)(GLenum pname, const GLfloat *params);
GLAPI PFNGLPOINTPARAMETERFVPROC glad_glPointParameterfv;
#define glPointParameterfv glad_glPointParameterfv
typedef void (APIENTRYP PFNGLPOINTSIZEPROC)(GLfloat size);
GLAPI PFNGLPOINTSIZEPROC glad_glPointSize;
#define glPointSize glad_glPointSize
typedef void (APIENTRYP PFNGLPOLYGONOFFSETPROC)(GLfloat factor, GLfloat units);
GLAPI PFNGLPOLYGONOFFSETPROC glad_glPolygonOffset;
#define glPolygonOffset glad_glPolygonOffset
typedef void (APIENTRYP PFNGLROTATEFPROC)(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
GLAPI PFNGLROTATEFPROC glad_glRotatef;
#define glRotatef glad_glRotatef
typedef void (APIENTRYP PFNGLSCALEFPROC)(GLfloat x, GLfloat y, GLfloat z);
GLAPI PFNGLSCALEFPROC glad_glScalef;
#define glScalef glad_glScalef
typedef void (APIENTRYP PFNGLTEXENVFPROC)(GLenum target, GLenum pname, GLfloat param);
GLAPI PFNGLTEXENVFPROC glad_glTexEnvf;
#define glTexEnvf glad_glTexEnvf
typedef void (APIENTRYP PFNGLTEXENVFVPROC)(GLenum target, GLenum pname, const GLfloat *params);
GLAPI PFNGLTEXENVFVPROC glad_glTexEnvfv;
#define glTexEnvfv glad_glTexEnvfv
typedef void (APIENTRYP PFNGLTEXPARAMETERFPROC)(GLenum target, GLenum pname, GLfloat param);
GLAPI PFNGLTEXPARAMETERFPROC glad_glTexParameterf;
#define glTexParameterf glad_glTexParameterf
typedef void (APIENTRYP PFNGLTEXPARAMETERFVPROC)(GLenum target, GLenum pname, const GLfloat *params);
GLAPI PFNGLTEXPARAMETERFVPROC glad_glTexParameterfv;
#define glTexParameterfv glad_glTexParameterfv
typedef void (APIENTRYP PFNGLTRANSLATEFPROC)(GLfloat x, GLfloat y, GLfloat z);
GLAPI PFNGLTRANSLATEFPROC glad_glTranslatef;
#define glTranslatef glad_glTranslatef
typedef void (APIENTRYP PFNGLACTIVETEXTUREPROC)(GLenum texture);
GLAPI PFNGLACTIVETEXTUREPROC glad_glActiveTexture;
#define glActiveTexture glad_glActiveTexture
typedef void (APIENTRYP PFNGLALPHAFUNCXPROC)(GLenum func, GLfixed ref);
GLAPI PFNGLALPHAFUNCXPROC glad_glAlphaFuncx;
#define glAlphaFuncx glad_glAlphaFuncx
typedef void (APIENTRYP PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
GLAPI PFNGLBINDBUFFERPROC glad_glBindBuffer;
#define glBindBuffer glad_glBindBuffer
typedef void (APIENTRYP PFNGLBINDTEXTUREPROC)(GLenum target, GLuint texture);
GLAPI PFNGLBINDTEXTUREPROC glad_glBindTexture;
#define glBindTexture glad_glBindTexture
typedef void (APIENTRYP PFNGLBLENDFUNCPROC)(GLenum sfactor, GLenum dfactor);
GLAPI PFNGLBLENDFUNCPROC glad_glBlendFunc;
#define glBlendFunc glad_glBlendFunc
typedef void (APIENTRYP PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
GLAPI PFNGLBUFFERDATAPROC glad_glBufferData;
#define glBufferData glad_glBufferData
typedef void (APIENTRYP PFNGLBUFFERSUBDATAPROC)(GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
GLAPI PFNGLBUFFERSUBDATAPROC glad_glBufferSubData;
#define glBufferSubData glad_glBufferSubData
typedef void (APIENTRYP PFNGLCLEARPROC)(GLbitfield mask);
GLAPI PFNGLCLEARPROC glad_glClear;
#define glClear glad_glClear
typedef void (APIENTRYP PFNGLCLEARCOLORXPROC)(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);
GLAPI PFNGLCLEARCOLORXPROC glad_glClearColorx;
#define glClearColorx glad_glClearColorx
typedef void (APIENTRYP PFNGLCLEARDEPTHXPROC)(GLfixed depth);
GLAPI PFNGLCLEARDEPTHXPROC glad_glClearDepthx;
#define glClearDepthx glad_glClearDepthx
typedef void (APIENTRYP PFNGLCLEARSTENCILPROC)(GLint s);
GLAPI PFNGLCLEARSTENCILPROC glad_glClearStencil;
#define glClearStencil glad_glClearStencil
typedef void (APIENTRYP PFNGLCLIENTACTIVETEXTUREPROC)(GLenum texture);
GLAPI PFNGLCLIENTACTIVETEXTUREPROC glad_glClientActiveTexture;
#define glClientActiveTexture glad_glClientActiveTexture
typedef void (APIENTRYP PFNGLCLIPPLANEXPROC)(GLenum plane, const GLfixed *equation);
GLAPI PFNGLCLIPPLANEXPROC glad_glClipPlanex;
#define glClipPlanex glad_glClipPlanex
typedef void (APIENTRYP PFNGLCOLOR4UBPROC)(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
GLAPI PFNGLCOLOR4UBPROC glad_glColor4ub;
#define glColor4ub glad_glColor4ub
typedef void (APIENTRYP PFNGLCOLOR4XPROC)(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);
GLAPI PFNGLCOLOR4XPROC glad_glColor4x;
#define glColor4x glad_glColor4x
typedef void (APIENTRYP PFNGLCOLORMASKPROC)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
GLAPI PFNGLCOLORMASKPROC glad_glColorMask;
#define glColorMask glad_glColorMask
typedef void (APIENTRYP PFNGLCOLORPOINTERPROC)(GLint size, GLenum type, GLsizei stride, const void *pointer);
GLAPI PFNGLCOLORPOINTERPROC glad_glColorPointer;
#define glColorPointer glad_glColorPointer
typedef void (APIENTRYP PFNGLCOMPRESSEDTEXIMAGE2DPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
GLAPI PFNGLCOMPRESSEDTEXIMAGE2DPROC glad_glCompressedTexImage2D;
#define glCompressedTexImage2D glad_glCompressedTexImage2D
typedef void (APIENTRYP PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
GLAPI PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glad_glCompressedTexSubImage2D;
#define glCompressedTexSubImage2D glad_glCompressedTexSubImage2D
typedef void (APIENTRYP PFNGLCOPYTEXIMAGE2DPROC)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
GLAPI PFNGLCOPYTEXIMAGE2DPROC glad_glCopyTexImage2D;
#define glCopyTexImage2D glad_glCopyTexImage2D
typedef void (APIENTRYP PFNGLCOPYTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI PFNGLCOPYTEXSUBIMAGE2DPROC glad_glCopyTexSubImage2D;
#define glCopyTexSubImage2D glad_glCopyTexSubImage2D
typedef void (APIENTRYP PFNGLCULLFACEPROC)(GLenum mode);
GLAPI PFNGLCULLFACEPROC glad_glCullFace;
#define glCullFace glad_glCullFace
typedef void (APIENTRYP PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint *buffers);
GLAPI PFNGLDELETEBUFFERSPROC glad_glDeleteBuffers;
#define glDeleteBuffers glad_glDeleteBuffers
typedef void (APIENTRYP PFNGLDELETETEXTURESPROC)(GLsizei n, const GLuint *textures);
GLAPI PFNGLDELETETEXTURESPROC glad_glDeleteTextures;
#define glDeleteTextures glad_glDeleteTextures
typedef void (APIENTRYP PFNGLDEPTHFUNCPROC)(GLenum func);
GLAPI PFNGLDEPTHFUNCPROC glad_glDepthFunc;
#define glDepthFunc glad_glDepthFunc
typedef void (APIENTRYP PFNGLDEPTHMASKPROC)(GLboolean flag);
GLAPI PFNGLDEPTHMASKPROC glad_glDepthMask;
#define glDepthMask glad_glDepthMask
typedef void (APIENTRYP PFNGLDEPTHRANGEXPROC)(GLfixed n, GLfixed f);
GLAPI PFNGLDEPTHRANGEXPROC glad_glDepthRangex;
#define glDepthRangex glad_glDepthRangex
typedef void (APIENTRYP PFNGLDISABLEPROC)(GLenum cap);
GLAPI PFNGLDISABLEPROC glad_glDisable;
#define glDisable glad_glDisable
typedef void (APIENTRYP PFNGLDISABLECLIENTSTATEPROC)(GLenum array);
GLAPI PFNGLDISABLECLIENTSTATEPROC glad_glDisableClientState;
#define glDisableClientState glad_glDisableClientState
typedef void (APIENTRYP PFNGLDRAWARRAYSPROC)(GLenum mode, GLint first, GLsizei count);
GLAPI PFNGLDRAWARRAYSPROC glad_glDrawArrays;
#define glDrawArrays glad_glDrawArrays
typedef void (APIENTRYP PFNGLDRAWELEMENTSPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices);
GLAPI PFNGLDRAWELEMENTSPROC glad_glDrawElements;
#define glDrawElements glad_glDrawElements
typedef void (APIENTRYP PFNGLENABLEPROC)(GLenum cap);
GLAPI PFNGLENABLEPROC glad_glEnable;
#define glEnable glad_glEnable
typedef void (APIENTRYP PFNGLENABLECLIENTSTATEPROC)(GLenum array);
GLAPI PFNGLENABLECLIENTSTATEPROC glad_glEnableClientState;
#define glEnableClientState glad_glEnableClientState
typedef void (APIENTRYP PFNGLFINISHPROC)(void);
GLAPI PFNGLFINISHPROC glad_glFinish;
#define glFinish glad_glFinish
typedef void (APIENTRYP PFNGLFLUSHPROC)(void);
GLAPI PFNGLFLUSHPROC glad_glFlush;
#define glFlush glad_glFlush
typedef void (APIENTRYP PFNGLFOGXPROC)(GLenum pname, GLfixed param);
GLAPI PFNGLFOGXPROC glad_glFogx;
#define glFogx glad_glFogx
typedef void (APIENTRYP PFNGLFOGXVPROC)(GLenum pname, const GLfixed *param);
GLAPI PFNGLFOGXVPROC glad_glFogxv;
#define glFogxv glad_glFogxv
typedef void (APIENTRYP PFNGLFRONTFACEPROC)(GLenum mode);
GLAPI PFNGLFRONTFACEPROC glad_glFrontFace;
#define glFrontFace glad_glFrontFace
typedef void (APIENTRYP PFNGLFRUSTUMXPROC)(GLfixed l, GLfixed r, GLfixed b, GLfixed t, GLfixed n, GLfixed f);
GLAPI PFNGLFRUSTUMXPROC glad_glFrustumx;
#define glFrustumx glad_glFrustumx
typedef void (APIENTRYP PFNGLGETBOOLEANVPROC)(GLenum pname, GLboolean *data);
GLAPI PFNGLGETBOOLEANVPROC glad_glGetBooleanv;
#define glGetBooleanv glad_glGetBooleanv
typedef void (APIENTRYP PFNGLGETBUFFERPARAMETERIVPROC)(GLenum target, GLenum pname, GLint *params);
GLAPI PFNGLGETBUFFERPARAMETERIVPROC glad_glGetBufferParameteriv;
#define glGetBufferParameteriv glad_glGetBufferParameteriv
typedef void (APIENTRYP PFNGLGETCLIPPLANEXPROC)(GLenum plane, GLfixed *equation);
GLAPI PFNGLGETCLIPPLANEXPROC glad_glGetClipPlanex;
#define glGetClipPlanex glad_glGetClipPlanex
typedef void (APIENTRYP PFNGLGENBUFFERSPROC)(GLsizei n, GLuint *buffers);
GLAPI PFNGLGENBUFFERSPROC glad_glGenBuffers;
#define glGenBuffers glad_glGenBuffers
typedef void (APIENTRYP PFNGLGENTEXTURESPROC)(GLsizei n, GLuint *textures);
GLAPI PFNGLGENTEXTURESPROC glad_glGenTextures;
#define glGenTextures glad_glGenTextures
typedef GLenum (APIENTRYP PFNGLGETERRORPROC)(void);
GLAPI PFNGLGETERRORPROC glad_glGetError;
#define glGetError glad_glGetError
typedef void (APIENTRYP PFNGLGETFIXEDVPROC)(GLenum pname, GLfixed *params);
GLAPI PFNGLGETFIXEDVPROC glad_glGetFixedv;
#define glGetFixedv glad_glGetFixedv
typedef void (APIENTRYP PFNGLGETINTEGERVPROC)(GLenum pname, GLint *data);
GLAPI PFNGLGETINTEGERVPROC glad_glGetIntegerv;
#define glGetIntegerv glad_glGetIntegerv
typedef void (APIENTRYP PFNGLGETLIGHTXVPROC)(GLenum light, GLenum pname, GLfixed *params);
GLAPI PFNGLGETLIGHTXVPROC glad_glGetLightxv;
#define glGetLightxv glad_glGetLightxv
typedef void (APIENTRYP PFNGLGETMATERIALXVPROC)(GLenum face, GLenum pname, GLfixed *params);
GLAPI PFNGLGETMATERIALXVPROC glad_glGetMaterialxv;
#define glGetMaterialxv glad_glGetMaterialxv
typedef void (APIENTRYP PFNGLGETPOINTERVPROC)(GLenum pname, void **params);
GLAPI PFNGLGETPOINTERVPROC glad_glGetPointerv;
#define glGetPointerv glad_glGetPointerv
typedef const GLubyte * (APIENTRYP PFNGLGETSTRINGPROC)(GLenum name);
GLAPI PFNGLGETSTRINGPROC glad_glGetString;
#define glGetString glad_glGetString
typedef void (APIENTRYP PFNGLGETTEXENVIVPROC)(GLenum target, GLenum pname, GLint *params);
GLAPI PFNGLGETTEXENVIVPROC glad_glGetTexEnviv;
#define glGetTexEnviv glad_glGetTexEnviv
typedef void (APIENTRYP PFNGLGETTEXENVXVPROC)(GLenum target, GLenum pname, GLfixed *params);
GLAPI PFNGLGETTEXENVXVPROC glad_glGetTexEnvxv;
#define glGetTexEnvxv glad_glGetTexEnvxv
typedef void (APIENTRYP PFNGLGETTEXPARAMETERIVPROC)(GLenum target, GLenum pname, GLint *params);
GLAPI PFNGLGETTEXPARAMETERIVPROC glad_glGetTexParameteriv;
#define glGetTexParameteriv glad_glGetTexParameteriv
typedef void (APIENTRYP PFNGLGETTEXPARAMETERXVPROC)(GLenum target, GLenum pname, GLfixed *params);
GLAPI PFNGLGETTEXPARAMETERXVPROC glad_glGetTexParameterxv;
#define glGetTexParameterxv glad_glGetTexParameterxv
typedef void (APIENTRYP PFNGLHINTPROC)(GLenum target, GLenum mode);
GLAPI PFNGLHINTPROC glad_glHint;
#define glHint glad_glHint
typedef GLboolean (APIENTRYP PFNGLISBUFFERPROC)(GLuint buffer);
GLAPI PFNGLISBUFFERPROC glad_glIsBuffer;
#define glIsBuffer glad_glIsBuffer
typedef GLboolean (APIENTRYP PFNGLISENABLEDPROC)(GLenum cap);
GLAPI PFNGLISENABLEDPROC glad_glIsEnabled;
#define glIsEnabled glad_glIsEnabled
typedef GLboolean (APIENTRYP PFNGLISTEXTUREPROC)(GLuint texture);
GLAPI PFNGLISTEXTUREPROC glad_glIsTexture;
#define glIsTexture glad_glIsTexture
typedef void (APIENTRYP PFNGLLIGHTMODELXPROC)(GLenum pname, GLfixed param);
GLAPI PFNGLLIGHTMODELXPROC glad_glLightModelx;
#define glLightModelx glad_glLightModelx
typedef void (APIENTRYP PFNGLLIGHTMODELXVPROC)(GLenum pname, const GLfixed *param);
GLAPI PFNGLLIGHTMODELXVPROC glad_glLightModelxv;
#define glLightModelxv glad_glLightModelxv
typedef void (APIENTRYP PFNGLLIGHTXPROC)(GLenum light, GLenum pname, GLfixed param);
GLAPI PFNGLLIGHTXPROC glad_glLightx;
#define glLightx glad_glLightx
typedef void (APIENTRYP PFNGLLIGHTXVPROC)(GLenum light, GLenum pname, const GLfixed *params);
GLAPI PFNGLLIGHTXVPROC glad_glLightxv;
#define glLightxv glad_glLightxv
typedef void (APIENTRYP PFNGLLINEWIDTHXPROC)(GLfixed width);
GLAPI PFNGLLINEWIDTHXPROC glad_glLineWidthx;
#define glLineWidthx glad_glLineWidthx
typedef void (APIENTRYP PFNGLLOADIDENTITYPROC)(void);
GLAPI PFNGLLOADIDENTITYPROC glad_glLoadIdentity;
#define glLoadIdentity glad_glLoadIdentity
typedef void (APIENTRYP PFNGLLOADMATRIXXPROC)(const GLfixed *m);
GLAPI PFNGLLOADMATRIXXPROC glad_glLoadMatrixx;
#define glLoadMatrixx glad_glLoadMatrixx
typedef void (APIENTRYP PFNGLLOGICOPPROC)(GLenum opcode);
GLAPI PFNGLLOGICOPPROC glad_glLogicOp;
#define glLogicOp glad_glLogicOp
typedef void (APIENTRYP PFNGLMATERIALXPROC)(GLenum face, GLenum pname, GLfixed param);
GLAPI PFNGLMATERIALXPROC glad_glMaterialx;
#define glMaterialx glad_glMaterialx
typedef void (APIENTRYP PFNGLMATERIALXVPROC)(GLenum face, GLenum pname, const GLfixed *param);
GLAPI PFNGLMATERIALXVPROC glad_glMaterialxv;
#define glMaterialxv glad_glMaterialxv
typedef void (APIENTRYP PFNGLMATRIXMODEPROC)(GLenum mode);
GLAPI PFNGLMATRIXMODEPROC glad_glMatrixMode;
#define glMatrixMode glad_glMatrixMode
typedef void (APIENTRYP PFNGLMULTMATRIXXPROC)(const GLfixed *m);
GLAPI PFNGLMULTMATRIXXPROC glad_glMultMatrixx;
#define glMultMatrixx glad_glMultMatrixx
typedef void (APIENTRYP PFNGLMULTITEXCOORD4XPROC)(GLenum texture, GLfixed s, GLfixed t, GLfixed r, GLfixed q);
GLAPI PFNGLMULTITEXCOORD4XPROC glad_glMultiTexCoord4x;
#define glMultiTexCoord4x glad_glMultiTexCoord4x
typedef void (APIENTRYP PFNGLNORMAL3XPROC)(GLfixed nx, GLfixed ny, GLfixed nz);
GLAPI PFNGLNORMAL3XPROC glad_glNormal3x;
#define glNormal3x glad_glNormal3x
typedef void (APIENTRYP PFNGLNORMALPOINTERPROC)(GLenum type, GLsizei stride, const void *pointer);
GLAPI PFNGLNORMALPOINTERPROC glad_glNormalPointer;
#define glNormalPointer glad_glNormalPointer
typedef void (APIENTRYP PFNGLORTHOXPROC)(GLfixed l, GLfixed r, GLfixed b, GLfixed t, GLfixed n, GLfixed f);
GLAPI PFNGLORTHOXPROC glad_glOrthox;
#define glOrthox glad_glOrthox
typedef void (APIENTRYP PFNGLPIXELSTOREIPROC)(GLenum pname, GLint param);
GLAPI PFNGLPIXELSTOREIPROC glad_glPixelStorei;
#define glPixelStorei glad_glPixelStorei
typedef void (APIENTRYP PFNGLPOINTPARAMETERXPROC)(GLenum pname, GLfixed param);
GLAPI PFNGLPOINTPARAMETERXPROC glad_glPointParameterx;
#define glPointParameterx glad_glPointParameterx
typedef void (APIENTRYP PFNGLPOINTPARAMETERXVPROC)(GLenum pname, const GLfixed *params);
GLAPI PFNGLPOINTPARAMETERXVPROC glad_glPointParameterxv;
#define glPointParameterxv glad_glPointParameterxv
typedef void (APIENTRYP PFNGLPOINTSIZEXPROC)(GLfixed size);
GLAPI PFNGLPOINTSIZEXPROC glad_glPointSizex;
#define glPointSizex glad_glPointSizex
typedef void (APIENTRYP PFNGLPOLYGONOFFSETXPROC)(GLfixed factor, GLfixed units);
GLAPI PFNGLPOLYGONOFFSETXPROC glad_glPolygonOffsetx;
#define glPolygonOffsetx glad_glPolygonOffsetx
typedef void (APIENTRYP PFNGLPOPMATRIXPROC)(void);
GLAPI PFNGLPOPMATRIXPROC glad_glPopMatrix;
#define glPopMatrix glad_glPopMatrix
typedef void (APIENTRYP PFNGLPUSHMATRIXPROC)(void);
GLAPI PFNGLPUSHMATRIXPROC glad_glPushMatrix;
#define glPushMatrix glad_glPushMatrix
typedef void (APIENTRYP PFNGLREADPIXELSPROC)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
GLAPI PFNGLREADPIXELSPROC glad_glReadPixels;
#define glReadPixels glad_glReadPixels
typedef void (APIENTRYP PFNGLROTATEXPROC)(GLfixed angle, GLfixed x, GLfixed y, GLfixed z);
GLAPI PFNGLROTATEXPROC glad_glRotatex;
#define glRotatex glad_glRotatex
typedef void (APIENTRYP PFNGLSAMPLECOVERAGEPROC)(GLfloat value, GLboolean invert);
GLAPI PFNGLSAMPLECOVERAGEPROC glad_glSampleCoverage;
#define glSampleCoverage glad_glSampleCoverage
typedef void (APIENTRYP PFNGLSAMPLECOVERAGEXPROC)(GLclampx value, GLboolean invert);
GLAPI PFNGLSAMPLECOVERAGEXPROC glad_glSampleCoveragex;
#define glSampleCoveragex glad_glSampleCoveragex
typedef void (APIENTRYP PFNGLSCALEXPROC)(GLfixed x, GLfixed y, GLfixed z);
GLAPI PFNGLSCALEXPROC glad_glScalex;
#define glScalex glad_glScalex
typedef void (APIENTRYP PFNGLSCISSORPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI PFNGLSCISSORPROC glad_glScissor;
#define glScissor glad_glScissor
typedef void (APIENTRYP PFNGLSHADEMODELPROC)(GLenum mode);
GLAPI PFNGLSHADEMODELPROC glad_glShadeModel;
#define glShadeModel glad_glShadeModel
typedef void (APIENTRYP PFNGLSTENCILFUNCPROC)(GLenum func, GLint ref, GLuint mask);
GLAPI PFNGLSTENCILFUNCPROC glad_glStencilFunc;
#define glStencilFunc glad_glStencilFunc
typedef void (APIENTRYP PFNGLSTENCILMASKPROC)(GLuint mask);
GLAPI PFNGLSTENCILMASKPROC glad_glStencilMask;
#define glStencilMask glad_glStencilMask
typedef void (APIENTRYP PFNGLSTENCILOPPROC)(GLenum fail, GLenum zfail, GLenum zpass);
GLAPI PFNGLSTENCILOPPROC glad_glStencilOp;
#define glStencilOp glad_glStencilOp
typedef void (APIENTRYP PFNGLTEXCOORDPOINTERPROC)(GLint size, GLenum type, GLsizei stride, const void *pointer);
GLAPI PFNGLTEXCOORDPOINTERPROC glad_glTexCoordPointer;
#define glTexCoordPointer glad_glTexCoordPointer
typedef void (APIENTRYP PFNGLTEXENVIPROC)(GLenum target, GLenum pname, GLint param);
GLAPI PFNGLTEXENVIPROC glad_glTexEnvi;
#define glTexEnvi glad_glTexEnvi
typedef void (APIENTRYP PFNGLTEXENVXPROC)(GLenum target, GLenum pname, GLfixed param);
GLAPI PFNGLTEXENVXPROC glad_glTexEnvx;
#define glTexEnvx glad_glTexEnvx
typedef void (APIENTRYP PFNGLTEXENVIVPROC)(GLenum target, GLenum pname, const GLint *params);
GLAPI PFNGLTEXENVIVPROC glad_glTexEnviv;
#define glTexEnviv glad_glTexEnviv
typedef void (APIENTRYP PFNGLTEXENVXVPROC)(GLenum target, GLenum pname, const GLfixed *params);
GLAPI PFNGLTEXENVXVPROC glad_glTexEnvxv;
#define glTexEnvxv glad_glTexEnvxv
typedef void (APIENTRYP PFNGLTEXIMAGE2DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
GLAPI PFNGLTEXIMAGE2DPROC glad_glTexImage2D;
#define glTexImage2D glad_glTexImage2D
typedef void (APIENTRYP PFNGLTEXPARAMETERIPROC)(GLenum target, GLenum pname, GLint param);
GLAPI PFNGLTEXPARAMETERIPROC glad_glTexParameteri;
#define glTexParameteri glad_glTexParameteri
typedef void (APIENTRYP PFNGLTEXPARAMETERXPROC)(GLenum target, GLenum pname, GLfixed param);
GLAPI PFNGLTEXPARAMETERXPROC glad_glTexParameterx;
#define glTexParameterx glad_glTexParameterx
typedef void (APIENTRYP PFNGLTEXPARAMETERIVPROC)(GLenum target, GLenum pname, const GLint *params);
GLAPI PFNGLTEXPARAMETERIVPROC glad_glTexParameteriv;
#define glTexParameteriv glad_glTexParameteriv
typedef void (APIENTRYP PFNGLTEXPARAMETERXVPROC)(GLenum target, GLenum pname, const GLfixed *params);
GLAPI PFNGLTEXPARAMETERXVPROC glad_glTexParameterxv;
#define glTexParameterxv glad_glTexParameterxv
typedef void (APIENTRYP PFNGLTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
GLAPI PFNGLTEXSUBIMAGE2DPROC glad_glTexSubImage2D;
#define glTexSubImage2D glad_glTexSubImage2D
typedef void (APIENTRYP PFNGLTRANSLATEXPROC)(GLfixed x, GLfixed y, GLfixed z);
GLAPI PFNGLTRANSLATEXPROC glad_glTranslatex;
#define glTranslatex glad_glTranslatex
typedef void (APIENTRYP PFNGLVERTEXPOINTERPROC)(GLint size, GLenum type, GLsizei stride, const void *pointer);
GLAPI PFNGLVERTEXPOINTERPROC glad_glVertexPointer;
#define glVertexPointer glad_glVertexPointer
typedef void (APIENTRYP PFNGLVIEWPORTPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI PFNGLVIEWPORTPROC glad_glViewport;
#define glViewport glad_glViewport
#endif

#ifdef __cplusplus
}
#endif

#endif