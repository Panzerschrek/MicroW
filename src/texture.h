#ifndef TEXTURE_H
#define TEXTURE_H
#include "mw.h"

//texture generator

//4 bypte perpixel texture, with size power_of_two
class Texture
{
public:
    Texture(){one_channel= false;}
    Texture( unsigned int log2_x, unsigned int log2_y );
    ~Texture(){delete[] data;}

    unsigned int SizeX();
    unsigned int SizeY();   
    unsigned int SizeXLog2();
    unsigned int SizeYLog2();
    const unsigned char* Data()const;

    void DrawTriangle( const short* vertices, const unsigned char* color  );//{ x1, y1, x2,y2, x3,y3 }
    void DrawLine( short x0, short y0, short x1, short y1, const unsigned char* color);
    void DrawCircle( short x, short y, short r, const unsigned char* color );
    void FillRect( short x0, short y0, short x1,short y1, const unsigned char* color );
    void Clear( const unsigned char* clear_color= default_clear_color );
    void Noise();
    void Smooth();
    void SetAlpha( unsigned char a );
    void DrawText( short x, short y, unsigned int size, const unsigned char* color, const char* text );

    void Copy( Texture* t );
    void Copy( const Texture* t, unsigned short width, unsigned short height, unsigned short src_x, unsigned short src_y, unsigned short dst_x, unsigned short dst_y );
    void AlphaBlend( const Texture* t );
    void Add( const Texture* t );
    void Sub( const Texture* t );
    void Mul( const Texture* t );

    void Add( unsigned char c );
    void Sub( unsigned char c );
    void Mul( unsigned char c );

    void Add( const unsigned char* c );
    void Sub( const unsigned char* c );
    void Mul( const unsigned char* c );

    /*void DownScale( unsigned int scale_log2 );
    void DownScale( unsigned int scale_x_log2, unsigned int scale_y_log2 );
    void UpScale( unsigned int scale_log2 );
    void UpScale( unsigned int scale_x_log2, unsigned int scale_y_log2 );*/
    void UpScaleX();
    void UpScaleY();
    void DownScaleX();
    void DownScaleY();

    void ToOneChannel( unsigned char channel );// convert texture from rgb to grayscale ( save one channel )
   

    void Move2GPU();
    void Bind( unsigned int unit= 0 );
private:
    unsigned char* data;
    unsigned int size_x, size_y, size_x_log2, size_y_log2;
    static unsigned char default_clear_color[4];
    GLuint tex_id;
    bool one_channel;


    unsigned char Noise2( int x,  int y, unsigned int mask );
    unsigned char FinalNoise(unsigned intx, unsigned int y);
    unsigned char InterpolatedNoise( unsigned int x, unsigned int y, unsigned int k );

public:
   static Texture* default_1024_noise;
};

inline unsigned int Texture::SizeX()
{
    return size_x;
}
inline unsigned int Texture::SizeY()
{
    return size_y;
}

inline unsigned int Texture::SizeXLog2()
{
    return size_x_log2;
}
inline unsigned int Texture::SizeYLog2()
{
    return size_y_log2;
}
inline const unsigned char* Texture::Data()const
{
    return (const unsigned char*) data;
}

#endif//TEXTURE_H