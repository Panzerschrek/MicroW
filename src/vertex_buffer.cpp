#include "vertex_buffer.h"


bool VertexBuffer::vertex_attribs_enabled[8]= {false,false,false,false,false,false,false,false};

void VertexBuffer::EnableAttribs( int count )
{
    int i;
for( i= 0; i< count; i++ )
         glEnableVertexAttribArray(i);
    for( i= count; i<8; i++ )
         glDisableVertexAttribArray(i);


}

void VertexBuffer::VertexAttrib( int attrib, unsigned int components, GLenum type, bool normalized, unsigned int shift )
{
    glVertexAttribPointer( attrib, components, type, normalized, vertex_size, (void*) shift );
    glEnableVertexAttribArray(attrib);
}

void VertexBuffer::VertexAttribInt( int attrib, unsigned int components, GLenum type, unsigned int shift )
{
    glVertexAttribIPointer( attrib, components, type, vertex_size, (void*) shift );
    glEnableVertexAttribArray( attrib );
}

VertexBuffer::VertexBuffer()
{
	index_vbo= vertex_vbo= 0xffffffff;
}


void VertexBuffer::VertexData( const void* data, unsigned int data_size, unsigned int vertex_size )
{
	if( vertex_vbo == 0xffffffff )
		glGenBuffers( 1, &vertex_vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vertex_vbo );
	glBufferData( GL_ARRAY_BUFFER, data_size, data, GL_STATIC_DRAW );

    this->vertex_size= vertex_size;
}


void VertexBuffer::IndexData( const void* data, unsigned int data_size )
{
	if( index_vbo == 0xffffffff )
		glGenBuffers( 1, &index_vbo );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, index_vbo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, data_size, data, GL_STATIC_DRAW );
}


void VertexBuffer::VertexSubData( const void* data, unsigned int data_size, unsigned int shift )
{
    glBindBuffer( GL_ARRAY_BUFFER, vertex_vbo );
    glBufferSubData( GL_ARRAY_BUFFER, shift, data_size, data );
}

void VertexBuffer::IndexSubData( const void* data, unsigned int data_size, unsigned int shift )
{
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, index_vbo );
    glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, shift, data_size, data );
}
