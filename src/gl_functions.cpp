#include "gl_functions.h"

#ifndef MW_IA32_MASM
extern "C"
{
PFNGLGENVERTEXARRAYSPROC     glGenVertexArrays= NULL;
PFNGLBINDVERTEXARRAYPROC     glBindVertexArray= NULL;
PFNGLVERTEXATTRIBPOINTERPROC	glVertexAttribPointer= NULL;
PFNGLVERTEXATTRIBIPOINTERPROC	glVertexAttribIPointer= NULL;
PFNGLGENBUFFERSPROC			glGenBuffers= NULL;
PFNGLBINDBUFFERPROC			glBindBuffer= NULL;
PFNGLBUFFERDATAPROC			glBufferData= NULL;
PFNGLBUFFERSUBDATAPROC		glBufferSubData= NULL;


PFNGLUNIFORM1IPROC			glUniform1i= NULL;
PFNGLUNIFORMMATRIX4FVPROC	glUniformMatrix4fv= NULL;
PFNGLUNIFORMMATRIX3FVPROC	glUniformMatrix3fv= NULL;
PFNGLUNIFORM3FPROC			glUniform3f= NULL;
PFNGLUNIFORM1FPROC          glUniform1f= NULL;

PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation= NULL;
PFNGLBINDATTRIBLOCATIONPROC	glBindAttribLocation= NULL;
PFNGLCREATEPROGRAMPROC		glCreateProgram= NULL;
PFNGLATTACHSHADERPROC		glAttachShader= NULL;
PFNGLLINKPROGRAMPROC		glLinkProgram= NULL;
PFNGLUSEPROGRAMPROC			glUseProgram= NULL;
PFNGLGETPROGRAMIVPROC		glGetProgramiv= NULL;
PFNGLGETPROGRAMINFOLOGPROC	glGetProgramInfoLog= NULL;

PFNGLCREATESHADERPROC		glCreateShader= NULL;
PFNGLSHADERSOURCEPROC		glShaderSource= NULL;
PFNGLCOMPILESHADERPROC		glCompileShader= NULL;
PFNGLGETSHADERIVPROC		glGetShaderiv= NULL;
PFNGLGETSHADERINFOLOGPROC	glGetShaderInfoLog= NULL;


PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray= NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray= NULL;

PFNGLGENERATEMIPMAPPROC glGenerateMipmap= NULL;
PFNGLACTIVETEXTUREPROC  glActiveTexture= NULL;
PFNGLTEXIMAGE3DPROC     glTexImage3D= NULL;
PFNGLTEXSUBIMAGE3DPROC    glTexSubImage3D= NULL;

PFNGLDRAWELEMENTSBASEVERTEXPROC glDrawElementsBaseVertex=NULL;

}
#endif//MW_IA32_MASM


const char* func_names[]= { 
    "glGenVertexArrays", "glBindVertexArray", "glVertexAttribPointer", "glVertexAttribIPointer", "glGenBuffers", "glBindBuffer", "glBufferData", "glBufferSubData",//8
    "glUniform1i", "glUniformMatrix4fv", "glUniformMatrix3fv", "glUniform3f", "glUniform1f",//5
    "glGetUniformLocation", "glBindAttribLocation", "glCreateProgram", "glAttachShader", "glLinkProgram", "glUseProgram", "glGetProgramiv", "glGetProgramInfoLog",//8
    "glCreateShader", "glShaderSource", "glCompileShader", "glGetShaderiv", "glGetShaderInfoLog", //5
    "glEnableVertexAttribArray", "glDisableVertexAttribArray", //2
    "glGenerateMipmap", "glActiveTexture", "glTexImage3D", "glTexSubImage3D", //4
    "glDrawElementsBaseVertex"//1
                 };
#define FUNC_COUNT 33



#define MACRO_TO_STR(X) #X
#define MACRO_VALUE_TO_STR(X) MACRO_TO_STR(X)

#define CHECK_FUNC_STATUS(x)\
if( x == NULL )\
{\
    printf( "error, function \"%s\"= %u\n", MACRO_TO_STR(x), (GLuint)x );\
}



#ifdef MW_OS_WINDOWS

#define GET_PROC_ADDRESS(x)\
wglGetProcAddress( MACRO_TO_STR(x) )

#else
#define GET_PROC_ADDRESS(x)\
glXGetProcAddressARB( (const unsigned char*)MACRO_TO_STR(x) )

#endif


