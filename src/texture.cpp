#include "texture.h"
#include "gl_functions.h"
#include "text.h"

unsigned char Texture::default_clear_color[4]= { 0,0,0,255 };
Texture* Texture::default_1024_noise= NULL;


Texture::Texture( unsigned int log2_x, unsigned int log2_y )
{
    size_x= 1<<log2_x;
    size_y= 1<<log2_y;

    size_x_log2= log2_x;
    size_y_log2= log2_y;
    tex_id= 0xffffffff;

    data= new unsigned char[ 4 * size_x * size_y ];
}


void Texture::Add( const Texture* t )
{
    unsigned char *d_dst, *d_end, *d_src;
    d_dst= data;
    d_end= data + ((size_x * size_y )<<2);
    d_src= t->data;

    for( ; d_dst!= d_end; d_dst+=4, d_src+=4 )
    {
        for( int i= 0; i< 3; i++ )
            d_dst[i]= min(d_dst[i] + d_src[i], 255 );
    }
}

void Texture::Sub( const Texture* t )
{
    unsigned char *d_dst, *d_end, *d_src;
    d_dst= data;
    d_end= data + ((size_x * size_y )<<2);
    d_src= t->data;

    for( ; d_dst!= d_end; d_dst+=4, d_src+=4 )
    {
        for( int i= 0; i< 3; i++ )
            d_dst[i]= max(d_dst[i] - d_src[i], 0 );
    }
}

void Texture::Copy( Texture* t )
{
    unsigned char *d_dst, *d_end, *d_src;
    d_dst= data;
    d_end= data + ((size_x * size_y )<<2);
    d_src= t->data;
    
     for( ; d_dst!= d_end; d_dst++, d_src++ )
         *d_dst= *d_src;
}

void Texture::AlphaBlend( const Texture* t )
{
    unsigned char *d_dst, *d_end, *d_src;
    d_dst= data;
    d_end= data + ((size_x * size_y )<<2);
    d_src= t->data;

    unsigned char a0, a1;
    for( ; d_dst!= d_end; d_dst+=4, d_src+=4 )
    {
        a0= d_src[3];
        a1= ~a0;
        for( int i= 0; i< 3; i++ )
            d_dst[i]= (d_dst[i] * a0 + d_src[i] * a1)>>8;
    }
}

void Texture::Mul( const Texture* t )
{
     unsigned char *d_dst, *d_end, *d_src;
    d_dst= data;
    d_end= data + ((size_x * size_y )<<2);
    d_src= t->data;

    for( ; d_dst!= d_end; d_dst+=4, d_src+=4 )
    {
        for( int i= 0; i< 3; i++ )
            d_dst[i]= (d_dst[i]*d_src[i])>>8;
    }
}

void Texture::Add( unsigned char c )
{
    unsigned char *d, *d_end;
    d= data;
    d_end= data + ((size_x*size_y)<<2);
    for( ; d!=d_end; d+=4 )
        for( int i= 0; i< 3; i++ )
            d[i]= min( d[i] + c, 255 );
}
void Texture::Sub( unsigned char c )
{
     unsigned char *d, *d_end;
    d= data;
    d_end= data + ((size_x*size_y)<<2);
    for( ; d!=d_end; d+=4 )
        for( int i= 0; i< 3; i++ )
            d[i]= max( d[i] - c, 0 );
}
void Texture::Mul( unsigned char c )
{
     unsigned char *d, *d_end;
    d= data;
    d_end= data + ((size_x*size_y)<<2);
    for( ; d!=d_end; d+=4 )
        for( int i= 0; i< 3; i++ )
            d[i]= (d[i] * c)>>8;
}


