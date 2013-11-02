#include "landscape.h"
#include "math.h"
#include "texture_generation.h"
#include "level.h"

static unsigned short Noise2( int x,  int y ) 
{
    int n = x + y * 57;
    n = (n << 13) ^ n;
    return ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) /0x7fff;
    
}

static unsigned short InterpolatedNoise( unsigned int x, unsigned int y, unsigned int k)
{
     unsigned int step= 1<<k;
     

    unsigned int X = x >> k;
    unsigned int Y = y >> k;

    unsigned int noise[4]=
    {
        Noise2(X, Y ),
        Noise2(X + 1, Y ),
        Noise2(X + 1, Y + 1 ),
        Noise2(X, Y + 1 )
    };

    unsigned int dx =  x - (X <<k );
    unsigned int dy =  y - (Y <<k );

    unsigned int interp_x[2]=
    {
        dy * noise[3] + (step - dy) * noise[0],
        dy * noise[2] + (step - dy) * noise[1]
    };
    return (interp_x[1] * dx + interp_x[0] * (step - dx) )>>(k+k);
}


static unsigned short FinalNoise(unsigned int x, unsigned int y)
{
    unsigned short r= 0;
    
   // r+= InterpolatedNoise( x, y, 8 )>>1;
    for( int i= 0; i<= 6; i++ )
        r+= InterpolatedNoise( x, y, 8 - i )>>(1+i);

   
    return r;
}





void Landscape::Gen()
{
	unsigned int i, j;

	for( i= 0; i< size_x; i++ )
		for( j= 0; j< size_y; j++ )
			heightmap[ i + j * size_x ]= FinalNoise( i, j );
}

void Landscape::Smooth()
{
    unsigned short* new_hm= new unsigned short[ size_x * size_y ];

    unsigned  i, j;
    int mask_x= size_x-1, mask_y=size_y-1;

    unsigned int r;
    for( i= 0; i< size_x; i++ )
		for( j= 0; j< size_y; j++ )
        {
            r=  heightmap[ i + j * size_x ] << 2;
            r+= heightmap[ ((i+1)&mask_x) + j * size_x ] << 1;
            r+= heightmap[ ((i-1)&mask_x) + j * size_x ] << 1;

            r+= heightmap[ i + ((j+1)&mask_y) * size_x ] << 1;
            r+= heightmap[ i + ((j-1)&mask_y) * size_x ] << 1;

            r+= heightmap[ ((i+1)&mask_x) + ((j+1)&mask_y) * size_x ];
            r+= heightmap[ ((i-1)&mask_x) + ((j+1)&mask_y) * size_x ];
            r+= heightmap[ ((i+1)&mask_x) + ((j-1)&mask_y) * size_x ];
            r+= heightmap[ ((i-1)&mask_x) + ((j-1)&mask_y) * size_x ];

            new_hm[ i + j * size_x ]= r>>4;
        }

    delete[] heightmap;
    heightmap= new_hm;
}

void Landscape::GenWay()
{
    for( unsigned int i= 0; i< size_x; i++ )
    {
        for( unsigned int j= 0; j< size_y; j++ )
            texture_map[ i + size_x * j ]= TEXTURE_GRASS;

    }
    
    /* for( unsigned int i= 16; i< size_x-16; i++ )
    {
        for( int j= -7; j< 7; j++ )
            texture_map[ j+ i + i * size_x ] = TEXTURE_PAVING_STONE;
       

       
         for( int j= -8; j< 8; j++ )
         {
            heightmap[ j + i + i * size_x ]&= 0xFF00;
            heightmap[ j + i + i * size_x ]-= 256;
            free_space_map[ j + i + i * size_x ]= false;
         }
        
    }*/
    for( int i= 512; i< 512+8; i++ )
        for( int j= 484; j< 484 + 8*TEXTURE_COUNT; j++ )
             texture_map[ i + j * size_x ] = (j-484)>>3;

 
}





Landscape::Landscape( unsigned int size_x, unsigned int size_y )
{
    amplitude= 128.0f;
	this->size_x= size_x;
	this->size_y= size_y;

	heightmap= new unsigned short[ size_x * size_y ];
    texture_map= new unsigned char[ size_x * size_y ];
    free_space_map= new bool[ size_x * size_y ];


    object_map= new StaticObject*[ size_x * size_y ];
    for( unsigned int i= 0, i_end= size_x * size_y; i< i_end; i++ )
        object_map[i]= NULL, free_space_map[i]= true;

	Gen();
    Smooth();
   
    GenWay();
}

