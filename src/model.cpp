#include "model.h"
#include "math.h"

Model::Model()
{
    index_count= vertex_count= 0;
    indeces= NULL;
    vertices= NULL;
    for( int i= 0; i< MAX_MODEL_ANIMATIONS; i++ )
        is_animation[i]= false;
}
Model::~Model()
{
    if( indeces != NULL )
        delete[] indeces;
    if( vertices != NULL )
        delete[] vertices;
}

void Model::Draw()
{
    if( index_count == 0 )//non indexed geometry
        glDrawArrays( primitive_type, 0, vertex_count );
    else
        glDrawElements( primitive_type, index_count, GL_UNSIGNED_SHORT, NULL );
}


void Model::Move( const short* vec )
{
    for( unsigned int i= 0; i< vertex_count; i++ )
    {
        vertices[i].pos[0]+= vec[0];
        vertices[i].pos[1]+= vec[1];
        vertices[i].pos[2]+= vec[2];
    }
}


void Model::Transform( const float* transform_matrix )
{
    float v[3];
    for( unsigned int i= 0; i< vertex_count; i++ )
    {
        v[0]= float(vertices[i].pos[0])*0.00390625f;// 1/256
        v[1]= float(vertices[i].pos[1])*0.00390625f;
        v[2]= float(vertices[i].pos[2])*0.00390625f;

        Vec3Mat4Mul( v, transform_matrix );

        vertices[i].pos[0]= short(v[0]*256.0f);
        vertices[i].pos[1]= short(v[1]*256.0f);
        vertices[i].pos[2]= short(v[2]*256.0f);
    }
}

void Model::TransformNormals( const float* transform_matrix )
{
    float v[3];
    for( unsigned int i= 0; i< vertex_count; i++ )
    {
        v[0]= float(vertices[i].normal[0]*0.0078125f);//1/128
        v[1]= float(vertices[i].normal[1]*0.0078125f);
        v[2]= float(vertices[i].normal[2]*0.0078125f);

        Vec3Mat4Mul( v, transform_matrix );

        vertices[i].normal[0]= char(v[0]*127.0f);
        vertices[i].normal[1]= char(v[1]*127.0f);
        vertices[i].normal[2]= char(v[2]*127.0f);
    }
}

void Model::Scale( short s )
{
    short sc[]={ s, s, s };
    Scale( sc );
}

void Model::ScaleTexture( const short* s )
{
    for( unsigned int i= 0; i< vertex_count; i++ )
    {
        vertices[i].tex_coord[0]= ( vertices[i].tex_coord[0] * s[0] )>>12;
        vertices[i].tex_coord[1]= ( vertices[i].tex_coord[1] * s[1] )>>12;
    }
}

void Model::ShiftTexture( const short* s )
{
     for( unsigned int i= 0; i< vertex_count; i++ )
    {
        vertices[i].tex_coord[0]+= s[0];
        vertices[i].tex_coord[1]+= s[1];
    }
}

void Model::Scale( const short* s )
{
    for( unsigned int i= 0; i< vertex_count; i++ )
    {
        for( unsigned int j= 0; j< 3; j++ )
            vertices[i].pos[j]= ( vertices[i].pos[j] * s[j] )>>8;
    }
}

void Model::SetTexture( short t )
{
    for( unsigned int i= 0; i< vertex_count; i++ )
        vertices[i].tex_coord[2]= t;
}

void Model::SetBone( unsigned char bone_id )
{
    for( unsigned int i= 0; i< vertex_count; i++ )
        vertices[i].bone_id= bone_id;
}

void Model::AddModel( const Model* m )
{
    unsigned short* ind;
    ModelVertex* v;
    unsigned int v_c, i_c;

    v_c= this->vertex_count + m->vertex_count;
    i_c=this->index_count + m->index_count;

    ind= new unsigned short[ i_c ];
    v= new ModelVertex[ v_c ];

    unsigned int i;

    for( i= 0; i< this->vertex_count; i++ )
        v[i]= this->vertices[i];


    unsigned short v_shift= this->vertex_count;
    for( i= 0; i< m->vertex_count; i++ )
        v[i+v_shift]= m->vertices[i];

    for( i= 0; i< this->index_count; i++ )
        ind[i]= this->indeces[i];

    unsigned int i_shift= this->index_count;
    for( i= 0; i< m->index_count; i++ )
        ind[i+i_shift]= m->indeces[i] + v_shift;

    delete[] this->vertices;
    delete[] this->indeces;

    this->vertices= v;
    this->indeces= ind;
    this->vertex_count= v_c;
    this->index_count= i_c;
}