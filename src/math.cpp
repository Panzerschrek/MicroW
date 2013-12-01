#include "math.h"
#include <stdlib.h>

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

float Vec3ZAngle( const float* v )
{
    float v2[]= { v[0], v[1], v[2] };
    
    return atan( v[2] / sqrt( v[0]*v[0] + v[1]*v[1] ) );
}


void Vec3Rotate( float* v, const float* axis, float a )
{
	float ax_y[3];
	float ax_x[]= { axis[0], axis[1], axis[2] };//vecotr from vector to rotate vector
	Vec3Mul( ax_x, Vec3Dot( axis, v ) );
	Vec3Mul( ax_x, -1.0f );
	Vec3Add( v, ax_x );
	
	ax_y[0]= ax_x[1] * axis[2] - axis[1] * ax_x[2];
	ax_y[1]= ax_x[2] * axis[0] - axis[2] * ax_x[0];
	ax_y[2]= ax_x[0] * axis[1] - axis[0] * ax_x[1];

	float cs[]= { cos(a), sin(a) };
	v[0]+= ( -1.0f +cs[0] ) * ax_x[0] + cs[1] * ax_y[0];
	v[1]+= ( -1.0f +cs[0] ) * ax_x[1] + cs[1] * ax_y[1];
	v[2]+= ( -1.0f +cs[0] ) * ax_x[2] + cs[1] * ax_y[2];
	
} 

float randf( float max )
{
    return max * float(rand())/float(RAND_MAX);
}

float randf( float min, float max )
{
    return ( max - min ) * float(rand())/float(RAND_MAX) +min;
}

bool FindCollisionWithSphere( const float* pos, const float* vec, const float* sphere_pos, float sphere_radius, float* collision_pos )
{
    float vec2_sphere[3];
    float vec2_near_point[3];
    float dot, l1, l2;


    vec2_sphere[0]= sphere_pos[0];
    vec2_sphere[1]= sphere_pos[1];
    vec2_sphere[2]= sphere_pos[2];
    Vec3Sub( vec2_sphere, pos );
    
    dot= Vec3Dot( vec2_sphere, vec );
    if( dot < 0.0f )
        return false;

    vec2_near_point[0]= dot * vec[0];
    vec2_near_point[1]= dot * vec[1];
    vec2_near_point[2]= dot * vec[2];

    l1= Vec3Len( vec2_sphere );
    l2= Vec3Len( vec2_near_point );

    float dist= sqrt( l1*l1 - l2*l2 );

    if(  dist < sphere_radius )
    {
        Vec3Mul( vec2_near_point, ( l2 - sqrt( sphere_radius * sphere_radius - dist*dist ) )/l2 );
        collision_pos[0]= pos[0] + vec2_near_point[0];
        collision_pos[1]= pos[1] + vec2_near_point[1];
        collision_pos[2]= pos[2] + vec2_near_point[2];
        return true;
    }

    return false;
}

bool FindCollisionWithCylinder( const float* pos, const float* vec, const float* cylinder_pos, float cylinder_radius, float cylinder_height, float* collision_pos )
{
    float vec2d[]= { vec[0], vec[1], 0.0f }; Vec3Normalize( vec2d );
    float vec2_cylinder2d[3];
    float vec2_nearest_point2d[3];
    float dot, l1, l2, dist;

    vec2_cylinder2d[0]= cylinder_pos[0];
    vec2_cylinder2d[1]= cylinder_pos[1];
    Vec3Sub( vec2_cylinder2d, pos );
    vec2_cylinder2d[2]= 0.0f;

    dot= Vec3Dot( vec2_cylinder2d, vec2d );
    if( dot <= 0.0f )
        return false;

    vec2_nearest_point2d[0]= dot * vec2d[0];
    vec2_nearest_point2d[1]= dot * vec2d[1];
    vec2_nearest_point2d[2]= 0.0f;

    l1= Vec3Len( vec2_cylinder2d );
    l2= Vec3Len( vec2_nearest_point2d );

    dist= sqrt( l1*l1 - l2*l2 );

    if(  dist < cylinder_radius )
    {
         Vec3Mul( vec2_nearest_point2d, ( l2 - sqrt( cylinder_radius * cylinder_radius - dist*dist ) )/l2 );
        collision_pos[0]= pos[0] + vec2_nearest_point2d[0];
        collision_pos[1]= pos[1] + vec2_nearest_point2d[1];
        collision_pos[2]= pos[2];
        if( fabs(vec[2]) < 1.0f )
        {
           collision_pos[2]= pos[2] + l2 * vec[2] / sqrt( 1.0f - vec[2] * vec[2] );
           if( collision_pos[2] < cylinder_pos[2] )
               return false;
           else if ( collision_pos[2] > cylinder_pos[2] + cylinder_height )
               return false;
           else
               return true;
        }
        else
            return false;
    }
    else
        return false;
}


//sample from http://www.benshoof.org/blog/minicrt/
static short control_word;
static short control_word2;
void SetFloatingPointRoundingToTruncate()
{
    __asm
    {
        fstcw   control_word                // store fpu control word
        mov     dx, word ptr [control_word]
        or      dx, 0x0C00                  // rounding: truncate
        mov     control_word2, dx
        fldcw   control_word2               // load modfied control word
    }
}