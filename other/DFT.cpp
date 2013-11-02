#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <windows.h>

#define f_type double

#define M_2PI 6.283185307179586476925286766559

typedef struct  WaveHeader
{
    char                RIFF[4];        // RIFF Header      Magic header
    unsigned long       ChunkSize;      // RIFF Chunk Size
    char                WAVE[4];        // WAVE Header
    char                fmt[4];         // FMT header
    unsigned long       Subchunk1Size;  // Size of the fmt chunk
    unsigned short      AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    unsigned short      NumOfChan;      // Number of channels 1=Mono 2=Sterio
    unsigned long       SamplesPerSec;  // Sampling Frequency in Hz
    unsigned long       bytesPerSec;    // bytes per second
    unsigned short      blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    unsigned short      bitsPerSample;  // Number of bits per sample
    char                Subchunk2ID[4]; // "data"  string
    unsigned long       Subchunk2Size;  // Sampled data length - //DATA SIZE!

};


typedef struct WavSound
{
    void* data;
    unsigned int data_size;
    unsigned int freq;
    unsigned int bits_per_sample;

};


struct HarmonicData
{
    unsigned short freq;//[0;65536]
    unsigned short amplitude;//[0;1]
    unsigned char phase;//[0;pi]
};


void LoadWAV( const char* file_name, WavSound* sound )
{
    WaveHeader wave_header;

    FILE* f= fopen( file_name, "rb" );
    if( f == NULL )
    {
        printf( "can notopen file \"%s\"\n", file_name );
        exit(-1);
    }

    fread( &wave_header,1, sizeof(WaveHeader), f );


    unsigned int pos= ftell(f);
    fseek( f, 0, SEEK_END );
    sound->data_size= ftell(f) - pos;

    fseek( f, pos, SEEK_SET );

    printf( "data size: %d\n", sound->data_size );

    sound->freq= wave_header.bytesPerSec / ( wave_header.bitsPerSample / 8 );
    //sound->data_size= wave_header.Subchunk2Size;
    sound->data= new char[ sound->data_size ];
    sound->bits_per_sample= wave_header.bitsPerSample;

    fread( sound->data, 1, sound->data_size, f );

    fclose(f);

}




#define max(x,y) (((x)>(y))?(x):(y))
#define min(x,y) (((x)<(y))?(x):(y))


f_type fract( f_type x, f_type y )
{
    int d= int( x / y );
    f_type i_x= (f_type)(d) * y;

    return x - i_x;
}


void GetAmplitudes( unsigned int bpp, const void* data, unsigned int freq, f_type t0, f_type t1, HarmonicData* harmonic_data, unsigned int harmonics )
{
    unsigned int N= int( (t1 - t0) * (f_type)(freq) );

    f_type* out_data= new f_type[N*2];
    f_type* in_data= new f_type[N];

    unsigned int i, n, k;
    f_type nN, dnN, fk;
    dnN= -2.0 * M_PI / (f_type)(N);

    if( bpp == 8 )
    {
        char* d= (char*) data;
        for( i= 0; i< N; i++  )
        {
            in_data[i]= ((f_type)d[i])* 0.0078125;// to [-1;1)
            // printf( "%f\n", in_data[i] );
            out_data[i*2]= 0.0;
            out_data[i*2+1]= 0.0;
        }
    }
    else
    {
        short* d= (short*) data;
        for( i= 0; i< N; i++  )
        {
            in_data[i]= ((f_type)d[i])* 0.000030517578125;// to [-1;1)
            // printf( "%f\n", in_data[i] );
            out_data[i*2]= 0.0;
            out_data[i*2+1]= 0.0;
        }
    }

    for( k= 0; k< N; k++ )//DFT
    {
        for( n= 0, nN= 0.0, fk= (f_type)(k); n< N; n++, nN+= dnN )
        {
            f_type a= f_type( M_2PI ) * f_type(n) * f_type(k) / f_type(N);
            out_data[k*2]+=  in_data[n] * cos( /*nN * fk*/a );
            out_data[k*2+1]+=  in_data[n] * sin( /*nN * fk*/a );
            if( nN > 2.0 * M_PI )
                nN-= 2.0 * M_PI;
        }
    }

    //complex amplitude -> amplitude + phase

    f_type inv_N= 1.0 / (f_type)(N);
    for( i= 0; i< N; i++ )
    {
        f_type a= sqrt( out_data[i*2] * out_data[i*2] + out_data[i*2+1] * out_data[i*2+1] );
        f_type p= acos( out_data[i*2] / a );


        out_data[i*2]= a * inv_N;
        out_data[i*2+1]= p;
    }


    f_type top_a[harmonics];
    f_type top_f[harmonics];
    f_type top_phi[harmonics];
    for( i= 0; i< harmonics; i++ )
        top_a[i]= float(harmonics-i)* 0.000001f;
    k= 0;

    for( i= int( 40.0f*(t1-t0))/*40 Hz - minfreq*/; i< N/2; i++ )
    {
        if( top_a[harmonics-1] <= out_data[i*2] )
        {

            int in;
            for( in= harmonics - 1; in >= 0; in-- )
            {
                if( out_data[i*2] > top_a[in] )
                {
                    top_a[in]= top_a[in-1];
                    top_f[in]= top_f[in-1];
                    top_phi[in]= top_phi[in-1];
                }
                else
                {
                    top_a[in+1]= out_data[i*2];
                    top_f[in+1]= f_type( i );
                    top_phi[in+1]= out_data[i*2+1];
                    break;
                }
            }
            if( in < 0 )
            {
                top_a[0]= out_data[i*2];
                top_f[0]= f_type( i );
                top_phi[0]= out_data[i*2+1];
            }

            k++;
        }
    }

    for( i= 0; i< harmonics; i++ )
    {
        printf( "A: %6.4f, f: %f, phi: %f\n", top_a[i], top_f[i]/(t1-t0), top_phi[i] * 180.0 / M_PI );
        harmonic_data[i].amplitude= min( (unsigned short)( top_a[i] * 65535.0f * 2.0f ), 65535 );
        harmonic_data[i].phase= (unsigned char) (top_phi[i] / M_PI * 255.0f);
        harmonic_data[i].freq= (unsigned short)( top_f[i]/(t1-t0) );
    }



}


