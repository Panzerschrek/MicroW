#ifndef MODEL_H
#define MODEL_H
#include "mw.h"


#define MAX_MODEL_BONES 8
#define MAX_MODEL_ANIMATIONS 4

#define ANIMATION_IDLE      -1
#define ANIMATION_ATTACK0   0
#define ANIMATION_ATTACK1   1
#define ANIMATION_WALK0     2
#define ANIMATION_WALK1     3
#define ANIMATION_KIND      4

#pragma pack(push,1)
struct ModelVertex
{
    short pos[3];// fixed point 8.8
    short tex_coord[3];//fixed point 4.12
    char normal[3];
    unsigned char bone_id;
};//16b struct
#pragma pack(pop)

struct ModelAnimation
{
    float duration;
    bool bones[8];// true if bone include in this animation
    unsigned int bone_count;
    unsigned int frames;
    float* n_matrices;//normal matrices
    float* v_matrices;//matrices for active bones n- bone_count*frames
};

class Model
{
public:

    Model();
    ~Model();
    ModelVertex* vertices;
    unsigned short* indeces;
    unsigned int vertex_count, index_count;
    GLenum primitive_type;//triangles/lines/strips/points/etc

    ModelAnimation animations[MAX_MODEL_ANIMATIONS];
    bool is_animation[MAX_MODEL_ANIMATIONS];
    unsigned int animation_count;

    void Draw();

    void Transform( const float* transform_matrix );
    void TransformNormals( const float* transform_matrix );
    void Move( const short* vec );
    void Scale( short s );
    void Scale( const short* s );
    void ScaleTexture( const short* s );
    void ShiftTexture( const short* s );
    void SetTexture( short t );//set texture coord[2]
    void SetBone( unsigned char bone_id );

    void AddModel( const Model* m );
};


#endif//MODEL_H