void Texture::Add( const unsigned char* c )
{
    unsigned char *d, *d_end;
    d= data;
    d_end= data + ((size_x*size_y)<<2);
    for( ; d!=d_end; d+=4 )
        for( int i= 0; i< 3; i++ )
            d[i]= min( d[i] + c[i], 255 );
}
void Texture::Sub( const unsigned char* c )
{
     unsigned char *d, *d_end;
    d= data;
    d_end= data + ((size_x*size_y)<<2);
    for( ; d!=d_end; d+=4 )
        for( int i= 0; i< 3; i++ )
            d[i]= max( d[i] - c[i], 0 );
}
void Texture::Mul( const unsigned char* c )
{
     unsigned char *d, *d_end;
    d= data;
    d_end= data + ((size_x*size_y)<<2);
    for( ; d!=d_end; d+=4 )
        for( int i= 0; i< 3; i++ )
            d[i]= (d[i] * c[i])>>8;
}

void Texture::SetAlpha( unsigned char a )
{
    unsigned int k_end= size_x * size_y * 4;
    for( unsigned int k= 3; k< k_end; k+= 4 )
        data[k]= a;
}

void Texture::Move2GPU()
{
    glGenTextures( 1, &tex_id );
    glBindTexture( GL_TEXTURE_2D, tex_id );

     glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8,
                  size_x, size_y, 0,
                  GL_RGBA,  GL_UNSIGNED_BYTE, data );
    
    glGenerateMipmap( GL_TEXTURE_2D );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

}
void Texture::Bind( unsigned int unit )
{
    glActiveTexture( GL_TEXTURE0 + unit );
    glBindTexture( GL_TEXTURE_2D, tex_id );
}
void Texture::Clear( const unsigned char* clear_color )
{
    unsigned int k;
    /*for( unsigned int i= 0; i!= size_x; i++ )
        for( unsigned int j= 0; j!= size_y; j++ )
        {
            k= (i + (j << size_x_log2) )<<2;
            data[k    ]= clear_color[0];
            data[k + 1]= clear_color[1];
            data[k + 2]= clear_color[2];
            data[k + 3]= clear_color[3];
        }*/
    unsigned int k_end= size_x * size_y * 4;
    for( k= 0; k< k_end; k+=4 )
    {
        data[k]= clear_color[0];
        data[k+1]= clear_color[1];
        data[k+2]= clear_color[2];
        data[k+3]= clear_color[3];
    }
}

void Texture::DrawCircle( short x, short y, short r, const unsigned char* color )
{
    int x_end,x0, y_end,y0,  r2, k;
    x_end= x + r;
    y_end= y + r;
    r2= r*r;
    int mask_x, mask_y;
    mask_x= size_x-1;
    mask_y= size_y-1;


    for( x0= x - r; x0< x_end; x0++ )
        for( y0= y - r; y0< y_end; y0++ )
        {
            if( (( x-x0)*(x-x0)+(y-y0)*(y-y0)) <= r2 )
            {
                k= ((x0&mask_x) + ((y0&mask_y) << size_x_log2) )<<2;
                data[k    ]= color[0];
                data[k + 1]= color[1];
                data[k + 2]= color[2];
               // data[k + 3]= color[3];
            }
        }
}

void Texture::FillRect( short x0, short y0, short x1,short y1, const unsigned char* color )
{
    int x, y, k;
    int mask_x, mask_y;
    mask_x= size_x-1;
    mask_y= size_y-1;

    for( x= x0; x<= x1; x++ )
        for( y= y0; y<= y1; y++ )
        {
            k= ((x&mask_x) + ((y&mask_y) << size_x_log2) )<<2;
                data[k    ]= color[0];
                data[k + 1]= color[1];
                data[k + 2]= color[2];
                data[k + 3]= color[3];
        }
}


