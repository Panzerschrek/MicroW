#include "mw.h"
extern "C"
{
extern PFNGLGENVERTEXARRAYSPROC     glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC     glBindVertexArray;
extern PFNGLVERTEXATTRIBPOINTERPROC	glVertexAttribPointer;
extern PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer;
extern PFNGLGENBUFFERSPROC			glGenBuffers;
extern PFNGLBINDBUFFERPROC			glBindBuffer;
extern PFNGLBUFFERDATAPROC			glBufferData;
extern PFNGLBUFFERSUBDATAPROC		glBufferSubData;


extern PFNGLUNIFORM1IPROC			glUniform1i;
extern PFNGLUNIFORMMATRIX4FVPROC	glUniformMatrix4fv;
extern PFNGLUNIFORMMATRIX3FVPROC	glUniformMatrix3fv;
extern PFNGLUNIFORM3FPROC			glUniform3f;
extern PFNGLUNIFORM1FPROC           glUniform1f;

extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLBINDATTRIBLOCATIONPROC	glBindAttribLocation;
extern PFNGLCREATEPROGRAMPROC		glCreateProgram;
extern PFNGLATTACHSHADERPROC		glAttachShader;
extern PFNGLLINKPROGRAMPROC		glLinkProgram;
extern PFNGLUSEPROGRAMPROC			glUseProgram;
extern PFNGLGETPROGRAMIVPROC		glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC	glGetProgramInfoLog;

extern PFNGLCREATESHADERPROC			glCreateShader;
extern PFNGLSHADERSOURCEPROC			glShaderSource;
extern PFNGLCOMPILESHADERPROC			glCompileShader;
extern PFNGLGETSHADERIVPROC			glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC		glGetShaderInfoLog;

extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;

extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
extern PFNGLACTIVETEXTUREPROC  glActiveTexture;
extern PFNGLTEXIMAGE3DPROC     glTexImage3D;
extern PFNGLTEXSUBIMAGE3DPROC    glTexSubImage3D;

extern PFNGLDRAWELEMENTSBASEVERTEXPROC glDrawElementsBaseVertex;
}
void GetGLFunctions();
