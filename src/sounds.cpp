#include "math.h"
#include "mw.h"

  unsigned char crowbar_sound[]= {
  24,
 61, 4, 187, 84, 0, 55, 4, 165, 75, 241, 81, 4, 203, 67, 17, 54, 4, 135, 62, 61, 0, 0, 252, 51, 255, 62, 4, 182, 51, 184, 178, 0,
 212, 34, 138, 89, 4, 5, 32, 230, 1, 0, 21, 30, 139, 108, 0, 132, 27, 66, 86, 4, 50, 25, 234, 124, 0, 130, 24, 174, 110, 0, 217, 23,
 8, 176, 0, 143, 21, 41, 83, 4, 141, 21, 212, 98, 0, 23, 20, 217, 60, 4, 104, 19, 153, 91, 4, 232, 18, 202, 64, 4, 168, 18, 218, 101,
 0, 145, 18, 166, 115, 0, 245, 17, 63, 114, 0, 124, 17, 7, 2, 0, 3, 17, 203, 65, 4, 182, 16, 247 };
  unsigned char chaingun_sound[]= {
 16,
 191, 3, 100, 167, 149, 195, 3, 152, 120, 103, 106, 0, 32, 70, 74, 188, 3, 183, 47, 136, 198, 3, 51, 45, 117, 24, 7, 8, 36, 156, 131, 7,
 23, 35, 33, 85, 3, 129, 32, 104, 185, 3, 255, 30, 150, 237, 7, 40, 30, 218, 88, 3, 137, 26, 142, 213, 0, 131, 26, 93, 99, 9, 223, 25,
 182, 163, 5, 177, 24, 31, 201, 3, 146, 23, 120, 223, 1, 141, 23, 179 };

  unsigned char shot357_sound[]= {
      32,
89, 0, 21, 13, 72, 76, 0, 2, 13, 217, 59, 0, 35, 12, 141, 56, 0, 28, 12, 23, 79, 0, 7, 12, 106, 86, 0, 230, 11, 7, 93,
 0, 47, 10, 158, 83, 0, 8, 9, 56, 126, 0, 208, 8, 41, 116, 0, 55, 8, 203, 53, 0, 236, 7, 200, 129, 0, 196, 7, 23, 123, 0, 169,
 7, 105, 73, 0, 146, 7, 134, 113, 0, 81, 7, 237, 119, 0, 13, 7, 142, 133, 0, 132, 6, 77, 63, 0, 61, 6, 244, 136, 0, 157, 5, 142,
 69, 0, 92, 5, 83, 96, 0, 36, 5, 234, 173, 0, 249, 4, 232, 179, 0, 243, 4, 157, 223, 5, 188, 4, 90, 159, 4, 163, 4, 59, 176, 0,
 148, 4, 220, 66, 0, 147, 4, 212, 149, 0, 122, 4, 153, 213, 1, 110, 4, 34, 146, 0, 106, 4, 233, 159, 0, 101, 4, 29, 109, 0, 81, 4,
 155 };

  unsigned char ambient7_sound[]= {
 16,
 239, 0, 16, 179, 85, 43, 1, 42, 176, 101, 103, 1, 63, 91, 21, 223, 1, 142, 85, 99, 119, 0, 213, 70, 117, 163, 1, 28, 43, 51, 191, 3,
 178, 36, 138, 251, 3, 151, 36, 205, 11, 3, 187, 34, 157, 131, 3, 85, 34, 202, 179, 0, 89, 29, 25, 207, 2, 48, 28, 109, 27, 2, 50, 27,
 161, 234, 0, 157, 26, 161, 59, 0, 19, 26, 174, 87, 2, 30, 22, 207 };

  unsigned char quad_sound[]={
      32,
 53, 0, 92, 37, 104, 136, 3, 161, 33, 238, 182, 4, 233, 31, 217, 154, 5, 49, 29, 200, 167, 2, 35, 29, 65, 236, 3, 101, 25, 230, 135, 3,
 55, 25, 113, 11, 2, 143, 24, 30, 14, 2, 48, 24, 179, 238, 3, 93, 23, 28, 76, 5, 246, 22, 104, 91, 0, 14, 22, 198, 169, 2, 5, 20,
 11, 225, 1, 5, 20, 146, 243, 3, 200, 19, 245, 107, 4, 180, 19, 42, 47, 1, 118, 19, 25, 55, 3, 109, 19, 222, 240, 3, 180, 18, 66, 155,
 3, 106, 18, 241, 142, 1, 234, 17, 195, 197, 1, 135, 17, 2, 78, 0, 71, 17, 209, 214, 0, 17, 17, 243, 198, 1, 151, 16, 227, 222, 1, 44,
 16, 175, 9, 2, 34, 16, 2, 187, 2, 10, 16, 118, 83, 0, 229, 15, 110, 182, 1, 2, 15, 97, 41, 0, 144, 14, 76, 217, 1, 118, 14, 137 };


#pragma pack( push, 1 )
struct Harmonic
{
    unsigned short freq;
    unsigned short amplitude;
    unsigned char phase;
};
#pragma pack(pop)