void Texture::DrawText( short x, short y, unsigned int size, const unsigned char* color, const char* text )
{
    unsigned int mask_x= size_x - 1, mask_y = size_y - 1;
    unsigned int d;
    short x0= x;
    unsigned char* text_data= Text::DefaultText()->text_texture_data;
    while( *text != 0 )
    {
        if( *text == '\n' )
        {
            y-= LETTER_HEIGHT*size;
            x=x0;
            text++;
            continue;
        }

        for( unsigned int i= 0; i< LETTER_WIDTH*size; i++ )
            for( unsigned int j= 0; j< LETTER_HEIGHT*size; j++ )
            {
                 d= i/size + (*text - 32)*LETTER_WIDTH  + j/size * FONT_BITMAP_WIDTH;
                if( text_data[d] != 0 )
                {
                    d= (((i+x)&mask_x) + (((j+y)&mask_y)<<size_x_log2))<<2;
                    for( unsigned int k= 0; k< 3; k++ )
                        data[d+k]= color[k];
                    
                }
            }
        text++;
        x+= LETTER_WIDTH*size;
    }
}

void Texture::DrawTriangle( const short* coord, const unsigned char* color )
{
    //coord - {x,y}
    unsigned int upper_vertex, bottom_vertex, middle_vertex;//indeces of vertices * 4
    int x_mask, y_mask, k;
     x_mask= size_x-1;
    y_mask= size_y-1;

    upper_vertex= ( coord[0+1] > coord[2+1] ) ? 0 : 2;
    upper_vertex= ( coord[upper_vertex+1] > coord[4+1] ) ? upper_vertex : 4;

    bottom_vertex= ( coord[0+1] < coord[2+1] ) ? 0 : 2;
    bottom_vertex= ( coord[bottom_vertex+1] < coord[4+1] ) ? bottom_vertex : 4;


    unsigned int vert_ind_sum= upper_vertex + bottom_vertex;
    if( vert_ind_sum == 2 ) middle_vertex= 4;
    else if( vert_ind_sum == 4 ) middle_vertex= 2;
    else middle_vertex= 0;

    short x, x_begin, x_end;
    short y, y_begin, y_end;
    short tmp;

    y= y_begin= coord[bottom_vertex+1];
    y_end= coord[middle_vertex+1];

    short dy1= coord[ middle_vertex + 1 ] - coord[ bottom_vertex + 1];
    short dy2= coord[ upper_vertex + 1] - coord[ bottom_vertex + 1];


    unsigned char* pixels;

    /*
    ...............
    ..+--...........
    ...\.--.........
    ....\..--.......
    .....\...--.....
    ......\....--...
    .......\.-----+.
    ........\%%%%%/.bottom half-tiangle
    .........\%%%/..
    ..........\%/...
    ...........+....
    ................
    */
    for( ; y<= y_end; y++ )
    {
        if( dy1 != 0 )
            x_begin= ( coord[ middle_vertex ] * ( y - coord[ bottom_vertex + 1] )
                       + coord[ bottom_vertex ] * ( coord[ middle_vertex + 1] - y ) ) / dy1;
        else x_begin= coord[ middle_vertex ];

        if( dy2 != 0 )
            x_end= (  coord[ upper_vertex ] * ( y - coord[ bottom_vertex + 1] )
                      + coord[ bottom_vertex] * ( coord[ upper_vertex + 1] - y ) ) / dy2;
        else x_end= coord[ upper_vertex ];

        if( x_end < x_begin )
        {
            tmp= x_end;
            x_end= x_begin;
            x_begin= tmp;
        }

        pixels= data +  ( ( (y&y_mask) * size_x ) <<2 );
        for( x= x_begin; x< x_end; x++ )
        {
             k= ((x&x_mask)<<2);
             pixels[k  ]= color[0];
			 pixels[k+1]= color[1];
			 pixels[k+2]= color[2];
			 pixels[k+3]= color[3];
        }
    }


    /*
    .................
    ..+--............
    ...\%--..........
    ....\%%--........
    .....\%%%--......
    ......\%%%%--....
    .......\.-----+..
    ........\...../..upper half-tiangle
    .........\.../...
    ..........\./....
    ...........+.....
    .................
    */

    y= y_begin= y_end + 1;
    y_end= coord[ upper_vertex  + 1 ];

    dy1= coord[ upper_vertex + 1 ] - coord[ bottom_vertex + 1];
    dy2= coord[ upper_vertex + 1 ] - coord[ middle_vertex + 1];


    for( ; y< y_end; y++ )
    {
        if( dy1!= 0 )
            x_begin= ( coord[ bottom_vertex ] * ( coord[ upper_vertex + 1 ] - y )
                       + coord[ upper_vertex ] * ( y - coord[ bottom_vertex + 1 ] ) ) / dy1;
        else
            x_begin= coord[ bottom_vertex ];

        if( dy2!= 0 )
            x_end= ( coord[ middle_vertex ] * ( coord[ upper_vertex + 1 ] - y )
                     + coord[ upper_vertex ] * ( y - coord[ middle_vertex + 1 ] ) ) / dy2;
        else
            x_begin= coord[ middle_vertex ];

        if( x_end < x_begin )
        {
            tmp= x_end;
            x_end= x_begin;
            x_begin= tmp;
        }

        pixels= data +  ( ( (y&y_mask) * size_x ) <<2 );
        for( x= x_begin; x< x_end; x++ )
        {
             k= ((x&x_mask)<<2);
             pixels[k  ]= color[0];
			 pixels[k+1]= color[1];
			 pixels[k+2]= color[2];
			 pixels[k+3]= color[3];
        }
    }


}



