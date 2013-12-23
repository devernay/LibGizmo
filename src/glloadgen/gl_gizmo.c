#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "gl_gizmo.h"

#if defined(__APPLE__)
#include <mach-o/dyld.h>

static void* AppleGLGetProcAddress (const GLubyte *name)
{
  static const struct mach_header* image = NULL;
  NSSymbol symbol;
  char* symbolName;
  if (NULL == image)
  {
    image = NSAddImage("/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL", NSADDIMAGE_OPTION_RETURN_ON_ERROR);
  }
  /* prepend a '_' for the Unix C symbol mangling convention */
  symbolName = malloc(strlen((const char*)name) + 2);
  strcpy(symbolName+1, (const char*)name);
  symbolName[0] = '_';
  symbol = NULL;
  /* if (NSIsSymbolNameDefined(symbolName))
	 symbol = NSLookupAndBindSymbol(symbolName); */
  symbol = image ? NSLookupSymbolInImage(image, symbolName, NSLOOKUPSYMBOLINIMAGE_OPTION_BIND | NSLOOKUPSYMBOLINIMAGE_OPTION_RETURN_ON_ERROR) : NULL;
  free(symbolName);
  return symbol ? NSAddressOfSymbol(symbol) : NULL;
}
#endif /* __APPLE__ */

#if defined(__sgi) || defined (__sun)
#include <dlfcn.h>
#include <stdio.h>

static void* SunGetProcAddress (const GLubyte* name)
{
  static void* h = NULL;
  static void* gpa;

  if (h == NULL)
  {
    if ((h = dlopen(NULL, RTLD_LAZY | RTLD_LOCAL)) == NULL) return NULL;
    gpa = dlsym(h, "glXGetProcAddress");
  }

  if (gpa != NULL)
    return ((void*(*)(const GLubyte*))gpa)(name);
  else
    return dlsym(h, (const char*)name);
}
#endif /* __sgi || __sun */

#ifdef GIZMO_EGL

#include <EGL/egl.h>
#define IntGetProcAddress(name) eglGetProcAddress(name)

#elif defined(_WIN32)

#ifdef _MSC_VER
#pragma warning(disable: 4055)
#pragma warning(disable: 4054)
#endif

static int TestPointer(const PROC pTest)
{
	ptrdiff_t iTest;
	if(!pTest) return 0;
	iTest = (ptrdiff_t)pTest;
	
	if(iTest == 1 || iTest == 2 || iTest == 3 || iTest == -1) return 0;
	
	return 1;
}

static PROC WinGetProcAddress(const char *name)
{
	HMODULE glMod = NULL;
	PROC pFunc = wglGetProcAddress((LPCSTR)name);
	if(TestPointer(pFunc))
	{
		return pFunc;
	}
	glMod = GetModuleHandleA("OpenGL32.dll");
	return (PROC)GetProcAddress(glMod, (LPCSTR)name);
}
	
#define IntGetProcAddress(name) WinGetProcAddress(name)
#else
	#if defined(__APPLE__)
		#define IntGetProcAddress(name) AppleGLGetProcAddress(name)
	#else
		#if defined(__sgi) || defined(__sun)
			#define IntGetProcAddress(name) SunGetProcAddress(name)
		#else /* GLX */
		    #include <GL/glx.h>

			#define IntGetProcAddress(name) (*glXGetProcAddressARB)((const GLubyte*)name)
		#endif
	#endif
#endif

void (CODEGEN_FUNCPTR *gizmo__ptrc_glBlendFunc)(GLenum, GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glClear)(GLbitfield) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glClearColor)(GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glClearDepth)(GLdouble) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glClearStencil)(GLint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glColorMask)(GLboolean, GLboolean, GLboolean, GLboolean) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glCullFace)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDepthFunc)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDepthMask)(GLboolean) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDepthRange)(GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDisable)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDrawBuffer)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glEnable)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glFinish)() = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glFlush)() = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glFrontFace)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetBooleanv)(GLenum, GLboolean *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetDoublev)(GLenum, GLdouble *) = NULL;
GLenum (CODEGEN_FUNCPTR *gizmo__ptrc_glGetError)() = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetFloatv)(GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetIntegerv)(GLenum, GLint *) = NULL;
const GLubyte * (CODEGEN_FUNCPTR *gizmo__ptrc_glGetString)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetTexImage)(GLenum, GLint, GLenum, GLenum, GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetTexLevelParameterfv)(GLenum, GLint, GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetTexLevelParameteriv)(GLenum, GLint, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetTexParameterfv)(GLenum, GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetTexParameteriv)(GLenum, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glHint)(GLenum, GLenum) = NULL;
GLboolean (CODEGEN_FUNCPTR *gizmo__ptrc_glIsEnabled)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glLineWidth)(GLfloat) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glLogicOp)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glPixelStoref)(GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glPixelStorei)(GLenum, GLint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glPointSize)(GLfloat) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glPolygonMode)(GLenum, GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glReadBuffer)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glReadPixels)(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glScissor)(GLint, GLint, GLsizei, GLsizei) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glStencilFunc)(GLenum, GLint, GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glStencilMask)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glStencilOp)(GLenum, GLenum, GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glTexImage1D)(GLenum, GLint, GLint, GLsizei, GLint, GLenum, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glTexImage2D)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glTexParameterf)(GLenum, GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glTexParameterfv)(GLenum, GLenum, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glTexParameteri)(GLenum, GLenum, GLint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glTexParameteriv)(GLenum, GLenum, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glViewport)(GLint, GLint, GLsizei, GLsizei) = NULL;

void (CODEGEN_FUNCPTR *gizmo__ptrc_glBindTexture)(GLenum, GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glCopyTexImage1D)(GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glCopyTexImage2D)(GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLsizei, GLint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glCopyTexSubImage1D)(GLenum, GLint, GLint, GLint, GLint, GLsizei) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glCopyTexSubImage2D)(GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDeleteTextures)(GLsizei, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDrawArrays)(GLenum, GLint, GLsizei) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDrawElements)(GLenum, GLsizei, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGenTextures)(GLsizei, GLuint *) = NULL;
GLboolean (CODEGEN_FUNCPTR *gizmo__ptrc_glIsTexture)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glPolygonOffset)(GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glTexSubImage1D)(GLenum, GLint, GLint, GLsizei, GLenum, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glTexSubImage2D)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *) = NULL;

void (CODEGEN_FUNCPTR *gizmo__ptrc_glBlendColor)(GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glBlendEquation)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glCopyTexSubImage3D)(GLenum, GLint, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDrawRangeElements)(GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glTexImage3D)(GLenum, GLint, GLint, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glTexSubImage3D)(GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *) = NULL;

void (CODEGEN_FUNCPTR *gizmo__ptrc_glActiveTexture)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glCompressedTexImage1D)(GLenum, GLint, GLenum, GLsizei, GLint, GLsizei, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glCompressedTexImage2D)(GLenum, GLint, GLenum, GLsizei, GLsizei, GLint, GLsizei, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glCompressedTexImage3D)(GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint, GLsizei, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glCompressedTexSubImage1D)(GLenum, GLint, GLint, GLsizei, GLenum, GLsizei, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glCompressedTexSubImage2D)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glCompressedTexSubImage3D)(GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetCompressedTexImage)(GLenum, GLint, GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glSampleCoverage)(GLfloat, GLboolean) = NULL;

void (CODEGEN_FUNCPTR *gizmo__ptrc_glBlendFuncSeparate)(GLenum, GLenum, GLenum, GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glMultiDrawArrays)(GLenum, const GLint *, const GLsizei *, GLsizei) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glMultiDrawElements)(GLenum, const GLsizei *, GLenum, const GLvoid *const*, GLsizei) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glPointParameterf)(GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glPointParameterfv)(GLenum, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glPointParameteri)(GLenum, GLint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glPointParameteriv)(GLenum, const GLint *) = NULL;

void (CODEGEN_FUNCPTR *gizmo__ptrc_glBeginQuery)(GLenum, GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glBindBuffer)(GLenum, GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glBufferData)(GLenum, GLsizeiptr, const GLvoid *, GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glBufferSubData)(GLenum, GLintptr, GLsizeiptr, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDeleteBuffers)(GLsizei, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDeleteQueries)(GLsizei, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glEndQuery)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGenBuffers)(GLsizei, GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGenQueries)(GLsizei, GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetBufferParameteriv)(GLenum, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetBufferPointerv)(GLenum, GLenum, GLvoid **) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetBufferSubData)(GLenum, GLintptr, GLsizeiptr, GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetQueryObjectiv)(GLuint, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetQueryObjectuiv)(GLuint, GLenum, GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetQueryiv)(GLenum, GLenum, GLint *) = NULL;
GLboolean (CODEGEN_FUNCPTR *gizmo__ptrc_glIsBuffer)(GLuint) = NULL;
GLboolean (CODEGEN_FUNCPTR *gizmo__ptrc_glIsQuery)(GLuint) = NULL;
void * (CODEGEN_FUNCPTR *gizmo__ptrc_glMapBuffer)(GLenum, GLenum) = NULL;
GLboolean (CODEGEN_FUNCPTR *gizmo__ptrc_glUnmapBuffer)(GLenum) = NULL;

void (CODEGEN_FUNCPTR *gizmo__ptrc_glAttachShader)(GLuint, GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glBindAttribLocation)(GLuint, GLuint, const GLchar *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glBlendEquationSeparate)(GLenum, GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glCompileShader)(GLuint) = NULL;
GLuint (CODEGEN_FUNCPTR *gizmo__ptrc_glCreateProgram)() = NULL;
GLuint (CODEGEN_FUNCPTR *gizmo__ptrc_glCreateShader)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDeleteProgram)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDeleteShader)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDetachShader)(GLuint, GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDisableVertexAttribArray)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDrawBuffers)(GLsizei, const GLenum *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glEnableVertexAttribArray)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetActiveAttrib)(GLuint, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLchar *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetActiveUniform)(GLuint, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLchar *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetAttachedShaders)(GLuint, GLsizei, GLsizei *, GLuint *) = NULL;
GLint (CODEGEN_FUNCPTR *gizmo__ptrc_glGetAttribLocation)(GLuint, const GLchar *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetProgramInfoLog)(GLuint, GLsizei, GLsizei *, GLchar *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetProgramiv)(GLuint, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetShaderInfoLog)(GLuint, GLsizei, GLsizei *, GLchar *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetShaderSource)(GLuint, GLsizei, GLsizei *, GLchar *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetShaderiv)(GLuint, GLenum, GLint *) = NULL;
GLint (CODEGEN_FUNCPTR *gizmo__ptrc_glGetUniformLocation)(GLuint, const GLchar *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetUniformfv)(GLuint, GLint, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetUniformiv)(GLuint, GLint, GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetVertexAttribPointerv)(GLuint, GLenum, GLvoid **) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetVertexAttribdv)(GLuint, GLenum, GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetVertexAttribfv)(GLuint, GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetVertexAttribiv)(GLuint, GLenum, GLint *) = NULL;
GLboolean (CODEGEN_FUNCPTR *gizmo__ptrc_glIsProgram)(GLuint) = NULL;
GLboolean (CODEGEN_FUNCPTR *gizmo__ptrc_glIsShader)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glLinkProgram)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glShaderSource)(GLuint, GLsizei, const GLchar *const*, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glStencilFuncSeparate)(GLenum, GLenum, GLint, GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glStencilMaskSeparate)(GLenum, GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glStencilOpSeparate)(GLenum, GLenum, GLenum, GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniform1f)(GLint, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniform1fv)(GLint, GLsizei, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniform1i)(GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniform1iv)(GLint, GLsizei, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniform2f)(GLint, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniform2fv)(GLint, GLsizei, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniform2i)(GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniform2iv)(GLint, GLsizei, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniform3f)(GLint, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniform3fv)(GLint, GLsizei, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniform3i)(GLint, GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniform3iv)(GLint, GLsizei, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniform4f)(GLint, GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniform4fv)(GLint, GLsizei, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniform4i)(GLint, GLint, GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniform4iv)(GLint, GLsizei, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniformMatrix2fv)(GLint, GLsizei, GLboolean, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniformMatrix3fv)(GLint, GLsizei, GLboolean, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniformMatrix4fv)(GLint, GLsizei, GLboolean, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUseProgram)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glValidateProgram)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib1d)(GLuint, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib1dv)(GLuint, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib1f)(GLuint, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib1fv)(GLuint, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib1s)(GLuint, GLshort) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib1sv)(GLuint, const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib2d)(GLuint, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib2dv)(GLuint, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib2f)(GLuint, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib2fv)(GLuint, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib2s)(GLuint, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib2sv)(GLuint, const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib3d)(GLuint, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib3dv)(GLuint, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib3f)(GLuint, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib3fv)(GLuint, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib3s)(GLuint, GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib3sv)(GLuint, const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib4Nbv)(GLuint, const GLbyte *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib4Niv)(GLuint, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib4Nsv)(GLuint, const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib4Nub)(GLuint, GLubyte, GLubyte, GLubyte, GLubyte) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib4Nubv)(GLuint, const GLubyte *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib4Nuiv)(GLuint, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib4Nusv)(GLuint, const GLushort *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib4bv)(GLuint, const GLbyte *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib4d)(GLuint, GLdouble, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib4dv)(GLuint, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib4f)(GLuint, GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib4fv)(GLuint, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib4iv)(GLuint, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib4s)(GLuint, GLshort, GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib4sv)(GLuint, const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib4ubv)(GLuint, const GLubyte *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib4uiv)(GLuint, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttrib4usv)(GLuint, const GLushort *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttribPointer)(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid *) = NULL;

