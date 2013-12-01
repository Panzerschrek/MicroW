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

void Landscape::RegenHeightmap()
{
    Gen();
    Smooth();
    GenWay();
}

void Landscape::GenWay()
{
	//mountains
    for( unsigned int i= 0; i< size_x; i++ )
    {
        for( unsigned int j= 0; j< size_y; j++ )
        {
            texture_map[ i + size_x * j ]= TEXTURE_GRASS;
            float n[3];
            Normal( float(i), float(j), n );
            unsigned int h= heightmap[ i + j * size_x ];
            unsigned short stone_level= 45874, snow_level= 47840;
            if( h>= stone_level )
            {
                h+= ( h - stone_level );
                if( h > 64435 )h= 65535;
                free_space_map[ i + j * size_x]= false;
                heightmap[ i + j * size_x ]= h;
                if( h >= snow_level )
                    texture_map[ i + j * size_x ]= TEXTURE_SNOW;
                else
                    texture_map[ i + j * size_x ]= TEXTURE_STONE;


            }
        }
    }

	//water
	for( unsigned int j= 0; j< size_y; j++ )
		for( unsigned int i= 0; i< size_x; i++ )
		{
			if( heightmap[ i + j*size_y ] < 65535*9/20 )
            {
                texture_map[ i + j * size_x ]= TEXTURE_SAND;
                free_space_map[ i + j * size_x]= false;
            }
        }

	//roads
    /*static const int way_width= 3;
    for( unsigned int i= 20; i< size_x-20; i++ )
    {
        float n[3];
        Normal( float(i), 512.0f, n );
        short dy= short( n[1] * 12.0f );
        for( unsigned int j= 512-way_width; j< 512+way_width; j++ )
        {
			unsigned int k= i + (j+dy) * size_x;
			if( free_space_map[k] )
			{
				texture_map[k]= TEXTURE_PAVING_STONE;
				heightmap[k]&= 0xFFF0;
				heightmap[k]+= 64;
				//heightmap[k]= heightmap[i + 512 * size_x ];
				free_space_map[k]= false;
			}
        }
    }

	for( unsigned int j= 20; j< size_x-20; j++ )
    {
        float n[3];
        Normal( 512.0f, float(j), n );
        short dx= short( n[0] * 12.0f );
        for( unsigned int i= 512-way_width; i< 512+way_width; i++ )
        {
			unsigned int k= i + dx + j * size_x;
			if( free_space_map[k] )
			{
				texture_map[k]= TEXTURE_PAVING_STONE;
				heightmap[k]&= 0xFFF0;
				heightmap[k]+= 64;
				//heightmap[k]= heightmap[i + 512 * size_x ];
				free_space_map[k]= false;
			}
        }
    }*/

	//central square
    const int central_square_width= 6;
    for( unsigned int i= 512-central_square_width; i< 512+central_square_width; i++ )
        for( unsigned int j= 512-central_square_width; j< 512+central_square_width; j++ )
            texture_map[ i + j * size_x ]= TEXTURE_PAVING_STONE;
    SetMaximumHeight( 512-central_square_width, 512-central_square_width, 512+central_square_width, 512+central_square_width );
   
  /*  for( int i= 512; i< 512+8; i++ )
        for( int j= 484; j< 484 + 8*TEXTURE_COUNT; j++ )
             texture_map[ i + j * size_x ] = (j-484)>>3;
*/
 
}

void Landscape::GenShadowMap( const float* sun_vector )
{
    float sun[]= { sun_vector[0], sun_vector[1], sun_vector[2] };
    Vec3Normalize( sun );
    float xy_len= sqrt( sun[0]*sun[0] + sun[1] * sun[1] );
    Vec3Mul( sun, 1.0f / xy_len );

    float pos[3];
    unsigned char in_shadow;
    for( unsigned int j= 0; j< size_y; j++ )
        for( unsigned int i= 0; i< size_x; i++ )
        {
            pos[0]= float(i);
            pos[1]= float(j);
            pos[2]= Height( pos[0], pos[1] ) + 0.05f;
            in_shadow= 255;
            while( pos[2] < amplitude )
            {
                Vec3Add( pos, sun );
                if( pos[2] <= Height( pos[0], pos[1] ) )
                {
                    in_shadow= 0;
                    break;
                }
            }
            shadow_map[ i + j*size_x ]= in_shadow;

        }
}



Landscape::Landscape( unsigned int size_x, unsigned int size_y )
{
    amplitude= 128.0f;
	this->size_x= size_x;
	this->size_y= size_y;

	heightmap= new unsigned short[ size_x * size_y ];
    texture_map= new unsigned char[ size_x * size_y ];
    free_space_map= new bool[ size_x * size_y ];
    shadow_map= new unsigned char [ size_x * size_x ];


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
    if( x <= 0.5f || x>=float(size_x-2) || y <= 0.5f || y>=float(size_y-2) )
        return 0.0f;

    unsigned int i_x, i_y;
    i_x= (unsigned int)(x);
    i_y= (unsigned int)(y);
   // return amplitude * float(heightmap[ i_x + i_y * size_x ] ) / 65535.0f;
    float dx, dy;
    dx= x - float(i_x);
    dy= y - float(i_y);

    float interp_x[2]=
    {
        dy *float(heightmap[ i_x + (i_y+1) * size_x ]) + (1.0f - dy) *float(heightmap[ i_x + i_y * size_x ]),
        dy *float(heightmap[ i_x+1 + (i_y+1) * size_x ]) + (1.0f - dy) *float(heightmap[ i_x+1 + i_y * size_x ])
    };

      return amplitude * (interp_x[1] * dx + interp_x[0] * (1.0f - dx) )/65535.0f;
}

void Landscape::Normal( float x, float y, float* n )const
{
    if( x <= 0.0f || x>=float(size_x) || y <= 0.0f || y>=float(size_y) )
        return;

    unsigned int i_x, i_y;
    i_x= int(x);
    i_y= int(y);

    n[0]= float( heightmap[ i_x - 1 + i_y * size_x ] - heightmap[ i_x + 1 + i_y * size_x ] );
    n[1]= float( heightmap[ i_x + (i_y-1) * size_x ] - heightmap[ i_x + (i_y+1) * size_x ] );
    n[0]/=65535.0f;
    n[1]/=65535.0f;
    
    n[2]= 1.0f / amplitude;

    Vec3Normalize(n);
}