unsigned char Texture::Noise2( int x,  int y, unsigned int mask  ) 
{
    x&= mask;
    y&= mask;

    int n = x + y * 57;
    n = (n << 13) ^ n;
    return ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) /0x7fffff;
    
}

unsigned char Texture::InterpolatedNoise( unsigned int x, unsigned int y, unsigned int k)
{
     unsigned int step= 1<<k;
     unsigned int mask= (size_x>>k)-1;//DO NOT TOUCH! This value make noise tilebale!

    unsigned int X = x >> k;
    unsigned int Y = y >> k;

    unsigned int noise[4]=
    {
        Noise2(X, Y, mask),
        Noise2(X + 1, Y, mask),
        Noise2(X + 1, Y + 1, mask),
        Noise2(X, Y + 1, mask)
    };

    unsigned int dx =  x - (X <<k );
    unsigned int dy =  y - (Y <<k );

    unsigned int interp_x[2]=
    {
        dy * noise[3] + (step - dy) * noise[0],
        dy * noise[2] + (step - dy) * noise[1]
    };
    return (interp_x[1] * dx + interp_x[0] * (step - dx) )>>(k+k);
  //  return noise[0];
}


unsigned char Texture::FinalNoise(unsigned int x, unsigned int y)
{
    unsigned char r=0;
 
   // r= InterpolatedNoise( x, y, 8 );
    //for( int i= 0, j= size_x_log2; i< size_x_log2; i++,j-- )
    for( int i= 1, j= 7; i< 8; i++, j-- )
        r+= InterpolatedNoise(x,y,i)>>j;

    
    return r;
}

void Texture::Smooth()
{
    unsigned char* new_d= new unsigned char[ size_x * size_y * 4 ];

    unsigned  int i, j, k;
    int mask_x= size_x-1, mask_y=size_y-1;

    unsigned int r;
    for( i= 0; i< size_x; i++ )
		for( j= 0; j< size_y; j++ )
        {
            for( k= 0; k< 4; k++ )
            {
            r=  data[ ( i + j * size_x )*4 + k ] << 2;
            r+= data[ (((i+1)&mask_x) + j * size_x)*4 + k ] << 1;
            r+= data[ (((i-1)&mask_x) + j * size_x)*4 + k ] << 1;

            r+= data[ ( i + ((j+1)&mask_y) * size_x )*4 + k ] << 1;
            r+= data[ ( i + ((j-1)&mask_y) * size_x )*4 + k ] << 1;

            r+= data[ ( ((i+1)&mask_x) + ((j+1)&mask_y) * size_x )*4 + k ];
            r+= data[ ( ((i-1)&mask_x) + ((j+1)&mask_y) * size_x )*4 + k ];
            r+= data[ ( ((i+1)&mask_x) + ((j-1)&mask_y) * size_x )*4 + k ];
            r+= data[ ( ((i-1)&mask_x) + ((j-1)&mask_y) * size_x )*4 + k ];

            new_d[ (i + j * size_x)*4 + k ]= r>>4;
            }
        }

    delete[] data;
    data= new_d;
}