void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniformMatrix2x3fv)(GLint, GLsizei, GLboolean, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniformMatrix2x4fv)(GLint, GLsizei, GLboolean, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniformMatrix3x2fv)(GLint, GLsizei, GLboolean, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniformMatrix3x4fv)(GLint, GLsizei, GLboolean, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniformMatrix4x2fv)(GLint, GLsizei, GLboolean, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniformMatrix4x3fv)(GLint, GLsizei, GLboolean, const GLfloat *) = NULL;

void (CODEGEN_FUNCPTR *gizmo__ptrc_glBeginConditionalRender)(GLuint, GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glBeginTransformFeedback)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glBindBufferBase)(GLenum, GLuint, GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glBindBufferRange)(GLenum, GLuint, GLuint, GLintptr, GLsizeiptr) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glBindFragDataLocation)(GLuint, GLuint, const GLchar *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glBindFramebuffer)(GLenum, GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glBindRenderbuffer)(GLenum, GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glBindVertexArray)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glBlitFramebuffer)(GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum) = NULL;
GLenum (CODEGEN_FUNCPTR *gizmo__ptrc_glCheckFramebufferStatus)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glClampColor)(GLenum, GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glClearBufferfi)(GLenum, GLint, GLfloat, GLint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glClearBufferfv)(GLenum, GLint, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glClearBufferiv)(GLenum, GLint, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glClearBufferuiv)(GLenum, GLint, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glColorMaski)(GLuint, GLboolean, GLboolean, GLboolean, GLboolean) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDeleteFramebuffers)(GLsizei, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDeleteRenderbuffers)(GLsizei, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDeleteVertexArrays)(GLsizei, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDisablei)(GLenum, GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glEnablei)(GLenum, GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glEndConditionalRender)() = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glEndTransformFeedback)() = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glFlushMappedBufferRange)(GLenum, GLintptr, GLsizeiptr) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glFramebufferRenderbuffer)(GLenum, GLenum, GLenum, GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glFramebufferTexture1D)(GLenum, GLenum, GLenum, GLuint, GLint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glFramebufferTexture2D)(GLenum, GLenum, GLenum, GLuint, GLint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glFramebufferTexture3D)(GLenum, GLenum, GLenum, GLuint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glFramebufferTextureLayer)(GLenum, GLenum, GLuint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGenFramebuffers)(GLsizei, GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGenRenderbuffers)(GLsizei, GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGenVertexArrays)(GLsizei, GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGenerateMipmap)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetBooleani_v)(GLenum, GLuint, GLboolean *) = NULL;
GLint (CODEGEN_FUNCPTR *gizmo__ptrc_glGetFragDataLocation)(GLuint, const GLchar *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetFramebufferAttachmentParameteriv)(GLenum, GLenum, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetIntegeri_v)(GLenum, GLuint, GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetRenderbufferParameteriv)(GLenum, GLenum, GLint *) = NULL;
const GLubyte * (CODEGEN_FUNCPTR *gizmo__ptrc_glGetStringi)(GLenum, GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetTexParameterIiv)(GLenum, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetTexParameterIuiv)(GLenum, GLenum, GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetTransformFeedbackVarying)(GLuint, GLuint, GLsizei, GLsizei *, GLsizei *, GLenum *, GLchar *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetUniformuiv)(GLuint, GLint, GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetVertexAttribIiv)(GLuint, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetVertexAttribIuiv)(GLuint, GLenum, GLuint *) = NULL;
GLboolean (CODEGEN_FUNCPTR *gizmo__ptrc_glIsEnabledi)(GLenum, GLuint) = NULL;
GLboolean (CODEGEN_FUNCPTR *gizmo__ptrc_glIsFramebuffer)(GLuint) = NULL;
GLboolean (CODEGEN_FUNCPTR *gizmo__ptrc_glIsRenderbuffer)(GLuint) = NULL;
GLboolean (CODEGEN_FUNCPTR *gizmo__ptrc_glIsVertexArray)(GLuint) = NULL;
void * (CODEGEN_FUNCPTR *gizmo__ptrc_glMapBufferRange)(GLenum, GLintptr, GLsizeiptr, GLbitfield) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glRenderbufferStorage)(GLenum, GLenum, GLsizei, GLsizei) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glRenderbufferStorageMultisample)(GLenum, GLsizei, GLenum, GLsizei, GLsizei) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glTexParameterIiv)(GLenum, GLenum, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glTexParameterIuiv)(GLenum, GLenum, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glTransformFeedbackVaryings)(GLuint, GLsizei, const GLchar *const*, GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniform1ui)(GLint, GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniform1uiv)(GLint, GLsizei, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniform2ui)(GLint, GLuint, GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniform2uiv)(GLint, GLsizei, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniform3ui)(GLint, GLuint, GLuint, GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniform3uiv)(GLint, GLsizei, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniform4ui)(GLint, GLuint, GLuint, GLuint, GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniform4uiv)(GLint, GLsizei, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttribI1i)(GLuint, GLint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttribI1iv)(GLuint, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttribI1ui)(GLuint, GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttribI1uiv)(GLuint, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttribI2i)(GLuint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttribI2iv)(GLuint, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttribI2ui)(GLuint, GLuint, GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttribI2uiv)(GLuint, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttribI3i)(GLuint, GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttribI3iv)(GLuint, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttribI3ui)(GLuint, GLuint, GLuint, GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttribI3uiv)(GLuint, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttribI4bv)(GLuint, const GLbyte *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttribI4i)(GLuint, GLint, GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttribI4iv)(GLuint, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttribI4sv)(GLuint, const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttribI4ubv)(GLuint, const GLubyte *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttribI4ui)(GLuint, GLuint, GLuint, GLuint, GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttribI4uiv)(GLuint, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttribI4usv)(GLuint, const GLushort *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glVertexAttribIPointer)(GLuint, GLint, GLenum, GLsizei, const GLvoid *) = NULL;

void (CODEGEN_FUNCPTR *gizmo__ptrc_glCopyBufferSubData)(GLenum, GLenum, GLintptr, GLintptr, GLsizeiptr) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDrawArraysInstanced)(GLenum, GLint, GLsizei, GLsizei) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDrawElementsInstanced)(GLenum, GLsizei, GLenum, const GLvoid *, GLsizei) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetActiveUniformBlockName)(GLuint, GLuint, GLsizei, GLsizei *, GLchar *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetActiveUniformBlockiv)(GLuint, GLuint, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetActiveUniformName)(GLuint, GLuint, GLsizei, GLsizei *, GLchar *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetActiveUniformsiv)(GLuint, GLsizei, const GLuint *, GLenum, GLint *) = NULL;
GLuint (CODEGEN_FUNCPTR *gizmo__ptrc_glGetUniformBlockIndex)(GLuint, const GLchar *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetUniformIndices)(GLuint, GLsizei, const GLchar *const*, GLuint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glPrimitiveRestartIndex)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glTexBuffer)(GLenum, GLenum, GLuint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glUniformBlockBinding)(GLuint, GLuint, GLuint) = NULL;

GLenum (CODEGEN_FUNCPTR *gizmo__ptrc_glClientWaitSync)(GLsync, GLbitfield, GLuint64) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDeleteSync)(GLsync) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDrawElementsBaseVertex)(GLenum, GLsizei, GLenum, const GLvoid *, GLint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDrawElementsInstancedBaseVertex)(GLenum, GLsizei, GLenum, const GLvoid *, GLsizei, GLint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glDrawRangeElementsBaseVertex)(GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid *, GLint) = NULL;
GLsync (CODEGEN_FUNCPTR *gizmo__ptrc_glFenceSync)(GLenum, GLbitfield) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glFramebufferTexture)(GLenum, GLenum, GLuint, GLint) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetBufferParameteri64v)(GLenum, GLenum, GLint64 *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetInteger64i_v)(GLenum, GLuint, GLint64 *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetInteger64v)(GLenum, GLint64 *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetMultisamplefv)(GLenum, GLuint, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glGetSynciv)(GLsync, GLenum, GLsizei, GLsizei *, GLint *) = NULL;
GLboolean (CODEGEN_FUNCPTR *gizmo__ptrc_glIsSync)(GLsync) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glMultiDrawElementsBaseVertex)(GLenum, const GLsizei *, GLenum, const GLvoid *const*, GLsizei, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glProvokingVertex)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glSampleMaski)(GLuint, GLbitfield) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glTexImage2DMultisample)(GLenum, GLsizei, GLint, GLsizei, GLsizei, GLboolean) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glTexImage3DMultisample)(GLenum, GLsizei, GLint, GLsizei, GLsizei, GLsizei, GLboolean) = NULL;
void (CODEGEN_FUNCPTR *gizmo__ptrc_glWaitSync)(GLsync, GLbitfield, GLuint64) = NULL;

