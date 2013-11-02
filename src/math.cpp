#include "math.h"

void Mat4Identity( float* m )
{
	unsigned int i= 0;
	for( i= 1; i< 15; i++ )
		m[i]= 0.0f;
	for( i= 0; i< 16; i+=5 )
		m[i]= 1.0f;
}

void Mat4Transpose( float* m )
{
    float tmp;
    for( int j= 0; j< 3; j++ )
         for( int i= j; i< 4; i++ )
         {
             tmp= m[i + j*4 ];
             m[ i + j*4]= m[ j + i*4 ];
             m[ j + i*4 ]= tmp;
         }
}



void Vec3Mat4Mul( const float* v, const float* m, float* v_dst )
{
	unsigned int i;
	for( i= 0; i< 3; i++ )
		v_dst[i]= v[0] * m[i] + v[1] * m[i+4] + v[2] * m[i+8] + m[i+12];
}

//v_dst*= m
void Vec3Mat4Mul( float* v_dst, const float* m )
{
	float v[3];
	unsigned int i;
	for( i= 0; i< 3; i++ )
		v[i]= v_dst[i];

	for( i= 0; i< 3; i++ )
		v_dst[i]= v[0] * m[i] + v[1] * m[i+4] + v[2] * m[i+8] + m[i+12];

}


void Mat3Mul( const float* m1, const float* m2, float* m_dst )
{
    unsigned int i, j;

	for( i= 0; i< 3; i++ )
		for( j= 0; j< 9; j+=3 )
		{
			m_dst[ i + j ]= m1[ 0 + j ] * m2[ i ]   + 	 m1[ 1 + j ] * m2[ 3 + i ] +
							m1[ 2 + j ] * m2[ 6 + i ];
		}
}

void Mat4Mul( const float* m1, const float* m2, float* m_dst )
{
	unsigned int i, j;

	for( i= 0; i< 4; i++ )
		for( j= 0; j< 16; j+=4 )
		{
			m_dst[ i + j ]= m1[ 0 + j ] * m2[ i ]   + 	 m1[ 1 + j ] * m2[ 4 + i ] +
							m1[ 2 + j ] * m2[ 8 + i ]   +  m1[ 3 + j ] * m2[ 12 + i ];
		}
}

//m1*= m2
void Mat4Mul( float* m1_dst, const float* m2 )
{
	unsigned int i, j;
	float m1[16];
	for( i= 0; i< 16; i++ )
		m1[i]= m1_dst[i];


	for( i= 0; i< 4; i++ )
		for( j= 0; j< 16; j+=4 )
		{
			m1_dst[ i + j ]= m1[ 0 + j ] * m2[ i ]   + 	 m1[ 1 + j ] * m2[ 4 + i ] +
							m1[ 2 + j ] * m2[ 8 + i ]   +  m1[ 3 + j ] * m2[ 12 + i ];
		}

}

void Mat4Perspective( float* m, float aspect, float fov_y, float z_near, float z_far )
{
    Mat4Identity(m);
    float f= 1.0f / tan( fov_y * 0.5f );
    
    m[0]= f / aspect;
    m[5]= f;

    /*f= 1.0f / ( z_far - z_near );
    m[14]= -2.0f * f * z_near * z_far;
    m[10]= ( z_near + z_far ) * f;
    m[11]= 1.0f;*/

    f= -1.0f / ( z_far - z_near );
    m[14]= 2.0f * f * z_near * z_far;
    m[10]= ( z_near + z_far ) * f;
    m[11]= -1.0f;


    m[15]= 0.0f;
}

void Mat4RotateXY( float* m, float a )
{
    Mat4Identity(m);

    m[0]= cos(a);
    m[1]= sin(a);
    m[4]= -m[1];
    m[5]= m[0];
}

void Mat4ToMat3( float* m )
{
    for( int i= 0; i< 3; i++ )
        m[3+i]= m[4+i];
    for( int i= 0; i< 3; i++ )
        m[6+i]= m[8+i];
}

void Mat3ToMat4( float* m )
{
    for( int i= 2; i>= 0; i-- )
        m[8+i]= m[6+i]; 
    for( int i= 2; i>= 0; i-- )
        m[4+i]= m[3+i]; 

    for( int i= 0; i< 3; i++ )
        m[12+i]= m[3+4*i]= 0.0f;
    m[15]= 1.0f;
}

float Vec3Len( const float* v )
{
    return sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );
}

float Distance( const float* v1, const float* v2 )
{
    float v0[]= { v1[0], v1[1], v2[2] };
    Vec3Sub( v0, v2 );
    return Vec3Len(v0);
}

void Vec3Normalize( float* v )
{
    float l;
    l= Vec3Len(v);

    if( l != 0.0f )
    {
        l= 1.0f / l;
        v[0]*= l;
        v[1]*= l;
        v[2]*= l;
    }
}

void Vec3Mul( float* v, float a )
{
    v[0]*= a;
    v[1]*= a;
    v[2]*= a;
}


void Vec3Add( float* v1, const float* v2 )
{
    v1[0]+= v2[0];
    v1[1]+= v2[1];
    v1[2]+= v2[2];
}


void Vec3Sub( float* v1, const float* v2 )
{
    v1[0]-= v2[0];
    v1[1]-= v2[1];
    v1[2]-= v2[2];
}

float Vec3Dot( const float* v1, const float* v2 )
{
    return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}

float Vec3XYAngle( const float* v )
{
    float a;
   
    if( v[0] != 0.0f )
    {
        a= atan( v[1] / v[0] );
        if( v[0] < 0.0f )
            a+= MW_PI;
    }
    else
    {
        if( v[1] > 0.0f )
            a= MW_PI2;
        else
            a= 3.0f * MW_PI4;
    }

    return a;

}