void Texture::Noise()
{
    unsigned int i, j, k;

    if( this != default_1024_noise )
        if( size_x == size_y && size_x == 1024 )
        {
            if( default_1024_noise == NULL )
            {
                default_1024_noise= new Texture(10,10);
                default_1024_noise->Clear();
                default_1024_noise->Noise();
            }
            memcpy( data, default_1024_noise->data, sizeof(char)*4*1024*1024 );
            return;
        }

	for( i= 0; i< size_x; i++ )
		for( j= 0; j< size_y; j++ )
        {
            k= ( i + (j<<size_x_log2) )<<2;
			data[k]= FinalNoise(i,j);
            data[k+1]= data[k+2]= data[k];
        }
}



/*void Texture::DownScale( unsigned int scale_log2 )
{
    unsigned char* new_d= new unsigned char[ size_x * size_x * 4 ];
    unsigned int scale= 1<<scale_log2;

    unsigned short pixel[4];
    unsigned int d;

    unsigned int mask_x= size_x-1, mask_y= size_y - 1;
    for( unsigned int i= 0; i!=size_x; i++ )
        for( unsigned int j= 0; j!= size_y; j++ )
        {
            for( unsigned int k= 0; k!= 4; k++ )
                pixel[k]= 0;

            for( unsigned int n= 0; n!= scale; n++ ) 
            {
                for( unsigned int m= 0; m!= scale; m++ )
                {
                    d= ((mask_x&((i<<scale_log2)+m)) + ((mask_y&((j<<scale_log2)+n))<<size_x_log2))<<2;

                    for( unsigned int k= 0; k!= 4; k++ )
                        pixel[k]+= data[d+k];
                    
                }//m
            }//n

            d= (i + (j<<size_x_log2))<<2;

            for( unsigned int k= 0; k< 4; k++ )
                new_d[d+k]= (pixel[k]>>scale_log2)>>scale_log2;

        }//j

        delete[] data;
        data= new_d;

}*/

void Texture::DownScaleX()
{
    unsigned char* new_d= new unsigned char[ size_x * size_x * 4 ];

    unsigned int d1, d2;
    unsigned int mask_x= size_x - 1;

    for( unsigned int i= 0; i< size_x; i++ )
        for( unsigned int j= 0; j< size_y; j++ )
        {
            d1= (i + (j<<size_x_log2) )<<2;
            d2= (((i<<1)&mask_x) + (j<<size_x_log2) )<<2;
            for( unsigned int k= 0; k< 4; k++ )
                new_d[d1+k]= (data[d2+k] + data[d2+k+4])>>1;
            
        }

        delete[] data;
        data= new_d;
}


void Texture::DownScaleY()
{
    unsigned char* new_d= new unsigned char[ size_x * size_x * 4 ];

    unsigned int d1, d2;
    unsigned int mask_y= size_y - 1;

    for( unsigned int i= 0; i< size_x; i++ )
        for( unsigned int j= 0; j< size_y; j++ )
        {
            d1= (i + (j<<size_x_log2) )<<2;
            d2= (i + (((j<<1)&mask_y)<<size_x_log2) )<<2;
            for( unsigned int k= 0; k< 4; k++ )
                new_d[d1+k]= ( data[d2+k] + data[d2+k+(size_x<<2)] )>>1;
            
        }

        delete[] data;
        data= new_d;
}
