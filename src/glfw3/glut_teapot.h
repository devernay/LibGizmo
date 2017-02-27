#ifndef glut_teapot_h
#define glut_teapot_h

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

void glutSolidTeapot(GLdouble scale);
void glutWireTeapot(GLdouble scale);

#ifdef __cplusplus
}
#endif

#endif