static int Load_Version_3_2()
{
  int numFailed = 0;
  gizmo__ptrc_glBlendFunc = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum))IntGetProcAddress("glBlendFunc");
  if(!gizmo__ptrc_glBlendFunc) numFailed++;
  gizmo__ptrc_glClear = (void (CODEGEN_FUNCPTR *)(GLbitfield))IntGetProcAddress("glClear");
  if(!gizmo__ptrc_glClear) numFailed++;
  gizmo__ptrc_glClearColor = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glClearColor");
  if(!gizmo__ptrc_glClearColor) numFailed++;
  gizmo__ptrc_glClearDepth = (void (CODEGEN_FUNCPTR *)(GLdouble))IntGetProcAddress("glClearDepth");
  if(!gizmo__ptrc_glClearDepth) numFailed++;
  gizmo__ptrc_glClearStencil = (void (CODEGEN_FUNCPTR *)(GLint))IntGetProcAddress("glClearStencil");
  if(!gizmo__ptrc_glClearStencil) numFailed++;
  gizmo__ptrc_glColorMask = (void (CODEGEN_FUNCPTR *)(GLboolean, GLboolean, GLboolean, GLboolean))IntGetProcAddress("glColorMask");
  if(!gizmo__ptrc_glColorMask) numFailed++;
  gizmo__ptrc_glCullFace = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glCullFace");
  if(!gizmo__ptrc_glCullFace) numFailed++;
  gizmo__ptrc_glDepthFunc = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glDepthFunc");
  if(!gizmo__ptrc_glDepthFunc) numFailed++;
  gizmo__ptrc_glDepthMask = (void (CODEGEN_FUNCPTR *)(GLboolean))IntGetProcAddress("glDepthMask");
  if(!gizmo__ptrc_glDepthMask) numFailed++;
  gizmo__ptrc_glDepthRange = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble))IntGetProcAddress("glDepthRange");
  if(!gizmo__ptrc_glDepthRange) numFailed++;
  gizmo__ptrc_glDisable = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glDisable");
  if(!gizmo__ptrc_glDisable) numFailed++;
  gizmo__ptrc_glDrawBuffer = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glDrawBuffer");
  if(!gizmo__ptrc_glDrawBuffer) numFailed++;
  gizmo__ptrc_glEnable = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glEnable");
  if(!gizmo__ptrc_glEnable) numFailed++;
  gizmo__ptrc_glFinish = (void (CODEGEN_FUNCPTR *)())IntGetProcAddress("glFinish");
  if(!gizmo__ptrc_glFinish) numFailed++;
  gizmo__ptrc_glFlush = (void (CODEGEN_FUNCPTR *)())IntGetProcAddress("glFlush");
  if(!gizmo__ptrc_glFlush) numFailed++;
  gizmo__ptrc_glFrontFace = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glFrontFace");
  if(!gizmo__ptrc_glFrontFace) numFailed++;
  gizmo__ptrc_glGetBooleanv = (void (CODEGEN_FUNCPTR *)(GLenum, GLboolean *))IntGetProcAddress("glGetBooleanv");
  if(!gizmo__ptrc_glGetBooleanv) numFailed++;
  gizmo__ptrc_glGetDoublev = (void (CODEGEN_FUNCPTR *)(GLenum, GLdouble *))IntGetProcAddress("glGetDoublev");
  if(!gizmo__ptrc_glGetDoublev) numFailed++;
  gizmo__ptrc_glGetError = (GLenum (CODEGEN_FUNCPTR *)())IntGetProcAddress("glGetError");
  if(!gizmo__ptrc_glGetError) numFailed++;
  gizmo__ptrc_glGetFloatv = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat *))IntGetProcAddress("glGetFloatv");
  if(!gizmo__ptrc_glGetFloatv) numFailed++;
  gizmo__ptrc_glGetIntegerv = (void (CODEGEN_FUNCPTR *)(GLenum, GLint *))IntGetProcAddress("glGetIntegerv");
  if(!gizmo__ptrc_glGetIntegerv) numFailed++;
  gizmo__ptrc_glGetString = (const GLubyte * (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glGetString");
  if(!gizmo__ptrc_glGetString) numFailed++;
  gizmo__ptrc_glGetTexImage = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLenum, GLenum, GLvoid *))IntGetProcAddress("glGetTexImage");
  if(!gizmo__ptrc_glGetTexImage) numFailed++;
  gizmo__ptrc_glGetTexLevelParameterfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLenum, GLfloat *))IntGetProcAddress("glGetTexLevelParameterfv");
  if(!gizmo__ptrc_glGetTexLevelParameterfv) numFailed++;
  gizmo__ptrc_glGetTexLevelParameteriv = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLenum, GLint *))IntGetProcAddress("glGetTexLevelParameteriv");
  if(!gizmo__ptrc_glGetTexLevelParameteriv) numFailed++;
  gizmo__ptrc_glGetTexParameterfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLfloat *))IntGetProcAddress("glGetTexParameterfv");
  if(!gizmo__ptrc_glGetTexParameterfv) numFailed++;
  gizmo__ptrc_glGetTexParameteriv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint *))IntGetProcAddress("glGetTexParameteriv");
  if(!gizmo__ptrc_glGetTexParameteriv) numFailed++;
  gizmo__ptrc_glHint = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum))IntGetProcAddress("glHint");
  if(!gizmo__ptrc_glHint) numFailed++;
  gizmo__ptrc_glIsEnabled = (GLboolean (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glIsEnabled");
  if(!gizmo__ptrc_glIsEnabled) numFailed++;
  gizmo__ptrc_glLineWidth = (void (CODEGEN_FUNCPTR *)(GLfloat))IntGetProcAddress("glLineWidth");
  if(!gizmo__ptrc_glLineWidth) numFailed++;
  gizmo__ptrc_glLogicOp = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glLogicOp");
  if(!gizmo__ptrc_glLogicOp) numFailed++;
  gizmo__ptrc_glPixelStoref = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat))IntGetProcAddress("glPixelStoref");
  if(!gizmo__ptrc_glPixelStoref) numFailed++;
  gizmo__ptrc_glPixelStorei = (void (CODEGEN_FUNCPTR *)(GLenum, GLint))IntGetProcAddress("glPixelStorei");
  if(!gizmo__ptrc_glPixelStorei) numFailed++;
  gizmo__ptrc_glPointSize = (void (CODEGEN_FUNCPTR *)(GLfloat))IntGetProcAddress("glPointSize");
  if(!gizmo__ptrc_glPointSize) numFailed++;
  gizmo__ptrc_glPolygonMode = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum))IntGetProcAddress("glPolygonMode");
  if(!gizmo__ptrc_glPolygonMode) numFailed++;
  gizmo__ptrc_glReadBuffer = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glReadBuffer");
  if(!gizmo__ptrc_glReadBuffer) numFailed++;
  gizmo__ptrc_glReadPixels = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLvoid *))IntGetProcAddress("glReadPixels");
  if(!gizmo__ptrc_glReadPixels) numFailed++;
  gizmo__ptrc_glScissor = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLsizei, GLsizei))IntGetProcAddress("glScissor");
  if(!gizmo__ptrc_glScissor) numFailed++;
  gizmo__ptrc_glStencilFunc = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLuint))IntGetProcAddress("glStencilFunc");
  if(!gizmo__ptrc_glStencilFunc) numFailed++;
  gizmo__ptrc_glStencilMask = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glStencilMask");
  if(!gizmo__ptrc_glStencilMask) numFailed++;
  gizmo__ptrc_glStencilOp = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLenum))IntGetProcAddress("glStencilOp");
  if(!gizmo__ptrc_glStencilOp) numFailed++;
  gizmo__ptrc_glTexImage1D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLsizei, GLint, GLenum, GLenum, const GLvoid *))IntGetProcAddress("glTexImage1D");
  if(!gizmo__ptrc_glTexImage1D) numFailed++;
  gizmo__ptrc_glTexImage2D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *))IntGetProcAddress("glTexImage2D");
  if(!gizmo__ptrc_glTexImage2D) numFailed++;
  gizmo__ptrc_glTexParameterf = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLfloat))IntGetProcAddress("glTexParameterf");
  if(!gizmo__ptrc_glTexParameterf) numFailed++;
  gizmo__ptrc_glTexParameterfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLfloat *))IntGetProcAddress("glTexParameterfv");
  if(!gizmo__ptrc_glTexParameterfv) numFailed++;
  gizmo__ptrc_glTexParameteri = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint))IntGetProcAddress("glTexParameteri");
  if(!gizmo__ptrc_glTexParameteri) numFailed++;
  gizmo__ptrc_glTexParameteriv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLint *))IntGetProcAddress("glTexParameteriv");
  if(!gizmo__ptrc_glTexParameteriv) numFailed++;
  gizmo__ptrc_glViewport = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLsizei, GLsizei))IntGetProcAddress("glViewport");
  if(!gizmo__ptrc_glViewport) numFailed++;
  gizmo__ptrc_glBindTexture = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint))IntGetProcAddress("glBindTexture");
  if(!gizmo__ptrc_glBindTexture) numFailed++;
  gizmo__ptrc_glCopyTexImage1D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLint))IntGetProcAddress("glCopyTexImage1D");
  if(!gizmo__ptrc_glCopyTexImage1D) numFailed++;
  gizmo__ptrc_glCopyTexImage2D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLsizei, GLint))IntGetProcAddress("glCopyTexImage2D");
  if(!gizmo__ptrc_glCopyTexImage2D) numFailed++;
  gizmo__ptrc_glCopyTexSubImage1D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLint, GLint, GLsizei))IntGetProcAddress("glCopyTexSubImage1D");
  if(!gizmo__ptrc_glCopyTexSubImage1D) numFailed++;
  gizmo__ptrc_glCopyTexSubImage2D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei))IntGetProcAddress("glCopyTexSubImage2D");
  if(!gizmo__ptrc_glCopyTexSubImage2D) numFailed++;
  gizmo__ptrc_glDeleteTextures = (void (CODEGEN_FUNCPTR *)(GLsizei, const GLuint *))IntGetProcAddress("glDeleteTextures");
  if(!gizmo__ptrc_glDeleteTextures) numFailed++;
  gizmo__ptrc_glDrawArrays = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLsizei))IntGetProcAddress("glDrawArrays");
  if(!gizmo__ptrc_glDrawArrays) numFailed++;
  gizmo__ptrc_glDrawElements = (void (CODEGEN_FUNCPTR *)(GLenum, GLsizei, GLenum, const GLvoid *))IntGetProcAddress("glDrawElements");
  if(!gizmo__ptrc_glDrawElements) numFailed++;
  gizmo__ptrc_glGenTextures = (void (CODEGEN_FUNCPTR *)(GLsizei, GLuint *))IntGetProcAddress("glGenTextures");
  if(!gizmo__ptrc_glGenTextures) numFailed++;
  gizmo__ptrc_glIsTexture = (GLboolean (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glIsTexture");
  if(!gizmo__ptrc_glIsTexture) numFailed++;
  gizmo__ptrc_glPolygonOffset = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat))IntGetProcAddress("glPolygonOffset");
  if(!gizmo__ptrc_glPolygonOffset) numFailed++;
  gizmo__ptrc_glTexSubImage1D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLsizei, GLenum, GLenum, const GLvoid *))IntGetProcAddress("glTexSubImage1D");
  if(!gizmo__ptrc_glTexSubImage1D) numFailed++;
  gizmo__ptrc_glTexSubImage2D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *))IntGetProcAddress("glTexSubImage2D");
  if(!gizmo__ptrc_glTexSubImage2D) numFailed++;
  gizmo__ptrc_glBlendColor = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glBlendColor");
  if(!gizmo__ptrc_glBlendColor) numFailed++;
  gizmo__ptrc_glBlendEquation = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glBlendEquation");
  if(!gizmo__ptrc_glBlendEquation) numFailed++;
  gizmo__ptrc_glCopyTexSubImage3D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei))IntGetProcAddress("glCopyTexSubImage3D");
  if(!gizmo__ptrc_glCopyTexSubImage3D) numFailed++;
  gizmo__ptrc_glDrawRangeElements = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid *))IntGetProcAddress("glDrawRangeElements");
  if(!gizmo__ptrc_glDrawRangeElements) numFailed++;
  gizmo__ptrc_glTexImage3D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *))IntGetProcAddress("glTexImage3D");
  if(!gizmo__ptrc_glTexImage3D) numFailed++;
  gizmo__ptrc_glTexSubImage3D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *))IntGetProcAddress("glTexSubImage3D");
  if(!gizmo__ptrc_glTexSubImage3D) numFailed++;
  gizmo__ptrc_glActiveTexture = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glActiveTexture");
  if(!gizmo__ptrc_glActiveTexture) numFailed++;
  gizmo__ptrc_glCompressedTexImage1D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLenum, GLsizei, GLint, GLsizei, const GLvoid *))IntGetProcAddress("glCompressedTexImage1D");
  if(!gizmo__ptrc_glCompressedTexImage1D) numFailed++;
  gizmo__ptrc_glCompressedTexImage2D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLenum, GLsizei, GLsizei, GLint, GLsizei, const GLvoid *))IntGetProcAddress("glCompressedTexImage2D");
  if(!gizmo__ptrc_glCompressedTexImage2D) numFailed++;
  gizmo__ptrc_glCompressedTexImage3D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint, GLsizei, const GLvoid *))IntGetProcAddress("glCompressedTexImage3D");
  if(!gizmo__ptrc_glCompressedTexImage3D) numFailed++;
  gizmo__ptrc_glCompressedTexSubImage1D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLsizei, GLenum, GLsizei, const GLvoid *))IntGetProcAddress("glCompressedTexSubImage1D");
  if(!gizmo__ptrc_glCompressedTexSubImage1D) numFailed++;
  gizmo__ptrc_glCompressedTexSubImage2D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid *))IntGetProcAddress("glCompressedTexSubImage2D");
  if(!gizmo__ptrc_glCompressedTexSubImage2D) numFailed++;
  gizmo__ptrc_glCompressedTexSubImage3D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid *))IntGetProcAddress("glCompressedTexSubImage3D");
  if(!gizmo__ptrc_glCompressedTexSubImage3D) numFailed++;
  gizmo__ptrc_glGetCompressedTexImage = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLvoid *))IntGetProcAddress("glGetCompressedTexImage");
  if(!gizmo__ptrc_glGetCompressedTexImage) numFailed++;
  gizmo__ptrc_glSampleCoverage = (void (CODEGEN_FUNCPTR *)(GLfloat, GLboolean))IntGetProcAddress("glSampleCoverage");
  if(!gizmo__ptrc_glSampleCoverage) numFailed++;
  gizmo__ptrc_glBlendFuncSeparate = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLenum, GLenum))IntGetProcAddress("glBlendFuncSeparate");
  if(!gizmo__ptrc_glBlendFuncSeparate) numFailed++;
  gizmo__ptrc_glMultiDrawArrays = (void (CODEGEN_FUNCPTR *)(GLenum, const GLint *, const GLsizei *, GLsizei))IntGetProcAddress("glMultiDrawArrays");
  if(!gizmo__ptrc_glMultiDrawArrays) numFailed++;
  gizmo__ptrc_glMultiDrawElements = (void (CODEGEN_FUNCPTR *)(GLenum, const GLsizei *, GLenum, const GLvoid *const*, GLsizei))IntGetProcAddress("glMultiDrawElements");
  if(!gizmo__ptrc_glMultiDrawElements) numFailed++;
  gizmo__ptrc_glPointParameterf = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat))IntGetProcAddress("glPointParameterf");
  if(!gizmo__ptrc_glPointParameterf) numFailed++;
  gizmo__ptrc_glPointParameterfv = (void (CODEGEN_FUNCPTR *)(GLenum, const GLfloat *))IntGetProcAddress("glPointParameterfv");
  if(!gizmo__ptrc_glPointParameterfv) numFailed++;
  gizmo__ptrc_glPointParameteri = (void (CODEGEN_FUNCPTR *)(GLenum, GLint))IntGetProcAddress("glPointParameteri");
  if(!gizmo__ptrc_glPointParameteri) numFailed++;
  gizmo__ptrc_glPointParameteriv = (void (CODEGEN_FUNCPTR *)(GLenum, const GLint *))IntGetProcAddress("glPointParameteriv");
  if(!gizmo__ptrc_glPointParameteriv) numFailed++;
  gizmo__ptrc_glBeginQuery = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint))IntGetProcAddress("glBeginQuery");
  if(!gizmo__ptrc_glBeginQuery) numFailed++;
  gizmo__ptrc_glBindBuffer = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint))IntGetProcAddress("glBindBuffer");
  if(!gizmo__ptrc_glBindBuffer) numFailed++;
  gizmo__ptrc_glBufferData = (void (CODEGEN_FUNCPTR *)(GLenum, GLsizeiptr, const GLvoid *, GLenum))IntGetProcAddress("glBufferData");
  if(!gizmo__ptrc_glBufferData) numFailed++;
  gizmo__ptrc_glBufferSubData = (void (CODEGEN_FUNCPTR *)(GLenum, GLintptr, GLsizeiptr, const GLvoid *))IntGetProcAddress("glBufferSubData");
  if(!gizmo__ptrc_glBufferSubData) numFailed++;
  gizmo__ptrc_glDeleteBuffers = (void (CODEGEN_FUNCPTR *)(GLsizei, const GLuint *))IntGetProcAddress("glDeleteBuffers");
  if(!gizmo__ptrc_glDeleteBuffers) numFailed++;
  gizmo__ptrc_glDeleteQueries = (void (CODEGEN_FUNCPTR *)(GLsizei, const GLuint *))IntGetProcAddress("glDeleteQueries");
  if(!gizmo__ptrc_glDeleteQueries) numFailed++;
  gizmo__ptrc_glEndQuery = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glEndQuery");
  if(!gizmo__ptrc_glEndQuery) numFailed++;
  gizmo__ptrc_glGenBuffers = (void (CODEGEN_FUNCPTR *)(GLsizei, GLuint *))IntGetProcAddress("glGenBuffers");
  if(!gizmo__ptrc_glGenBuffers) numFailed++;
  gizmo__ptrc_glGenQueries = (void (CODEGEN_FUNCPTR *)(GLsizei, GLuint *))IntGetProcAddress("glGenQueries");
  if(!gizmo__ptrc_glGenQueries) numFailed++;
  gizmo__ptrc_glGetBufferParameteriv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint *))IntGetProcAddress("glGetBufferParameteriv");
  if(!gizmo__ptrc_glGetBufferParameteriv) numFailed++;
  gizmo__ptrc_glGetBufferPointerv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLvoid **))IntGetProcAddress("glGetBufferPointerv");
  if(!gizmo__ptrc_glGetBufferPointerv) numFailed++;
  gizmo__ptrc_glGetBufferSubData = (void (CODEGEN_FUNCPTR *)(GLenum, GLintptr, GLsizeiptr, GLvoid *))IntGetProcAddress("glGetBufferSubData");
  if(!gizmo__ptrc_glGetBufferSubData) numFailed++;
  gizmo__ptrc_glGetQueryObjectiv = (void (CODEGEN_FUNCPTR *)(GLuint, GLenum, GLint *))IntGetProcAddress("glGetQueryObjectiv");
  if(!gizmo__ptrc_glGetQueryObjectiv) numFailed++;
  gizmo__ptrc_glGetQueryObjectuiv = (void (CODEGEN_FUNCPTR *)(GLuint, GLenum, GLuint *))IntGetProcAddress("glGetQueryObjectuiv");
  if(!gizmo__ptrc_glGetQueryObjectuiv) numFailed++;
  gizmo__ptrc_glGetQueryiv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint *))IntGetProcAddress("glGetQueryiv");
  if(!gizmo__ptrc_glGetQueryiv) numFailed++;
  gizmo__ptrc_glIsBuffer = (GLboolean (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glIsBuffer");
  if(!gizmo__ptrc_glIsBuffer) numFailed++;
  gizmo__ptrc_glIsQuery = (GLboolean (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glIsQuery");
  if(!gizmo__ptrc_glIsQuery) numFailed++;
  gizmo__ptrc_glMapBuffer = (void * (CODEGEN_FUNCPTR *)(GLenum, GLenum))IntGetProcAddress("glMapBuffer");
  if(!gizmo__ptrc_glMapBuffer) numFailed++;
  gizmo__ptrc_glUnmapBuffer = (GLboolean (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glUnmapBuffer");
  if(!gizmo__ptrc_glUnmapBuffer) numFailed++;
  gizmo__ptrc_glAttachShader = (void (CODEGEN_FUNCPTR *)(GLuint, GLuint))IntGetProcAddress("glAttachShader");
  if(!gizmo__ptrc_glAttachShader) numFailed++;
  gizmo__ptrc_glBindAttribLocation = (void (CODEGEN_FUNCPTR *)(GLuint, GLuint, const GLchar *))IntGetProcAddress("glBindAttribLocation");
  if(!gizmo__ptrc_glBindAttribLocation) numFailed++;
  gizmo__ptrc_glBlendEquationSeparate = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum))IntGetProcAddress("glBlendEquationSeparate");
  if(!gizmo__ptrc_glBlendEquationSeparate) numFailed++;
  gizmo__ptrc_glCompileShader = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glCompileShader");
  if(!gizmo__ptrc_glCompileShader) numFailed++;
  gizmo__ptrc_glCreateProgram = (GLuint (CODEGEN_FUNCPTR *)())IntGetProcAddress("glCreateProgram");
  if(!gizmo__ptrc_glCreateProgram) numFailed++;
  gizmo__ptrc_glCreateShader = (GLuint (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glCreateShader");
  if(!gizmo__ptrc_glCreateShader) numFailed++;
  gizmo__ptrc_glDeleteProgram = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glDeleteProgram");
  if(!gizmo__ptrc_glDeleteProgram) numFailed++;
  gizmo__ptrc_glDeleteShader = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glDeleteShader");
  if(!gizmo__ptrc_glDeleteShader) numFailed++;
  gizmo__ptrc_glDetachShader = (void (CODEGEN_FUNCPTR *)(GLuint, GLuint))IntGetProcAddress("glDetachShader");
  if(!gizmo__ptrc_glDetachShader) numFailed++;
  gizmo__ptrc_glDisableVertexAttribArray = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glDisableVertexAttribArray");
  if(!gizmo__ptrc_glDisableVertexAttribArray) numFailed++;
  gizmo__ptrc_glDrawBuffers = (void (CODEGEN_FUNCPTR *)(GLsizei, const GLenum *))IntGetProcAddress("glDrawBuffers");
  if(!gizmo__ptrc_glDrawBuffers) numFailed++;
  gizmo__ptrc_glEnableVertexAttribArray = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glEnableVertexAttribArray");
  if(!gizmo__ptrc_glEnableVertexAttribArray) numFailed++;
  gizmo__ptrc_glGetActiveAttrib = (void (CODEGEN_FUNCPTR *)(GLuint, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLchar *))IntGetProcAddress("glGetActiveAttrib");
  if(!gizmo__ptrc_glGetActiveAttrib) numFailed++;
  gizmo__ptrc_glGetActiveUniform = (void (CODEGEN_FUNCPTR *)(GLuint, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLchar *))IntGetProcAddress("glGetActiveUniform");
  if(!gizmo__ptrc_glGetActiveUniform) numFailed++;
  gizmo__ptrc_glGetAttachedShaders = (void (CODEGEN_FUNCPTR *)(GLuint, GLsizei, GLsizei *, GLuint *))IntGetProcAddress("glGetAttachedShaders");
  if(!gizmo__ptrc_glGetAttachedShaders) numFailed++;
  gizmo__ptrc_glGetAttribLocation = (GLint (CODEGEN_FUNCPTR *)(GLuint, const GLchar *))IntGetProcAddress("glGetAttribLocation");
  if(!gizmo__ptrc_glGetAttribLocation) numFailed++;
  gizmo__ptrc_glGetProgramInfoLog = (void (CODEGEN_FUNCPTR *)(GLuint, GLsizei, GLsizei *, GLchar *))IntGetProcAddress("glGetProgramInfoLog");
  if(!gizmo__ptrc_glGetProgramInfoLog) numFailed++;
  gizmo__ptrc_glGetProgramiv = (void (CODEGEN_FUNCPTR *)(GLuint, GLenum, GLint *))IntGetProcAddress("glGetProgramiv");
  if(!gizmo__ptrc_glGetProgramiv) numFailed++;
  gizmo__ptrc_glGetShaderInfoLog = (void (CODEGEN_FUNCPTR *)(GLuint, GLsizei, GLsizei *, GLchar *))IntGetProcAddress("glGetShaderInfoLog");
  if(!gizmo__ptrc_glGetShaderInfoLog) numFailed++;
  gizmo__ptrc_glGetShaderSource = (void (CODEGEN_FUNCPTR *)(GLuint, GLsizei, GLsizei *, GLchar *))IntGetProcAddress("glGetShaderSource");
  if(!gizmo__ptrc_glGetShaderSource) numFailed++;
  gizmo__ptrc_glGetShaderiv = (void (CODEGEN_FUNCPTR *)(GLuint, GLenum, GLint *))IntGetProcAddress("glGetShaderiv");
  if(!gizmo__ptrc_glGetShaderiv) numFailed++;
  gizmo__ptrc_glGetUniformLocation = (GLint (CODEGEN_FUNCPTR *)(GLuint, const GLchar *))IntGetProcAddress("glGetUniformLocation");
  if(!gizmo__ptrc_glGetUniformLocation) numFailed++;
  gizmo__ptrc_glGetUniformfv = (void (CODEGEN_FUNCPTR *)(GLuint, GLint, GLfloat *))IntGetProcAddress("glGetUniformfv");
  if(!gizmo__ptrc_glGetUniformfv) numFailed++;
  gizmo__ptrc_glGetUniformiv = (void (CODEGEN_FUNCPTR *)(GLuint, GLint, GLint *))IntGetProcAddress("glGetUniformiv");
  if(!gizmo__ptrc_glGetUniformiv) numFailed++;
  gizmo__ptrc_glGetVertexAttribPointerv = (void (CODEGEN_FUNCPTR *)(GLuint, GLenum, GLvoid **))IntGetProcAddress("glGetVertexAttribPointerv");
  if(!gizmo__ptrc_glGetVertexAttribPointerv) numFailed++;
  gizmo__ptrc_glGetVertexAttribdv = (void (CODEGEN_FUNCPTR *)(GLuint, GLenum, GLdouble *))IntGetProcAddress("glGetVertexAttribdv");
  if(!gizmo__ptrc_glGetVertexAttribdv) numFailed++;
  gizmo__ptrc_glGetVertexAttribfv = (void (CODEGEN_FUNCPTR *)(GLuint, GLenum, GLfloat *))IntGetProcAddress("glGetVertexAttribfv");
  if(!gizmo__ptrc_glGetVertexAttribfv) numFailed++;
  gizmo__ptrc_glGetVertexAttribiv = (void (CODEGEN_FUNCPTR *)(GLuint, GLenum, GLint *))IntGetProcAddress("glGetVertexAttribiv");
  if(!gizmo__ptrc_glGetVertexAttribiv) numFailed++;
  gizmo__ptrc_glIsProgram = (GLboolean (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glIsProgram");
  if(!gizmo__ptrc_glIsProgram) numFailed++;
  gizmo__ptrc_glIsShader = (GLboolean (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glIsShader");
  if(!gizmo__ptrc_glIsShader) numFailed++;
  gizmo__ptrc_glLinkProgram = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glLinkProgram");
  if(!gizmo__ptrc_glLinkProgram) numFailed++;
  gizmo__ptrc_glShaderSource = (void (CODEGEN_FUNCPTR *)(GLuint, GLsizei, const GLchar *const*, const GLint *))IntGetProcAddress("glShaderSource");
  if(!gizmo__ptrc_glShaderSource) numFailed++;
  gizmo__ptrc_glStencilFuncSeparate = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint, GLuint))IntGetProcAddress("glStencilFuncSeparate");
  if(!gizmo__ptrc_glStencilFuncSeparate) numFailed++;
  gizmo__ptrc_glStencilMaskSeparate = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint))IntGetProcAddress("glStencilMaskSeparate");
  if(!gizmo__ptrc_glStencilMaskSeparate) numFailed++;
  gizmo__ptrc_glStencilOpSeparate = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLenum, GLenum))IntGetProcAddress("glStencilOpSeparate");
  if(!gizmo__ptrc_glStencilOpSeparate) numFailed++;
  gizmo__ptrc_glUniform1f = (void (CODEGEN_FUNCPTR *)(GLint, GLfloat))IntGetProcAddress("glUniform1f");
  if(!gizmo__ptrc_glUniform1f) numFailed++;
  gizmo__ptrc_glUniform1fv = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLfloat *))IntGetProcAddress("glUniform1fv");
  if(!gizmo__ptrc_glUniform1fv) numFailed++;
  gizmo__ptrc_glUniform1i = (void (CODEGEN_FUNCPTR *)(GLint, GLint))IntGetProcAddress("glUniform1i");
  if(!gizmo__ptrc_glUniform1i) numFailed++;
  gizmo__ptrc_glUniform1iv = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLint *))IntGetProcAddress("glUniform1iv");
  if(!gizmo__ptrc_glUniform1iv) numFailed++;
  gizmo__ptrc_glUniform2f = (void (CODEGEN_FUNCPTR *)(GLint, GLfloat, GLfloat))IntGetProcAddress("glUniform2f");
  if(!gizmo__ptrc_glUniform2f) numFailed++;
  gizmo__ptrc_glUniform2fv = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLfloat *))IntGetProcAddress("glUniform2fv");
  if(!gizmo__ptrc_glUniform2fv) numFailed++;
  gizmo__ptrc_glUniform2i = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint))IntGetProcAddress("glUniform2i");
  if(!gizmo__ptrc_glUniform2i) numFailed++;
  gizmo__ptrc_glUniform2iv = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLint *))IntGetProcAddress("glUniform2iv");
  if(!gizmo__ptrc_glUniform2iv) numFailed++;
  gizmo__ptrc_glUniform3f = (void (CODEGEN_FUNCPTR *)(GLint, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glUniform3f");
  if(!gizmo__ptrc_glUniform3f) numFailed++;
  gizmo__ptrc_glUniform3fv = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLfloat *))IntGetProcAddress("glUniform3fv");
  if(!gizmo__ptrc_glUniform3fv) numFailed++;
  gizmo__ptrc_glUniform3i = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint, GLint))IntGetProcAddress("glUniform3i");
  if(!gizmo__ptrc_glUniform3i) numFailed++;
  gizmo__ptrc_glUniform3iv = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLint *))IntGetProcAddress("glUniform3iv");
  if(!gizmo__ptrc_glUniform3iv) numFailed++;
  gizmo__ptrc_glUniform4f = (void (CODEGEN_FUNCPTR *)(GLint, GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glUniform4f");
  if(!gizmo__ptrc_glUniform4f) numFailed++;
  gizmo__ptrc_glUniform4fv = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLfloat *))IntGetProcAddress("glUniform4fv");
  if(!gizmo__ptrc_glUniform4fv) numFailed++;
  gizmo__ptrc_glUniform4i = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint, GLint, GLint))IntGetProcAddress("glUniform4i");
  if(!gizmo__ptrc_glUniform4i) numFailed++;
  gizmo__ptrc_glUniform4iv = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLint *))IntGetProcAddress("glUniform4iv");
  if(!gizmo__ptrc_glUniform4iv) numFailed++;
  gizmo__ptrc_glUniformMatrix2fv = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, GLboolean, const GLfloat *))IntGetProcAddress("glUniformMatrix2fv");
  if(!gizmo__ptrc_glUniformMatrix2fv) numFailed++;
  gizmo__ptrc_glUniformMatrix3fv = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, GLboolean, const GLfloat *))IntGetProcAddress("glUniformMatrix3fv");
  if(!gizmo__ptrc_glUniformMatrix3fv) numFailed++;
  gizmo__ptrc_glUniformMatrix4fv = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, GLboolean, const GLfloat *))IntGetProcAddress("glUniformMatrix4fv");
  if(!gizmo__ptrc_glUniformMatrix4fv) numFailed++;
  gizmo__ptrc_glUseProgram = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glUseProgram");
  if(!gizmo__ptrc_glUseProgram) numFailed++;
  gizmo__ptrc_glValidateProgram = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glValidateProgram");
  if(!gizmo__ptrc_glValidateProgram) numFailed++;
  gizmo__ptrc_glVertexAttrib1d = (void (CODEGEN_FUNCPTR *)(GLuint, GLdouble))IntGetProcAddress("glVertexAttrib1d");
  if(!gizmo__ptrc_glVertexAttrib1d) numFailed++;
  gizmo__ptrc_glVertexAttrib1dv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLdouble *))IntGetProcAddress("glVertexAttrib1dv");
  if(!gizmo__ptrc_glVertexAttrib1dv) numFailed++;
  gizmo__ptrc_glVertexAttrib1f = (void (CODEGEN_FUNCPTR *)(GLuint, GLfloat))IntGetProcAddress("glVertexAttrib1f");
  if(!gizmo__ptrc_glVertexAttrib1f) numFailed++;
  gizmo__ptrc_glVertexAttrib1fv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLfloat *))IntGetProcAddress("glVertexAttrib1fv");
  if(!gizmo__ptrc_glVertexAttrib1fv) numFailed++;
  gizmo__ptrc_glVertexAttrib1s = (void (CODEGEN_FUNCPTR *)(GLuint, GLshort))IntGetProcAddress("glVertexAttrib1s");
  if(!gizmo__ptrc_glVertexAttrib1s) numFailed++;
  gizmo__ptrc_glVertexAttrib1sv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLshort *))IntGetProcAddress("glVertexAttrib1sv");
  if(!gizmo__ptrc_glVertexAttrib1sv) numFailed++;
  gizmo__ptrc_glVertexAttrib2d = (void (CODEGEN_FUNCPTR *)(GLuint, GLdouble, GLdouble))IntGetProcAddress("glVertexAttrib2d");
  if(!gizmo__ptrc_glVertexAttrib2d) numFailed++;
  gizmo__ptrc_glVertexAttrib2dv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLdouble *))IntGetProcAddress("glVertexAttrib2dv");
  if(!gizmo__ptrc_glVertexAttrib2dv) numFailed++;
  gizmo__ptrc_glVertexAttrib2f = (void (CODEGEN_FUNCPTR *)(GLuint, GLfloat, GLfloat))IntGetProcAddress("glVertexAttrib2f");
  if(!gizmo__ptrc_glVertexAttrib2f) numFailed++;
  gizmo__ptrc_glVertexAttrib2fv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLfloat *))IntGetProcAddress("glVertexAttrib2fv");
  if(!gizmo__ptrc_glVertexAttrib2fv) numFailed++;
  gizmo__ptrc_glVertexAttrib2s = (void (CODEGEN_FUNCPTR *)(GLuint, GLshort, GLshort))IntGetProcAddress("glVertexAttrib2s");
  if(!gizmo__ptrc_glVertexAttrib2s) numFailed++;
  gizmo__ptrc_glVertexAttrib2sv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLshort *))IntGetProcAddress("glVertexAttrib2sv");
  if(!gizmo__ptrc_glVertexAttrib2sv) numFailed++;
  gizmo__ptrc_glVertexAttrib3d = (void (CODEGEN_FUNCPTR *)(GLuint, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glVertexAttrib3d");
  if(!gizmo__ptrc_glVertexAttrib3d) numFailed++;
  gizmo__ptrc_glVertexAttrib3dv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLdouble *))IntGetProcAddress("glVertexAttrib3dv");
  if(!gizmo__ptrc_glVertexAttrib3dv) numFailed++;
  gizmo__ptrc_glVertexAttrib3f = (void (CODEGEN_FUNCPTR *)(GLuint, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glVertexAttrib3f");
  if(!gizmo__ptrc_glVertexAttrib3f) numFailed++;
  gizmo__ptrc_glVertexAttrib3fv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLfloat *))IntGetProcAddress("glVertexAttrib3fv");
  if(!gizmo__ptrc_glVertexAttrib3fv) numFailed++;
  gizmo__ptrc_glVertexAttrib3s = (void (CODEGEN_FUNCPTR *)(GLuint, GLshort, GLshort, GLshort))IntGetProcAddress("glVertexAttrib3s");
  if(!gizmo__ptrc_glVertexAttrib3s) numFailed++;
  gizmo__ptrc_glVertexAttrib3sv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLshort *))IntGetProcAddress("glVertexAttrib3sv");
  if(!gizmo__ptrc_glVertexAttrib3sv) numFailed++;
  gizmo__ptrc_glVertexAttrib4Nbv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLbyte *))IntGetProcAddress("glVertexAttrib4Nbv");
  if(!gizmo__ptrc_glVertexAttrib4Nbv) numFailed++;
  gizmo__ptrc_glVertexAttrib4Niv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLint *))IntGetProcAddress("glVertexAttrib4Niv");
  if(!gizmo__ptrc_glVertexAttrib4Niv) numFailed++;
  gizmo__ptrc_glVertexAttrib4Nsv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLshort *))IntGetProcAddress("glVertexAttrib4Nsv");
  if(!gizmo__ptrc_glVertexAttrib4Nsv) numFailed++;
  gizmo__ptrc_glVertexAttrib4Nub = (void (CODEGEN_FUNCPTR *)(GLuint, GLubyte, GLubyte, GLubyte, GLubyte))IntGetProcAddress("glVertexAttrib4Nub");
  if(!gizmo__ptrc_glVertexAttrib4Nub) numFailed++;
  gizmo__ptrc_glVertexAttrib4Nubv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLubyte *))IntGetProcAddress("glVertexAttrib4Nubv");
  if(!gizmo__ptrc_glVertexAttrib4Nubv) numFailed++;
  gizmo__ptrc_glVertexAttrib4Nuiv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLuint *))IntGetProcAddress("glVertexAttrib4Nuiv");
  if(!gizmo__ptrc_glVertexAttrib4Nuiv) numFailed++;
  gizmo__ptrc_glVertexAttrib4Nusv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLushort *))IntGetProcAddress("glVertexAttrib4Nusv");
  if(!gizmo__ptrc_glVertexAttrib4Nusv) numFailed++;
  gizmo__ptrc_glVertexAttrib4bv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLbyte *))IntGetProcAddress("glVertexAttrib4bv");
  if(!gizmo__ptrc_glVertexAttrib4bv) numFailed++;
  gizmo__ptrc_glVertexAttrib4d = (void (CODEGEN_FUNCPTR *)(GLuint, GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glVertexAttrib4d");
  if(!gizmo__ptrc_glVertexAttrib4d) numFailed++;
  gizmo__ptrc_glVertexAttrib4dv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLdouble *))IntGetProcAddress("glVertexAttrib4dv");
  if(!gizmo__ptrc_glVertexAttrib4dv) numFailed++;
  gizmo__ptrc_glVertexAttrib4f = (void (CODEGEN_FUNCPTR *)(GLuint, GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glVertexAttrib4f");
  if(!gizmo__ptrc_glVertexAttrib4f) numFailed++;
  gizmo__ptrc_glVertexAttrib4fv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLfloat *))IntGetProcAddress("glVertexAttrib4fv");
  if(!gizmo__ptrc_glVertexAttrib4fv) numFailed++;
  gizmo__ptrc_glVertexAttrib4iv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLint *))IntGetProcAddress("glVertexAttrib4iv");
  if(!gizmo__ptrc_glVertexAttrib4iv) numFailed++;
  gizmo__ptrc_glVertexAttrib4s = (void (CODEGEN_FUNCPTR *)(GLuint, GLshort, GLshort, GLshort, GLshort))IntGetProcAddress("glVertexAttrib4s");
  if(!gizmo__ptrc_glVertexAttrib4s) numFailed++;
  gizmo__ptrc_glVertexAttrib4sv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLshort *))IntGetProcAddress("glVertexAttrib4sv");
  if(!gizmo__ptrc_glVertexAttrib4sv) numFailed++;
  gizmo__ptrc_glVertexAttrib4ubv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLubyte *))IntGetProcAddress("glVertexAttrib4ubv");
  if(!gizmo__ptrc_glVertexAttrib4ubv) numFailed++;
  gizmo__ptrc_glVertexAttrib4uiv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLuint *))IntGetProcAddress("glVertexAttrib4uiv");
  if(!gizmo__ptrc_glVertexAttrib4uiv) numFailed++;
  gizmo__ptrc_glVertexAttrib4usv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLushort *))IntGetProcAddress("glVertexAttrib4usv");
  if(!gizmo__ptrc_glVertexAttrib4usv) numFailed++;
  gizmo__ptrc_glVertexAttribPointer = (void (CODEGEN_FUNCPTR *)(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid *))IntGetProcAddress("glVertexAttribPointer");
  if(!gizmo__ptrc_glVertexAttribPointer) numFailed++;
  gizmo__ptrc_glUniformMatrix2x3fv = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, GLboolean, const GLfloat *))IntGetProcAddress("glUniformMatrix2x3fv");
  if(!gizmo__ptrc_glUniformMatrix2x3fv) numFailed++;
  gizmo__ptrc_glUniformMatrix2x4fv = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, GLboolean, const GLfloat *))IntGetProcAddress("glUniformMatrix2x4fv");
  if(!gizmo__ptrc_glUniformMatrix2x4fv) numFailed++;
  gizmo__ptrc_glUniformMatrix3x2fv = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, GLboolean, const GLfloat *))IntGetProcAddress("glUniformMatrix3x2fv");
  if(!gizmo__ptrc_glUniformMatrix3x2fv) numFailed++;
  gizmo__ptrc_glUniformMatrix3x4fv = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, GLboolean, const GLfloat *))IntGetProcAddress("glUniformMatrix3x4fv");
  if(!gizmo__ptrc_glUniformMatrix3x4fv) numFailed++;
  gizmo__ptrc_glUniformMatrix4x2fv = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, GLboolean, const GLfloat *))IntGetProcAddress("glUniformMatrix4x2fv");
  if(!gizmo__ptrc_glUniformMatrix4x2fv) numFailed++;
  gizmo__ptrc_glUniformMatrix4x3fv = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, GLboolean, const GLfloat *))IntGetProcAddress("glUniformMatrix4x3fv");
  if(!gizmo__ptrc_glUniformMatrix4x3fv) numFailed++;
  gizmo__ptrc_glBeginConditionalRender = (void (CODEGEN_FUNCPTR *)(GLuint, GLenum))IntGetProcAddress("glBeginConditionalRender");
  if(!gizmo__ptrc_glBeginConditionalRender) numFailed++;
  gizmo__ptrc_glBeginTransformFeedback = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glBeginTransformFeedback");
  if(!gizmo__ptrc_glBeginTransformFeedback) numFailed++;
  gizmo__ptrc_glBindBufferBase = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLuint))IntGetProcAddress("glBindBufferBase");
  if(!gizmo__ptrc_glBindBufferBase) numFailed++;
  gizmo__ptrc_glBindBufferRange = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLuint, GLintptr, GLsizeiptr))IntGetProcAddress("glBindBufferRange");
  if(!gizmo__ptrc_glBindBufferRange) numFailed++;
  gizmo__ptrc_glBindFragDataLocation = (void (CODEGEN_FUNCPTR *)(GLuint, GLuint, const GLchar *))IntGetProcAddress("glBindFragDataLocation");
  if(!gizmo__ptrc_glBindFragDataLocation) numFailed++;
  gizmo__ptrc_glBindFramebuffer = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint))IntGetProcAddress("glBindFramebuffer");
  if(!gizmo__ptrc_glBindFramebuffer) numFailed++;
  gizmo__ptrc_glBindRenderbuffer = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint))IntGetProcAddress("glBindRenderbuffer");
  if(!gizmo__ptrc_glBindRenderbuffer) numFailed++;
  gizmo__ptrc_glBindVertexArray = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glBindVertexArray");
  if(!gizmo__ptrc_glBindVertexArray) numFailed++;
  gizmo__ptrc_glBlitFramebuffer = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum))IntGetProcAddress("glBlitFramebuffer");
  if(!gizmo__ptrc_glBlitFramebuffer) numFailed++;
  gizmo__ptrc_glCheckFramebufferStatus = (GLenum (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glCheckFramebufferStatus");
  if(!gizmo__ptrc_glCheckFramebufferStatus) numFailed++;
  gizmo__ptrc_glClampColor = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum))IntGetProcAddress("glClampColor");
  if(!gizmo__ptrc_glClampColor) numFailed++;
  gizmo__ptrc_glClearBufferfi = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLfloat, GLint))IntGetProcAddress("glClearBufferfi");
  if(!gizmo__ptrc_glClearBufferfi) numFailed++;
  gizmo__ptrc_glClearBufferfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, const GLfloat *))IntGetProcAddress("glClearBufferfv");
  if(!gizmo__ptrc_glClearBufferfv) numFailed++;
  gizmo__ptrc_glClearBufferiv = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, const GLint *))IntGetProcAddress("glClearBufferiv");
  if(!gizmo__ptrc_glClearBufferiv) numFailed++;
  gizmo__ptrc_glClearBufferuiv = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, const GLuint *))IntGetProcAddress("glClearBufferuiv");
  if(!gizmo__ptrc_glClearBufferuiv) numFailed++;
  gizmo__ptrc_glColorMaski = (void (CODEGEN_FUNCPTR *)(GLuint, GLboolean, GLboolean, GLboolean, GLboolean))IntGetProcAddress("glColorMaski");
  if(!gizmo__ptrc_glColorMaski) numFailed++;
  gizmo__ptrc_glDeleteFramebuffers = (void (CODEGEN_FUNCPTR *)(GLsizei, const GLuint *))IntGetProcAddress("glDeleteFramebuffers");
  if(!gizmo__ptrc_glDeleteFramebuffers) numFailed++;
  gizmo__ptrc_glDeleteRenderbuffers = (void (CODEGEN_FUNCPTR *)(GLsizei, const GLuint *))IntGetProcAddress("glDeleteRenderbuffers");
  if(!gizmo__ptrc_glDeleteRenderbuffers) numFailed++;
  gizmo__ptrc_glDeleteVertexArrays = (void (CODEGEN_FUNCPTR *)(GLsizei, const GLuint *))IntGetProcAddress("glDeleteVertexArrays");
  if(!gizmo__ptrc_glDeleteVertexArrays) numFailed++;
  gizmo__ptrc_glDisablei = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint))IntGetProcAddress("glDisablei");
  if(!gizmo__ptrc_glDisablei) numFailed++;
  gizmo__ptrc_glEnablei = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint))IntGetProcAddress("glEnablei");
  if(!gizmo__ptrc_glEnablei) numFailed++;
  gizmo__ptrc_glEndConditionalRender = (void (CODEGEN_FUNCPTR *)())IntGetProcAddress("glEndConditionalRender");
  if(!gizmo__ptrc_glEndConditionalRender) numFailed++;
  gizmo__ptrc_glEndTransformFeedback = (void (CODEGEN_FUNCPTR *)())IntGetProcAddress("glEndTransformFeedback");
  if(!gizmo__ptrc_glEndTransformFeedback) numFailed++;
  gizmo__ptrc_glFlushMappedBufferRange = (void (CODEGEN_FUNCPTR *)(GLenum, GLintptr, GLsizeiptr))IntGetProcAddress("glFlushMappedBufferRange");
  if(!gizmo__ptrc_glFlushMappedBufferRange) numFailed++;
  gizmo__ptrc_glFramebufferRenderbuffer = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLenum, GLuint))IntGetProcAddress("glFramebufferRenderbuffer");
  if(!gizmo__ptrc_glFramebufferRenderbuffer) numFailed++;
  gizmo__ptrc_glFramebufferTexture1D = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLenum, GLuint, GLint))IntGetProcAddress("glFramebufferTexture1D");
  if(!gizmo__ptrc_glFramebufferTexture1D) numFailed++;
  gizmo__ptrc_glFramebufferTexture2D = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLenum, GLuint, GLint))IntGetProcAddress("glFramebufferTexture2D");
  if(!gizmo__ptrc_glFramebufferTexture2D) numFailed++;
  gizmo__ptrc_glFramebufferTexture3D = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLenum, GLuint, GLint, GLint))IntGetProcAddress("glFramebufferTexture3D");
  if(!gizmo__ptrc_glFramebufferTexture3D) numFailed++;
  gizmo__ptrc_glFramebufferTextureLayer = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLuint, GLint, GLint))IntGetProcAddress("glFramebufferTextureLayer");
  if(!gizmo__ptrc_glFramebufferTextureLayer) numFailed++;
  gizmo__ptrc_glGenFramebuffers = (void (CODEGEN_FUNCPTR *)(GLsizei, GLuint *))IntGetProcAddress("glGenFramebuffers");
  if(!gizmo__ptrc_glGenFramebuffers) numFailed++;
  gizmo__ptrc_glGenRenderbuffers = (void (CODEGEN_FUNCPTR *)(GLsizei, GLuint *))IntGetProcAddress("glGenRenderbuffers");
  if(!gizmo__ptrc_glGenRenderbuffers) numFailed++;
  gizmo__ptrc_glGenVertexArrays = (void (CODEGEN_FUNCPTR *)(GLsizei, GLuint *))IntGetProcAddress("glGenVertexArrays");
  if(!gizmo__ptrc_glGenVertexArrays) numFailed++;
  gizmo__ptrc_glGenerateMipmap = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glGenerateMipmap");
  if(!gizmo__ptrc_glGenerateMipmap) numFailed++;
  gizmo__ptrc_glGetBooleani_v = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLboolean *))IntGetProcAddress("glGetBooleani_v");
  if(!gizmo__ptrc_glGetBooleani_v) numFailed++;
  gizmo__ptrc_glGetFragDataLocation = (GLint (CODEGEN_FUNCPTR *)(GLuint, const GLchar *))IntGetProcAddress("glGetFragDataLocation");
  if(!gizmo__ptrc_glGetFragDataLocation) numFailed++;
  gizmo__ptrc_glGetFramebufferAttachmentParameteriv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLenum, GLint *))IntGetProcAddress("glGetFramebufferAttachmentParameteriv");
  if(!gizmo__ptrc_glGetFramebufferAttachmentParameteriv) numFailed++;
  gizmo__ptrc_glGetIntegeri_v = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLint *))IntGetProcAddress("glGetIntegeri_v");
  if(!gizmo__ptrc_glGetIntegeri_v) numFailed++;
  gizmo__ptrc_glGetRenderbufferParameteriv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint *))IntGetProcAddress("glGetRenderbufferParameteriv");
  if(!gizmo__ptrc_glGetRenderbufferParameteriv) numFailed++;
  gizmo__ptrc_glGetStringi = (const GLubyte * (CODEGEN_FUNCPTR *)(GLenum, GLuint))IntGetProcAddress("glGetStringi");
  if(!gizmo__ptrc_glGetStringi) numFailed++;
  gizmo__ptrc_glGetTexParameterIiv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint *))IntGetProcAddress("glGetTexParameterIiv");
  if(!gizmo__ptrc_glGetTexParameterIiv) numFailed++;
  gizmo__ptrc_glGetTexParameterIuiv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLuint *))IntGetProcAddress("glGetTexParameterIuiv");
  if(!gizmo__ptrc_glGetTexParameterIuiv) numFailed++;
  gizmo__ptrc_glGetTransformFeedbackVarying = (void (CODEGEN_FUNCPTR *)(GLuint, GLuint, GLsizei, GLsizei *, GLsizei *, GLenum *, GLchar *))IntGetProcAddress("glGetTransformFeedbackVarying");
  if(!gizmo__ptrc_glGetTransformFeedbackVarying) numFailed++;
  gizmo__ptrc_glGetUniformuiv = (void (CODEGEN_FUNCPTR *)(GLuint, GLint, GLuint *))IntGetProcAddress("glGetUniformuiv");
  if(!gizmo__ptrc_glGetUniformuiv) numFailed++;
  gizmo__ptrc_glGetVertexAttribIiv = (void (CODEGEN_FUNCPTR *)(GLuint, GLenum, GLint *))IntGetProcAddress("glGetVertexAttribIiv");
  if(!gizmo__ptrc_glGetVertexAttribIiv) numFailed++;
  gizmo__ptrc_glGetVertexAttribIuiv = (void (CODEGEN_FUNCPTR *)(GLuint, GLenum, GLuint *))IntGetProcAddress("glGetVertexAttribIuiv");
  if(!gizmo__ptrc_glGetVertexAttribIuiv) numFailed++;
  gizmo__ptrc_glIsEnabledi = (GLboolean (CODEGEN_FUNCPTR *)(GLenum, GLuint))IntGetProcAddress("glIsEnabledi");
  if(!gizmo__ptrc_glIsEnabledi) numFailed++;
  gizmo__ptrc_glIsFramebuffer = (GLboolean (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glIsFramebuffer");
  if(!gizmo__ptrc_glIsFramebuffer) numFailed++;
  gizmo__ptrc_glIsRenderbuffer = (GLboolean (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glIsRenderbuffer");
  if(!gizmo__ptrc_glIsRenderbuffer) numFailed++;
  gizmo__ptrc_glIsVertexArray = (GLboolean (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glIsVertexArray");
  if(!gizmo__ptrc_glIsVertexArray) numFailed++;
  gizmo__ptrc_glMapBufferRange = (void * (CODEGEN_FUNCPTR *)(GLenum, GLintptr, GLsizeiptr, GLbitfield))IntGetProcAddress("glMapBufferRange");
  if(!gizmo__ptrc_glMapBufferRange) numFailed++;
  gizmo__ptrc_glRenderbufferStorage = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLsizei, GLsizei))IntGetProcAddress("glRenderbufferStorage");
  if(!gizmo__ptrc_glRenderbufferStorage) numFailed++;
  gizmo__ptrc_glRenderbufferStorageMultisample = (void (CODEGEN_FUNCPTR *)(GLenum, GLsizei, GLenum, GLsizei, GLsizei))IntGetProcAddress("glRenderbufferStorageMultisample");
  if(!gizmo__ptrc_glRenderbufferStorageMultisample) numFailed++;
  gizmo__ptrc_glTexParameterIiv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLint *))IntGetProcAddress("glTexParameterIiv");
  if(!gizmo__ptrc_glTexParameterIiv) numFailed++;
  gizmo__ptrc_glTexParameterIuiv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLuint *))IntGetProcAddress("glTexParameterIuiv");
  if(!gizmo__ptrc_glTexParameterIuiv) numFailed++;
  gizmo__ptrc_glTransformFeedbackVaryings = (void (CODEGEN_FUNCPTR *)(GLuint, GLsizei, const GLchar *const*, GLenum))IntGetProcAddress("glTransformFeedbackVaryings");
  if(!gizmo__ptrc_glTransformFeedbackVaryings) numFailed++;
  gizmo__ptrc_glUniform1ui = (void (CODEGEN_FUNCPTR *)(GLint, GLuint))IntGetProcAddress("glUniform1ui");
  if(!gizmo__ptrc_glUniform1ui) numFailed++;
  gizmo__ptrc_glUniform1uiv = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLuint *))IntGetProcAddress("glUniform1uiv");
  if(!gizmo__ptrc_glUniform1uiv) numFailed++;
  gizmo__ptrc_glUniform2ui = (void (CODEGEN_FUNCPTR *)(GLint, GLuint, GLuint))IntGetProcAddress("glUniform2ui");
  if(!gizmo__ptrc_glUniform2ui) numFailed++;
  gizmo__ptrc_glUniform2uiv = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLuint *))IntGetProcAddress("glUniform2uiv");
  if(!gizmo__ptrc_glUniform2uiv) numFailed++;
  gizmo__ptrc_glUniform3ui = (void (CODEGEN_FUNCPTR *)(GLint, GLuint, GLuint, GLuint))IntGetProcAddress("glUniform3ui");
  if(!gizmo__ptrc_glUniform3ui) numFailed++;
  gizmo__ptrc_glUniform3uiv = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLuint *))IntGetProcAddress("glUniform3uiv");
  if(!gizmo__ptrc_glUniform3uiv) numFailed++;
  gizmo__ptrc_glUniform4ui = (void (CODEGEN_FUNCPTR *)(GLint, GLuint, GLuint, GLuint, GLuint))IntGetProcAddress("glUniform4ui");
  if(!gizmo__ptrc_glUniform4ui) numFailed++;
  gizmo__ptrc_glUniform4uiv = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLuint *))IntGetProcAddress("glUniform4uiv");
  if(!gizmo__ptrc_glUniform4uiv) numFailed++;
  gizmo__ptrc_glVertexAttribI1i = (void (CODEGEN_FUNCPTR *)(GLuint, GLint))IntGetProcAddress("glVertexAttribI1i");
  if(!gizmo__ptrc_glVertexAttribI1i) numFailed++;
  gizmo__ptrc_glVertexAttribI1iv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLint *))IntGetProcAddress("glVertexAttribI1iv");
  if(!gizmo__ptrc_glVertexAttribI1iv) numFailed++;
  gizmo__ptrc_glVertexAttribI1ui = (void (CODEGEN_FUNCPTR *)(GLuint, GLuint))IntGetProcAddress("glVertexAttribI1ui");
  if(!gizmo__ptrc_glVertexAttribI1ui) numFailed++;
  gizmo__ptrc_glVertexAttribI1uiv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLuint *))IntGetProcAddress("glVertexAttribI1uiv");
  if(!gizmo__ptrc_glVertexAttribI1uiv) numFailed++;
  gizmo__ptrc_glVertexAttribI2i = (void (CODEGEN_FUNCPTR *)(GLuint, GLint, GLint))IntGetProcAddress("glVertexAttribI2i");
  if(!gizmo__ptrc_glVertexAttribI2i) numFailed++;
  gizmo__ptrc_glVertexAttribI2iv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLint *))IntGetProcAddress("glVertexAttribI2iv");
  if(!gizmo__ptrc_glVertexAttribI2iv) numFailed++;
  gizmo__ptrc_glVertexAttribI2ui = (void (CODEGEN_FUNCPTR *)(GLuint, GLuint, GLuint))IntGetProcAddress("glVertexAttribI2ui");
  if(!gizmo__ptrc_glVertexAttribI2ui) numFailed++;
  gizmo__ptrc_glVertexAttribI2uiv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLuint *))IntGetProcAddress("glVertexAttribI2uiv");
  if(!gizmo__ptrc_glVertexAttribI2uiv) numFailed++;
  gizmo__ptrc_glVertexAttribI3i = (void (CODEGEN_FUNCPTR *)(GLuint, GLint, GLint, GLint))IntGetProcAddress("glVertexAttribI3i");
  if(!gizmo__ptrc_glVertexAttribI3i) numFailed++;
  gizmo__ptrc_glVertexAttribI3iv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLint *))IntGetProcAddress("glVertexAttribI3iv");
  if(!gizmo__ptrc_glVertexAttribI3iv) numFailed++;
  gizmo__ptrc_glVertexAttribI3ui = (void (CODEGEN_FUNCPTR *)(GLuint, GLuint, GLuint, GLuint))IntGetProcAddress("glVertexAttribI3ui");
  if(!gizmo__ptrc_glVertexAttribI3ui) numFailed++;
  gizmo__ptrc_glVertexAttribI3uiv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLuint *))IntGetProcAddress("glVertexAttribI3uiv");
  if(!gizmo__ptrc_glVertexAttribI3uiv) numFailed++;
  gizmo__ptrc_glVertexAttribI4bv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLbyte *))IntGetProcAddress("glVertexAttribI4bv");
  if(!gizmo__ptrc_glVertexAttribI4bv) numFailed++;
  gizmo__ptrc_glVertexAttribI4i = (void (CODEGEN_FUNCPTR *)(GLuint, GLint, GLint, GLint, GLint))IntGetProcAddress("glVertexAttribI4i");
  if(!gizmo__ptrc_glVertexAttribI4i) numFailed++;
  gizmo__ptrc_glVertexAttribI4iv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLint *))IntGetProcAddress("glVertexAttribI4iv");
  if(!gizmo__ptrc_glVertexAttribI4iv) numFailed++;
  gizmo__ptrc_glVertexAttribI4sv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLshort *))IntGetProcAddress("glVertexAttribI4sv");
  if(!gizmo__ptrc_glVertexAttribI4sv) numFailed++;
  gizmo__ptrc_glVertexAttribI4ubv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLubyte *))IntGetProcAddress("glVertexAttribI4ubv");
  if(!gizmo__ptrc_glVertexAttribI4ubv) numFailed++;
  gizmo__ptrc_glVertexAttribI4ui = (void (CODEGEN_FUNCPTR *)(GLuint, GLuint, GLuint, GLuint, GLuint))IntGetProcAddress("glVertexAttribI4ui");
  if(!gizmo__ptrc_glVertexAttribI4ui) numFailed++;
  gizmo__ptrc_glVertexAttribI4uiv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLuint *))IntGetProcAddress("glVertexAttribI4uiv");
  if(!gizmo__ptrc_glVertexAttribI4uiv) numFailed++;
  gizmo__ptrc_glVertexAttribI4usv = (void (CODEGEN_FUNCPTR *)(GLuint, const GLushort *))IntGetProcAddress("glVertexAttribI4usv");
  if(!gizmo__ptrc_glVertexAttribI4usv) numFailed++;
  gizmo__ptrc_glVertexAttribIPointer = (void (CODEGEN_FUNCPTR *)(GLuint, GLint, GLenum, GLsizei, const GLvoid *))IntGetProcAddress("glVertexAttribIPointer");
  if(!gizmo__ptrc_glVertexAttribIPointer) numFailed++;
  gizmo__ptrc_glCopyBufferSubData = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLintptr, GLintptr, GLsizeiptr))IntGetProcAddress("glCopyBufferSubData");
  if(!gizmo__ptrc_glCopyBufferSubData) numFailed++;
  gizmo__ptrc_glDrawArraysInstanced = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLsizei, GLsizei))IntGetProcAddress("glDrawArraysInstanced");
  if(!gizmo__ptrc_glDrawArraysInstanced) numFailed++;
  gizmo__ptrc_glDrawElementsInstanced = (void (CODEGEN_FUNCPTR *)(GLenum, GLsizei, GLenum, const GLvoid *, GLsizei))IntGetProcAddress("glDrawElementsInstanced");
  if(!gizmo__ptrc_glDrawElementsInstanced) numFailed++;
  gizmo__ptrc_glGetActiveUniformBlockName = (void (CODEGEN_FUNCPTR *)(GLuint, GLuint, GLsizei, GLsizei *, GLchar *))IntGetProcAddress("glGetActiveUniformBlockName");
  if(!gizmo__ptrc_glGetActiveUniformBlockName) numFailed++;
  gizmo__ptrc_glGetActiveUniformBlockiv = (void (CODEGEN_FUNCPTR *)(GLuint, GLuint, GLenum, GLint *))IntGetProcAddress("glGetActiveUniformBlockiv");
  if(!gizmo__ptrc_glGetActiveUniformBlockiv) numFailed++;
  gizmo__ptrc_glGetActiveUniformName = (void (CODEGEN_FUNCPTR *)(GLuint, GLuint, GLsizei, GLsizei *, GLchar *))IntGetProcAddress("glGetActiveUniformName");
  if(!gizmo__ptrc_glGetActiveUniformName) numFailed++;
  gizmo__ptrc_glGetActiveUniformsiv = (void (CODEGEN_FUNCPTR *)(GLuint, GLsizei, const GLuint *, GLenum, GLint *))IntGetProcAddress("glGetActiveUniformsiv");
  if(!gizmo__ptrc_glGetActiveUniformsiv) numFailed++;
  gizmo__ptrc_glGetUniformBlockIndex = (GLuint (CODEGEN_FUNCPTR *)(GLuint, const GLchar *))IntGetProcAddress("glGetUniformBlockIndex");
  if(!gizmo__ptrc_glGetUniformBlockIndex) numFailed++;
  gizmo__ptrc_glGetUniformIndices = (void (CODEGEN_FUNCPTR *)(GLuint, GLsizei, const GLchar *const*, GLuint *))IntGetProcAddress("glGetUniformIndices");
  if(!gizmo__ptrc_glGetUniformIndices) numFailed++;
  gizmo__ptrc_glPrimitiveRestartIndex = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glPrimitiveRestartIndex");
  if(!gizmo__ptrc_glPrimitiveRestartIndex) numFailed++;
  gizmo__ptrc_glTexBuffer = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLuint))IntGetProcAddress("glTexBuffer");
  if(!gizmo__ptrc_glTexBuffer) numFailed++;
  gizmo__ptrc_glUniformBlockBinding = (void (CODEGEN_FUNCPTR *)(GLuint, GLuint, GLuint))IntGetProcAddress("glUniformBlockBinding");
  if(!gizmo__ptrc_glUniformBlockBinding) numFailed++;
  gizmo__ptrc_glClientWaitSync = (GLenum (CODEGEN_FUNCPTR *)(GLsync, GLbitfield, GLuint64))IntGetProcAddress("glClientWaitSync");
  if(!gizmo__ptrc_glClientWaitSync) numFailed++;
  gizmo__ptrc_glDeleteSync = (void (CODEGEN_FUNCPTR *)(GLsync))IntGetProcAddress("glDeleteSync");
  if(!gizmo__ptrc_glDeleteSync) numFailed++;
  gizmo__ptrc_glDrawElementsBaseVertex = (void (CODEGEN_FUNCPTR *)(GLenum, GLsizei, GLenum, const GLvoid *, GLint))IntGetProcAddress("glDrawElementsBaseVertex");
  if(!gizmo__ptrc_glDrawElementsBaseVertex) numFailed++;
  gizmo__ptrc_glDrawElementsInstancedBaseVertex = (void (CODEGEN_FUNCPTR *)(GLenum, GLsizei, GLenum, const GLvoid *, GLsizei, GLint))IntGetProcAddress("glDrawElementsInstancedBaseVertex");
  if(!gizmo__ptrc_glDrawElementsInstancedBaseVertex) numFailed++;
  gizmo__ptrc_glDrawRangeElementsBaseVertex = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid *, GLint))IntGetProcAddress("glDrawRangeElementsBaseVertex");
  if(!gizmo__ptrc_glDrawRangeElementsBaseVertex) numFailed++;
  gizmo__ptrc_glFenceSync = (GLsync (CODEGEN_FUNCPTR *)(GLenum, GLbitfield))IntGetProcAddress("glFenceSync");
  if(!gizmo__ptrc_glFenceSync) numFailed++;
  gizmo__ptrc_glFramebufferTexture = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLuint, GLint))IntGetProcAddress("glFramebufferTexture");
  if(!gizmo__ptrc_glFramebufferTexture) numFailed++;
  gizmo__ptrc_glGetBufferParameteri64v = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint64 *))IntGetProcAddress("glGetBufferParameteri64v");
  if(!gizmo__ptrc_glGetBufferParameteri64v) numFailed++;
  gizmo__ptrc_glGetInteger64i_v = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLint64 *))IntGetProcAddress("glGetInteger64i_v");
  if(!gizmo__ptrc_glGetInteger64i_v) numFailed++;
  gizmo__ptrc_glGetInteger64v = (void (CODEGEN_FUNCPTR *)(GLenum, GLint64 *))IntGetProcAddress("glGetInteger64v");
  if(!gizmo__ptrc_glGetInteger64v) numFailed++;
  gizmo__ptrc_glGetMultisamplefv = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLfloat *))IntGetProcAddress("glGetMultisamplefv");
  if(!gizmo__ptrc_glGetMultisamplefv) numFailed++;
  gizmo__ptrc_glGetSynciv = (void (CODEGEN_FUNCPTR *)(GLsync, GLenum, GLsizei, GLsizei *, GLint *))IntGetProcAddress("glGetSynciv");
  if(!gizmo__ptrc_glGetSynciv) numFailed++;
  gizmo__ptrc_glIsSync = (GLboolean (CODEGEN_FUNCPTR *)(GLsync))IntGetProcAddress("glIsSync");
  if(!gizmo__ptrc_glIsSync) numFailed++;
  gizmo__ptrc_glMultiDrawElementsBaseVertex = (void (CODEGEN_FUNCPTR *)(GLenum, const GLsizei *, GLenum, const GLvoid *const*, GLsizei, const GLint *))IntGetProcAddress("glMultiDrawElementsBaseVertex");
  if(!gizmo__ptrc_glMultiDrawElementsBaseVertex) numFailed++;
  gizmo__ptrc_glProvokingVertex = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glProvokingVertex");
  if(!gizmo__ptrc_glProvokingVertex) numFailed++;
  gizmo__ptrc_glSampleMaski = (void (CODEGEN_FUNCPTR *)(GLuint, GLbitfield))IntGetProcAddress("glSampleMaski");
  if(!gizmo__ptrc_glSampleMaski) numFailed++;
  gizmo__ptrc_glTexImage2DMultisample = (void (CODEGEN_FUNCPTR *)(GLenum, GLsizei, GLint, GLsizei, GLsizei, GLboolean))IntGetProcAddress("glTexImage2DMultisample");
  if(!gizmo__ptrc_glTexImage2DMultisample) numFailed++;
  gizmo__ptrc_glTexImage3DMultisample = (void (CODEGEN_FUNCPTR *)(GLenum, GLsizei, GLint, GLsizei, GLsizei, GLsizei, GLboolean))IntGetProcAddress("glTexImage3DMultisample");
  if(!gizmo__ptrc_glTexImage3DMultisample) numFailed++;
  gizmo__ptrc_glWaitSync = (void (CODEGEN_FUNCPTR *)(GLsync, GLbitfield, GLuint64))IntGetProcAddress("glWaitSync");
  if(!gizmo__ptrc_glWaitSync) numFailed++;
  return numFailed;
}