static short Noise( int x ) 
{
    x = (x << 13) ^ x;
    return ((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) /0xffff - 0x7fff;
    
}

static short InterpolatedNoise( unsigned int x, unsigned int k )
{
    unsigned int step= 1<<k;
    unsigned int X = x >> k;

    unsigned int noise[2]=
    {
        Noise(X),
        Noise(X + 1 )
    };
  
    unsigned int dx =  x - (X <<k );
    return ( noise[1] * dx  + noise[0] * (step-dx) )>>k;
}

static short FinalNoise(unsigned int x, unsigned int harmonics= 6 )
{
    short r= 0;
    for( unsigned int i= 0; i<= harmonics; i++ )
        r+= InterpolatedNoise( x, 8 - i )>>(i+1);

    //( int i= 0; i<= 5; i++ )
    //    r+= InterpolatedNoise( x, 6 - i )>>3;

    return r;
}

float AbsoluteNoise( unsigned int x, unsigned int lod )//from 1 to 4410
{
    static float* n_data= NULL;
    const unsigned int sample_len= 65536;
    if( n_data == NULL )
    {
        n_data= new float[ sample_len * 2 ];
        for( int i= 0; i< sample_len; i++ )
            n_data[i]= float(rand()) - 16384.0f;

        float* prev_data= n_data;
        float* next_data;
        for( int j= 1; j < sample_len; j*=2 )
        {
            next_data= prev_data + sample_len/j;
            for( int i= 0, i_end= sample_len/j/2; i< i_end; i++ )
                next_data[i]= ( prev_data[i*2] + prev_data[i*2+1] ) * 0.4995f;
            prev_data= next_data;
        }
    
    }
   
 
    float* d= n_data;
    int j= 0;
    int i= sample_len;
    while( j != lod )
    {
        d+= i;
        i/=2;
        j++;
    }
   return d[ (x>>lod)%i ];

}


void SmoothSound( short* data, int len )
{
    //signal edges smoothing
    const int edge_len= 64;

    for( int i= 0; i< edge_len; i++ )
        data[i]= data[i] * i / edge_len;
    for( int i= len - edge_len; i<  len; i++ )
        data[i]= data[i] * (len-i)/edge_len;
}

unsigned int GenBlasterShot( short** data )
{
    short* d= new short[ 7350 ];

    float a, x;
    float f = 1000.0f, df= -500.0f / 7350.0f;

    for( int i= 0; i< 7350; i++, f+=df )
    {
        a=    MW_2PI * float(i)/float(22050);
               
        x= 16384.0f* cos( a * f );//+ 8192.0f * cos( a * 0.5f ) + 4096.0f * cos( a * 0.25f );
        x*= 0.5f;
        d[i]= short(x);
       // d[i]= ((i>>5)&1) ? 16384 : -16384;//square wave
    }
    
    SmoothSound( d, 7350 );
    *data= d;
    return 7350;
}

unsigned int GenFootStep( short** data )
{
     short* d= new short[ 4410 ];
     for( int i= 0; i< 4410/2; i++ )
         d[i]=  i * FinalNoise(i*5) /( 4410/2);
     for( int i= 4410/2; i< 4410; i++ )
         d[i]=  (4410 - i) *FinalNoise(i*5) / ( 4410/2);

     *data= d;
     return 4410;
}

unsigned int GenPistolShot(  short** data )
{
     short* d= new short[ 22050 ];
       float a= 0.6f;
     float e= 0.9998f;
     for( unsigned int i= 0; i< 22050; i++ )
     {
         unsigned int j= i&0xfffffffc;
        // d[i]= short( a* ( 0.5f * AbsoluteNoise(j, 1) + 0.8f * AbsoluteNoise(j*2, 2) + AbsoluteNoise(j*3, 3) ) )/2; 
         d[i]= short( float( (FinalNoise(i*35, 4) + FinalNoise(i*47, 3)) )*a );
         a*= e;
     }
     *data= d;
     return 22050;
}

unsigned int GenFireSound( short** data )
{
    short* d= new short[ 22050 ];

    short x;
    for( int i= 0; i< 22050; i++ )
    {
       // x= FinalNoise(i*2);
        x= FinalNoise(i*4);
        x=x * FinalNoise(i>>4) / 32768;
        d[i]= x;
    }
    SmoothSound( d, 22050 );

     *data= d;
     return 22050;
}

unsigned int GenShovelHit( short** data )
{
    short* sound_data= new short[ 7350 ];
    float a;
    float x;

    Harmonic* h= (Harmonic*)( crowbar_sound + 1 );
    for( int i= 0; i< 7350; i++ )
    {
        a=    MW_2PI * float(i)/float(22050);       
        x= 0.0f;
        for( unsigned int k=0; k< crowbar_sound[0]; k++ )
            x+= float(h[k].amplitude) * cos( a * float( h[k].freq ) + float(h[k].phase) * MW_2PI );
         x*= 0.25f;

        if( x > 32767.0f ) x= 32767.0f;
        else if( x< -32767.0f ) x= -32767.0f;
        sound_data[i]= short(x);
    }

     SmoothSound( sound_data, 7350 );
    *data= sound_data;
    return 7350;
}

unsigned int (*SoundGenFunc[])( short**)= { GenShovelHit, /*GenBlasterShot*/GenPistolShot, GenBlasterShot };