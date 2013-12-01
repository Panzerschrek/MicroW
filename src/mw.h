#ifndef MICRO_RPG_H
#define MICRO_RPG_H


#ifdef MW_OS_WINDOWS
#include <windows.h>
inline void usleep(int n ) { Sleep(n/1000); }
#else
#include <unistd.h>
#endif

#include <stdio.h>

#include <stdlib.h>
#include <string.h>

#include <GL/gl.h>

#ifdef MW_OS_WINDOWS
#include "GL/wglext.h"
#else
//#include "GL/glxext.h"
#include <GL/glx.h>
#include <X11/X.h>
#include <X11/keysym.h>
#endif
#include "GL/glext.h"


#ifndef MW_DEBUG
#define printf "printf must be removed from release build"ffawf
#endif//generate error for all printf calling

#define MW_GL_VERSION 33


template< unsigned int N, unsigned int k >
unsigned int NearestPOT_r()
{
   if( N <= k ) 
       return k;
   else 
       return NearestPOT_r< N, k*2 >();
}

template< unsigned int N >  
unsigned int NearestPOT()
{
        return  NearestPOT_r< N, 1 >();
}



#endif//MICRO_RPG_H
