#ifndef MATH_H
#define MATH_H



//matrix and vector functions
void Mat4Identity( float* m );
void Mat4Transpose( float* m );

void Vec3Mat4Mul( const float* v, const float* m, float* v_dst );
void Vec3Mat4Mul( float* v_dst, const float* m );

void Mat3Mul( const float* m1, const float* m2, float* m_dst );
void Mat4Mul( const float* m1, const float* m2, float* m_dst );
void Mat4Mul( float* m1_dst, const float* m2 );

void Mat4Perspective( float* m, float aspect, float fov_y, float z_near, float z_far );
void Mat4RotateXY( float* m, float a );
void Mat4ToMat3( float* m );
void Mat3ToMat4( float* m );

void Vec3Mul( float* v, float a );
float Vec3Len( const float* v );
float Distance( const float* v1, const float* v2 );
void Vec3Normalize( float* v );
float Vec3Dot( const float* v1, const float* v2 );

void Vec3Add( float* v1, const float* v2 );
void Vec3Sub( float* v1, const float* v2 );


float Vec3XYAngle( const float* v );


#define MW_PI 3.1415926535897932384626433832795f
#define MW_2PI 6.283185307179586476925286766559f
#define MW_PI2 1.5707963267948966192313216916398f
#define MW_2PI3 2.0943951023931954923084289221863f
#define MW_PI3 1.0471975511965977461542144610932f
#define MW_PI4 0.78539816339744830961566084581988f
#define MW_PI6 0.52359877559829887307710723054658f
/*
if target is win32 and compiler is M$ Visual Studio default math functions from math.h replaced by inlined asm functions.
*/

#ifndef MW_IA32_MASM
#include <math.h>
#endif


#ifdef MW_IA32_MASM

inline float cos( float a )
{
    float r;
    __asm
    {
        fld a
        fcos
        fstp r
    }
    return r;
}

inline float sqrt( float a )
{
    float r;
    __asm
    {
        fld a
        fsqrt
        fstp r
    }

    return r;

}

inline float log( float a )
{
    float r;
    __asm
    {
        fldln2
        fld a
        FYL2X
        fstp r
        
    }
    return r;
}

/*inline float exp( float a )
{
    float r;
    __asm
    {
       
    }
    return r;
}*/


inline float sin( float a )
{
    float r;
    __asm
    {
        fld a
        fsin
        fstp r
    }
    return r;
}


inline float tan( float a )
{
    float r;
    __asm
    {
        fld a
        fptan
        fstp r
        fstp r
    }
    return r;
}

inline float fabs( float x )
{
    float r;
    __asm
    {
        fld x
        fabs
        fstp r
    }
    return r;
}

inline float sign( float a )
{
   if( a > 0.0f )
       return 1.0f;
   else if ( a < 0.0f )
       return -1.0f;
   else
       return 0.0f;
}

inline float atan( float a )
{
    float r;
    __asm
    {
        fld a
        fld1
        fpatan
        fstp r
       // fstp r
    }
    return r;
}

#endif//MRPG_IA32_MASM

#endif//MATH_H
