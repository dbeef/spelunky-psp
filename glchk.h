#ifndef _glchk_h
#define _glchk_h

#ifndef SYS
#define SYS 0
#endif


#if !SYS

extern void __pspgl_log (const char *fmt, ...);

/* disable verbose logging to "ms0:/pspgl.ge" */
#if LOGME
	#define psp_log(x...) __pspgl_log(x)
#else
	#define psp_log(x...) do {} while (0)
#endif

/* enable GLerror logging to "ms0:/log.txt" */
#if 1
	#define EGLCHK(x)							\
	do {									\
		EGLint errcode;							\
		psp_log("%d: %s\n", __LINE__, # x );				\
		x;								\
		errcode = eglGetError();					\
		if (errcode != EGL_SUCCESS) {					\
			__pspgl_log("%s (%d): EGL error 0x%04x\n",			\
				__FUNCTION__, __LINE__, (unsigned int) errcode);\
		}								\
	} while (0)

	#define GLCHK(x)							\
	do {									\
		GLint errcode;							\
		psp_log(#x "\n");							\
		x;								\
		errcode = glGetError();						\
		if (errcode != GL_NO_ERROR) {					\
			__pspgl_log("%s (%d): GL error 0x%04x\n",			\
				__FUNCTION__, __LINE__, (unsigned int) errcode);\
		}								\
	} while (0)
#else
	#define GLCHK(x) x
	#define EGLCHK(x) x
#endif

#else
#include <stdio.h>
#if 1
	#define GLCHK(x)							\
	do {									\
		GLint errcode;							\
		x;								\
		errcode = glGetError();						\
		if (errcode != GL_NO_ERROR) {					\
			printf("%s (%d): GL error 0x%04x\n",			\
				__FUNCTION__, __LINE__, (unsigned int) errcode);\
		}								\
	} while (0)
#else
	#define GLCHK(x) x
	#define EGLCHK(x) x
#endif
#define psp_log	printf
#endif

void screenshot(const char *);

#endif /* _glchk_h */