void Landscape::SetTexture( unsigned int x, unsigned int y, unsigned char texture_id, unsigned int radius )
{
    unsigned int x_end= min( x + radius, size_x ), y_end= min( y + radius, size_y );
    int r2= radius * radius;
    for( unsigned int i= max( x - radius, 0 ); i<= x_end; i++ )
        for( unsigned int j= max( y - radius, 0 ); j<= y_end; j++ )
        {
            int dx= i - x, dy= j - y;
            if( dx * dx + dy * dy <= r2 )
            {
                texture_map[ i + j * size_x ]= texture_id;
            }
        }
}

void Landscape::SetTexture( unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned char texture_id )
{
    unsigned int x, y;
    for( x= x0; x<= x1; x++ )
        for( y= y0; y<= y1; y++ )
            texture_map[x + y * size_x ]= texture_id;
}


void Landscape::SetMaximumHeight( unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1  )
{
    unsigned short h_max= 0;
    unsigned int x, y;
    for( x= x0; x<= x1; x++ )
        for( y= y0; y<= y1; y++ )
            if( heightmap[x + y * size_x ] > h_max ) h_max= heightmap[x + y * size_x ];

    for( x= x0; x<= x1; x++ )
        for( y= y0; y<= y1; y++ )
            heightmap[x + y * size_x ]= h_max;
}

void Landscape::SetMinimumHeight( unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1  )
{
    unsigned short h_min= 65535;
    unsigned int x, y;
    for( x= x0; x<= x1; x++ )
        for( y= y0; y<= y1; y++ )
            if( heightmap[x + y * size_x ] < h_min ) h_min= heightmap[x + y * size_x ];

    for( x= x0; x<= x1; x++ )
        for( y= y0; y<= y1; y++ )
            heightmap[x + y * size_x ]= h_min;
}


void Landscape::PlaceObject( unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, StaticObject* object )
{
    object_map[ (x0+x1)/2 + ((y0+y1)/2) * size_x ]= object;
    unsigned int x, y;
    for( x= x0; x<= x1; x++ )
        for( y= y0; y<= y1; y++ )
            free_space_map[ x + y * size_x ]= false;//object_map[ x + y * size_x ]= object;

}

bool Landscape::CanPlaceObject( unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1 )
{
    unsigned int x, y;
    for( x= x0; x<= x1; x++ )
        for( y= y0; y<= y1; y++ )
            if( ! free_space_map[ x + y * size_x ] )
                return false;
    return true;

}

float Landscape::Height( float x, float y )const
{
    if( x <= 0.0f || x>=float(size_x) || y <= 0.0f || y>=float(size_y) )
        return 0.0f;

    unsigned int i_x, i_y;
    i_x= int(x);
    i_y= int(y);

    float dx, dy;
    dx= x - float(i_x);
    dy= y - float(i_y);

    float interp_x[2]=
    {
        dy * float(heightmap[ i_x + (i_y+1) * size_x  ]) + (1.0f - dy) * float(heightmap[ i_x + i_y * size_x  ]),
        dy * float(heightmap[ i_x+1 + (i_y+1) * size_x  ]) + (1.0f - dy) * float(heightmap[ i_x+1 + i_y * size_x  ])
    };

      return amplitude * (interp_x[1] * dx + interp_x[0] * (1.0f - dx) ) / 65535.0f;
}

void Landscape::Normal( float x, float y, float* n )const
{
    if( x <= 0.0f || x>=float(size_x) || y <= 0.0f || y>=float(size_y) )
        return;

    unsigned int i_x, i_y;
    i_x= int(x);
    i_y= int(y);

    n[0]= float( heightmap[ i_x - 1 + i_y * size_x ] - heightmap[ i_x + 1 + i_y * size_y ] );
    n[1]= float( heightmap[ i_x + (i_y-1) * size_x ] - heightmap[ i_x + (i_y+1) * size_y ] );
    
    n[2]= 1.0f / amplitude;

    Vec3Normalize(n);


}