void GetGLFunctions()
{
#ifndef MW_IA32_MASM

    glGenVertexArrays= (PFNGLGENVERTEXARRAYSPROC) GET_PROC_ADDRESS(glGenVertexArrays);
    glBindVertexArray= (PFNGLBINDVERTEXARRAYPROC) GET_PROC_ADDRESS(glBindVertexArray);
    glVertexAttribPointer= ( PFNGLVERTEXATTRIBPOINTERPROC ) GET_PROC_ADDRESS( glVertexAttribPointer );

    glGenBuffers= (PFNGLGENBUFFERSPROC) GET_PROC_ADDRESS( glGenBuffers );
    glBindBuffer= (PFNGLBINDBUFFERPROC) GET_PROC_ADDRESS( glBindBuffer );
    glBufferData= (PFNGLBUFFERDATAPROC)	GET_PROC_ADDRESS( glBufferData );
    glBufferSubData= (PFNGLBUFFERSUBDATAPROC) GET_PROC_ADDRESS( glBufferSubData );

    glUniform1i= (PFNGLUNIFORM1IPROC) GET_PROC_ADDRESS(glUniform1i);
    glUniformMatrix4fv= (PFNGLUNIFORMMATRIX4FVPROC)	GET_PROC_ADDRESS(glUniformMatrix4fv);
    glUniformMatrix3fv= (PFNGLUNIFORMMATRIX3FVPROC)	GET_PROC_ADDRESS(glUniformMatrix3fv);
    glUniform3f= (PFNGLUNIFORM3FPROC) GET_PROC_ADDRESS(glUniform3f);
    glUniform1f= (PFNGLUNIFORM1FPROC) GET_PROC_ADDRESS(glUniform1f);

    glGetUniformLocation= (PFNGLGETUNIFORMLOCATIONPROC) GET_PROC_ADDRESS(glGetUniformLocation);
    glBindAttribLocation= (PFNGLBINDATTRIBLOCATIONPROC) GET_PROC_ADDRESS(glBindAttribLocation);
    glCreateProgram= (PFNGLCREATEPROGRAMPROC) GET_PROC_ADDRESS(glCreateProgram);
    glAttachShader= (PFNGLATTACHSHADERPROC)	GET_PROC_ADDRESS(glAttachShader);
    glLinkProgram= (PFNGLLINKPROGRAMPROC)GET_PROC_ADDRESS(glLinkProgram);
    glUseProgram= (PFNGLUSEPROGRAMPROC)	GET_PROC_ADDRESS(glUseProgram);
    glGetProgramiv= (PFNGLGETPROGRAMIVPROC)		GET_PROC_ADDRESS(glGetProgramiv);
	glGetProgramInfoLog= (PFNGLGETPROGRAMINFOLOGPROC)	GET_PROC_ADDRESS(glGetProgramInfoLog);

    glCreateShader= (PFNGLCREATESHADERPROC)GET_PROC_ADDRESS(glCreateShader);
    glShaderSource= (PFNGLSHADERSOURCEPROC)	GET_PROC_ADDRESS(glShaderSource);
    glCompileShader= (PFNGLCOMPILESHADERPROC)GET_PROC_ADDRESS(glCompileShader);
    glGetShaderiv= (PFNGLGETSHADERIVPROC)		GET_PROC_ADDRESS(glGetShaderiv);
	glGetShaderInfoLog= (PFNGLGETSHADERINFOLOGPROC)	GET_PROC_ADDRESS(glGetShaderInfoLog);

    glEnableVertexAttribArray=  (PFNGLENABLEVERTEXATTRIBARRAYPROC) GET_PROC_ADDRESS(glEnableVertexAttribArray);
    glDisableVertexAttribArray= (PFNGLDISABLEVERTEXATTRIBARRAYPROC) GET_PROC_ADDRESS(glDisableVertexAttribArray);

    glActiveTexture= (PFNGLACTIVETEXTUREPROC)  GET_PROC_ADDRESS(glActiveTexture);
    glGenerateMipmap= (PFNGLGENERATEMIPMAPPROC) GET_PROC_ADDRESS(glGenerateMipmap);
    glTexImage3D=   (PFNGLTEXIMAGE3DPROC)     GET_PROC_ADDRESS(glTexImage3D);
    glTexSubImage3D= (PFNGLTEXSUBIMAGE3DPROC) GET_PROC_ADDRESS(glTexSubImage3D);
  

#else//MW_IA32_MASM
    _asm
    {
       
        lea ebx, glGenVertexArrays//defined in gl_func_a.asm - first gl function on list
        lea edi, func_names
        mov esi, DWORD PTR[ wglGetProcAddress ]
        mov ecx, 0

l1:
        cmp ecx, FUNC_COUNT
        jz ex
        
        push edi
        push esi
        push ecx
        push ebx

        push DWORD PTR[ edi ] 
        call esi
        pop ebx
        mov DWORD PTR[ebx], eax
        pop ecx
        pop esi
        pop edi
        
        add ebx, 4
        add edi, 4
        inc ecx
        jmp l1
ex:
    }
    

    
#endif//MW_IA32_MASM

}