typedef int (*PFN_LOADFUNCPOINTERS)();
typedef struct gizmo_ogl_StrToExtMap_s
{
  char *extensionName;
  int *extensionVariable;
  PFN_LOADFUNCPOINTERS LoadExtension;
} gizmo_ogl_StrToExtMap;

static gizmo_ogl_StrToExtMap ExtensionMap[1] = {
  {"", NULL, NULL},
};

static int g_extensionMapSize = 0;

static gizmo_ogl_StrToExtMap *FindExtEntry(const char *extensionName)
{
  int loop;
  gizmo_ogl_StrToExtMap *currLoc = ExtensionMap;
  for(loop = 0; loop < g_extensionMapSize; ++loop, ++currLoc)
  {
  	if(strcmp(extensionName, currLoc->extensionName) == 0)
  		return currLoc;
  }
  
  return NULL;
}

static void ClearExtensionVars()
{
}


static void LoadExtByName(const char *extensionName)
{
	gizmo_ogl_StrToExtMap *entry = NULL;
	entry = FindExtEntry(extensionName);
	if(entry)
	{
		if(entry->LoadExtension)
		{
			int numFailed = entry->LoadExtension();
			if(numFailed == 0)
			{
				*(entry->extensionVariable) = gizmo_ogl_LOAD_SUCCEEDED;
			}
			else
			{
				*(entry->extensionVariable) = gizmo_ogl_LOAD_SUCCEEDED + numFailed;
			}
		}
		else
		{
			*(entry->extensionVariable) = gizmo_ogl_LOAD_SUCCEEDED;
		}
	}
}


