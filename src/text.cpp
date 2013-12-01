#include "../other/font_data.c"
#include "text.h"



const unsigned char Text::default_color[4]= {255, 255, 255, 32 };
Text* Text::default_text= NULL;



void Text::CreateTexture()
{
	unsigned char* decompressed_font= new unsigned char[ FONT_BITMAP_WIDTH * FONT_BITMAP_HEIGHT ];

    static unsigned char bits[]= { 128, 64, 32, 16, 8, 4, 2, 1 };
	for( unsigned int i= 0; i< FONT_BITMAP_WIDTH * FONT_BITMAP_HEIGHT / 8; i++ )
		for( unsigned int j= 0; j< 8; j++ )
					decompressed_font[ i*8 + j ]= ( ( font_data[i] & bits[j] ) == 0 ) ? 0 : 255;



	glGenTextures( 1, &font_texture_id );

	glBindTexture( GL_TEXTURE_2D, font_texture_id );

	//glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RED,
					 FONT_BITMAP_WIDTH, FONT_BITMAP_HEIGHT, 0,
					 	 GL_RED,  GL_UNSIGNED_BYTE, decompressed_font );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, /*GL_NEAREST_MIPMAP_LINEAR*/GL_NEAREST );

	text_texture_data= decompressed_font;
}


void Text::DrawCross()
{
    draw_crosshair= true;
    static const char t[]= { 127, 0 };
    static const unsigned char color[]= { 255, 255, 255, 0 };
    AddText( 0, 0, 2, color, t );
    draw_crosshair= false;

}

void Text::AddText( unsigned int colomn, unsigned int row, unsigned int size, const unsigned char* color, const char* text )
{
	const char* str= text;

	float x, x0, y;
	float dx, dy;

    if( draw_crosshair )//HACK for crosshair
    {
        x= -float( 2*LETTER_WIDTH ) / screen_x;
        y= -float( 2*LETTER_HEIGHT ) / screen_y;
    }
    else
    {
	    x0= x=  2.0f * float( colomn * LETTER_WIDTH ) / screen_x - 1.0f;
	    y=  -2.0f * float( (row + 1) * LETTER_HEIGHT ) / screen_y + 1.0f;
    }

	dx= 2.0f * float( LETTER_WIDTH * size ) / screen_x;
	dy= 2.0f * float( LETTER_HEIGHT * size ) / screen_y;


	TextVertex* v= vertices + vertex_buffer_pos;
	while( *str != 0 )
	{

        if( *str == '\n' )
        {
            x= x0;
            y-=dy;
            str++;
            continue;
        }
		v[0].pos[0]= x;
		v[0].pos[1]= y;
		v[0].tex_coord[0]= *str - 32;
		v[0].tex_coord[1]= 0;

		v[1].pos[0]= x;
		v[1].pos[1]= y + dy;
		v[1].tex_coord[0]= *str - 32;
		v[1].tex_coord[1]= 1;

		v[2].pos[0]= x + dx;
		v[2].pos[1]= y + dy;
		v[2].tex_coord[0]= *str - 32 + 1;
		v[2].tex_coord[1]= 1;

		v[3].pos[0]= x + dx;
		v[3].pos[1]= y;
		v[3].tex_coord[0]= *str - 32 + 1;
		v[3].tex_coord[1]= 0;

        for( unsigned int i= 0; i< 4; i++ )
             *((int*)v[i].color)= *((int*)color);//copy 4 bytes per one asm command

		x+= dx;
		v+= 4;
		str++;
	}
	vertex_buffer_pos= v - vertices;
}

void Text::Draw()
{
    glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, font_texture_id );

    text_vbo.Bind();
    text_vbo.VertexSubData( vertices, vertex_buffer_pos * sizeof(TextVertex), 0 );
    

    text_shader.Bind();
    text_shader.UniformInt( 0, 0 );//texture - unit 0

	glEnable( GL_BLEND );
    glDrawElements( GL_TRIANGLES, vertex_buffer_pos * 6 / 4, GL_UNSIGNED_SHORT, NULL );
	glDisable( GL_BLEND );
    vertex_buffer_pos= 0;
}


Text::Text():
vertex_buffer_pos(0),
draw_crosshair(false)
{
    default_text= this;
	CreateTexture();

	vertices= new  TextVertex[ MRPG_MAX_TEXT_BUFFER_SIZE * 4 ];

	int v[4];
	glGetIntegerv( GL_VIEWPORT, v );
	screen_x= float( v[2] );
	screen_y= float( v[3] );

    unsigned short* quad_indeces= new unsigned short[MRPG_MAX_TEXT_BUFFER_SIZE*6];
    for( unsigned int i= 0, j= 0; i< MRPG_MAX_TEXT_BUFFER_SIZE*6; i+= 6, j+=4 )
    {
        quad_indeces[i]= j;
        quad_indeces[i + 1]= j + 1;
        quad_indeces[i + 2]= j + 2;

        quad_indeces[i + 3]= j + 2;
        quad_indeces[i + 4]= j + 3;
        quad_indeces[i + 5]= j;
    }

    text_vbo.GenVAO();
    text_vbo.VertexData( NULL, MRPG_MAX_TEXT_BUFFER_SIZE * 4 * sizeof(TextVertex),
                                    sizeof(TextVertex) );
    text_vbo.IndexData( quad_indeces, MRPG_MAX_TEXT_BUFFER_SIZE * 6 * sizeof(short) ); delete[] quad_indeces;
    
    text_vbo.VertexAttrib( 0, 2, GL_FLOAT, false, 0 );//vertex coordinates
    text_vbo.VertexAttrib( 1, 2, GL_UNSIGNED_SHORT, false, sizeof(float)*2 );//texture coordinates
    text_vbo.VertexAttrib( 2, 4, GL_UNSIGNED_BYTE, true, sizeof(float)*2 + 2*sizeof(short) );//color


    text_shader.SetAttribLocation( "v", 0 );
    text_shader.SetAttribLocation( "tc", 1 );
    text_shader.SetAttribLocation( "c", 2 );
	text_shader.Create( text_shader_v, text_shader_f );
    text_shader.FindUniform( "tex" );
}
