#ifndef TEXT_H
#define TEXT_H

#include "mw.h"
#include "vertex_buffer.h"
#include "glsl_program.h"
#include "shaders.h"

class Texture;

#pragma pack( push, 1 )
struct TextVertex
{
	float pos[2];
	unsigned short tex_coord[2];
	unsigned char color[4];
};
#pragma pack (pop)


#define MRPG_MAX_TEXT_BUFFER_SIZE 8192

#define LETTER_WIDTH 8
#define LETTER_HEIGHT 18
#define FONT_BITMAP_WIDTH 768
#define FONT_BITMAP_HEIGHT 18

class Text
{
	public:

	Text();
	~Text(){}
	void AddText( unsigned int colomn, unsigned int row, unsigned int size, const unsigned char* color, const char* text );
    
    void DrawCross();

	void Draw();
    void SetViewport( unsigned int x, unsigned int y );

	private:

	void CreateTexture();

	GLSLProgram text_shader;
	TextVertex* vertices;
    VertexBuffer text_vbo;
	unsigned int vertex_buffer_size;
	unsigned int vertex_buffer_pos;
	float screen_x, screen_y;

    unsigned char* text_texture_data;

	GLuint font_texture_id;
    bool draw_crosshair;

public:
    static const unsigned char default_color[4];

    friend class Texture;

private:
    static Text* default_text;
public:
    static Text* DefaultText();

};

inline Text* Text::DefaultText()
{
    return default_text;
}

inline void Text::SetViewport( unsigned int x, unsigned int y )
{
    screen_x= float(x);
    screen_y= float(y);
}

#endif//TEXT_H