static void ProcExtsFromExtList()
{
	GLint iLoop;
	GLint iNumExtensions = 0;
	gizmo__ptrc_glGetIntegerv(GL_NUM_EXTENSIONS, &iNumExtensions);

	for(iLoop = 0; iLoop < iNumExtensions; iLoop++)
	{
		const char *strExtensionName = (const char *)gizmo__ptrc_glGetStringi(GL_EXTENSIONS, iLoop);
		LoadExtByName(strExtensionName);
	}
}

int gizmo_ogl_LoadFunctions()
{
  int numFailed = 0;
  ClearExtensionVars();
  
  gizmo__ptrc_glGetIntegerv = (void (CODEGEN_FUNCPTR *)(GLenum, GLint *))IntGetProcAddress("glGetIntegerv");
  if(!gizmo__ptrc_glGetIntegerv) return gizmo_ogl_LOAD_FAILED;
  gizmo__ptrc_glGetStringi = (const GLubyte * (CODEGEN_FUNCPTR *)(GLenum, GLuint))IntGetProcAddress("glGetStringi");
  if(!gizmo__ptrc_glGetStringi) return gizmo_ogl_LOAD_FAILED;
  
  ProcExtsFromExtList();
  numFailed = Load_Version_3_2();
  
  if(numFailed == 0)
  	return gizmo_ogl_LOAD_SUCCEEDED;
  else
  	return gizmo_ogl_LOAD_SUCCEEDED + numFailed;
}

static int g_major_version = 0;
static int g_minor_version = 0;

static void GetGLVersion()
{
	glGetIntegerv(GL_MAJOR_VERSION, &g_major_version);
	glGetIntegerv(GL_MINOR_VERSION, &g_minor_version);
}

int gizmo_ogl_GetMajorVersion()
{
	if(g_major_version == 0)
		GetGLVersion();
	return g_major_version;
}

int gizmo_ogl_GetMinorVersion()
{
	if(g_major_version == 0) //Yes, check the major version to get the minor one.
		GetGLVersion();
	return g_minor_version;
}

int gizmo_ogl_IsVersionGEQ(int majorVersion, int minorVersion)
{
	if(g_major_version == 0)
		GetGLVersion();
		
	if(majorVersion > g_major_version) return 1;
	if(majorVersion < g_major_version) return 0;
	if(minorVersion >= g_minor_version) return 1;
	return 0;
}