void SaveHarmonics2File( const char* file_name, HarmonicData* harmonics, unsigned int harmonic_count, float len )
{
    FILE* f= fopen( file_name, "wb" );
    if( f == NULL )
    {
        printf( "error, cannot open file \"%s\"\n", file_name );
        return;
    }
    unsigned char count= (unsigned char)(harmonic_count);
    fwrite( &count,1, 1, f );

    count= (unsigned char)( len * 64.0f );//length of sample i unit - 1/64 s
    fwrite( &count,1, 1, f );

    for( int i= 0; i< harmonic_count; i++ )
    {
        fwrite( &harmonics[i].freq,1, sizeof(short), f );
        fwrite( &harmonics[i].amplitude,1, sizeof(short), f );
        fwrite( &harmonics[i].phase,1, sizeof(char), f );
    }

    fclose(f);
}

int main( int argc, char* argv[])
{

    HarmonicData harmonics[128];
    unsigned int harmonic_count= 16;
    WavSound ws;

    char file_name[128];
    char out_file_name[128];
    float t0= 0.0f, t1= 0.3f;

    bool is_in_file= false, is_out_file= false;
    for( int i= 1; i< argc; i++ )
    {
        if( !strcmp( "-o", argv[i] ) )
        {
            if( i != argc - 1 )
            {
                is_out_file= true;
                strcpy( out_file_name, argv[i+1] );
                i++;
                continue;
            }
            else
            {
                printf( "error, need out file name before -o\n" );
                exit(-1);
            }
        }
        else if( !strcmp( "-c", argv[i] ) )
        {
            if( i != argc - 1 )
            {
                harmonic_count= atoi( argv[i+1] );
                i++;
                continue;
            }
            else
            {
                printf( "error, need number before -c\n" );
                exit(-1);
            }
        }
        else if( !strcmp( "-t0", argv[i] ) )
        {
            if( i != argc - 1 )
            {
                t0= atof( argv[i+1] );
                i++;
                continue;
            }
            else
            {
                printf( "error, need number before -t0\n" );
                exit(-1);
            }
        }
          else if( !strcmp( "-t1", argv[i] ) )
        {
            if( i != argc - 1 )
            {
                t1= atof( argv[i+1] );
                i++;
                continue;
            }
            else
            {
                printf( "error, need number before -t1\n" );
                exit(-1);
            }
        }

        else
        {
            strcpy( file_name, argv[i] );
            is_in_file= true;
        }
    }

    if( ! is_out_file )
    {
        printf( "error, need int file\n" );
        exit(-1);
    }
    if( ! is_out_file )
    {
        printf( "error, need out file\n" );
        exit(-1);

    }

    /* printf( "enter file name: " );
     scanf( "%s", file_name );
     printf( "\nenter out file name: " );
     scanf( "%s", out_file_name );

     printf( "\nenter interval ( t1, t2 ): " );
     scanf( "%f %f", &t1, &t2 );

     printf( "dt: %f\n", t2 - t1 );*/

    LoadWAV( file_name, &ws );
    t1= min( t1, float(ws.data_size) / float(ws.freq) /float(ws.bits_per_sample/8) );
    printf( "dt: %f\n", t1 - t0 );

    GetAmplitudes( ws.bits_per_sample, ws.data, ws.freq, t0, t1, harmonics, harmonic_count );

    SaveHarmonics2File( out_file_name, harmonics, harmonic_count, t1 - t0 );
    return 0;
}
