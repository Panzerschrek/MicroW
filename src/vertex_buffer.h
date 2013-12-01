#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "mw.h"

#include "gl_functions.h"

class VertexBuffer
{
	public:

	VertexBuffer();
	~VertexBuffer(){}
    static void EnableAttribs( int count );

    void GenVAO();
	void VertexData( const void* data, unsigned int data_size, unsigned int vertex_size );
	void IndexData( const void* data, unsigned int data_size );
	void VertexAttrib( int attrib, unsigned int components, GLenum type, bool normalized, unsigned int shift );
    void VertexAttribInt( int attrib, unsigned int components, GLenum type, unsigned int shift );

	void VertexSubData( const void* data, unsigned int data_size, unsigned int shift );
	void IndexSubData( const void* data, unsigned int data_size, unsigned int shift );

	void Bind();

	private:

	unsigned int vertex_size;
	GLuint index_vbo, vertex_vbo, vao;

    static bool vertex_attribs_enabled[8];
};


inline void VertexBuffer::GenVAO()
{
    glGenVertexArrays( 1, &vao);
    glBindVertexArray(vao);
}
inline void VertexBuffer::Bind()
{
	//glBindBuffer( GL_ARRAY_BUFFER, vertex_vbo );
	//if( index_vbo != 0xffffffff )
	//	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, index_vbo );
    glBindVertexArray(vao);
}

#endif//VERTEX_BUFFER_H
