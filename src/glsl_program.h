#ifndef GLSL_PROGRAM_H
#define GLSL_PROGRAM_H
#include "mw.h"
#include "gl_functions.h"

class GLSLProgram
{
	public:

	GLSLProgram();
	~GLSLProgram(){}


	void Bind();

	void UniformInt	( unsigned int uniform_id, int i );
	void UniformMat4( unsigned int uniform_id, const float* mat );
    void UniformMat3( unsigned int uniform_id, const float* mat );
    void UniformMat4Array( unsigned int uniform_id, unsigned int count, const float* mat );
    void UniformMat3Array( unsigned int uniform_id, unsigned int count, const float* mat );
	void UniformVec3( unsigned int uniform_id, const float* v );
	void UniformFloat(unsigned int uniform_id, float f );

	void SetAttribLocation( const char* attrib_name, unsigned int attrib );
	void FindUniform( const char* uniform );


	void Create( const char* vertex_shader, const char* fragment_shader );

	private:

	GLuint v_shader, f_shader, program_id;

	GLuint attribs[ 8 ];
	char attrib_names[ 8 ][ 16 ];
	unsigned int attrib_count;

	GLint uniforms[ 24 ];
	char uniform_names[ 24 ][ 16 ];
	unsigned int uniform_count;
};


inline void GLSLProgram::Bind()
{
	glUseProgram( program_id );
}

#endif//GLSL_PROGRAM_H
