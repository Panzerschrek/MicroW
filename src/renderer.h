#ifndef RENDERER_H
#define RENDERER_H


#include "player.h"
#include "level.h"
#include "mw.h"
#include "text.h"
#include "vertex_buffer.h"
#include "glsl_program.h"
#include "texture.h"
#include "model.h"

#define QUADS_PER_CHUNK 64
#define CHUNK_LOD_COUNT 5

#pragma pack( push, 1 )
struct LandscapeVertex
{
	unsigned short pos[2];
};
#pragma pack(pop)



class Renderer
{
	public:
	Renderer( Level* l, Player* p );
	~Renderer(){}

	void SetSceneScale( float s );
	void ZoomIn();
	void ZoomOut();

    void Init();
	void Draw();
    void DrawGUI();
    void DrawFirstFrame();
    void SetViewport( unsigned int x, unsigned int y );


	private:


    void DrawStaticModels();
    void DrawLandscape();
    void DrawWeapon();
    void DrawMonsters();
    void GenLandscapeMesh();

    void InitTextures();
    void InitShaders();
    void InitVertexBuffers();

    bool IsObjectBehindPlayer( const float* obj_pos, float obj_radius );

   // VertexBuffer microbes_vbo;
   VertexBuffer level_vbo;

    GLSLProgram level_shader;
	Player* player;
	Level* level;


    //VertexBuffer player_weapon_vbo;
    VertexBuffer monster_vbo;

    GLSLProgram item_glow_shader;
    VertexBuffer item_glow_vbo;
    float* item_glow_vertices;
    unsigned int item_sprite_count;
    void DrawItemGlow();
    Texture item_glow_texture;

    GLSLProgram landscape_shader;
	GLuint landscape_heightmap_texture;
	VertexBuffer landscape_vbo;
	LandscapeVertex* landscape_vertices;
	unsigned int landscape_x, landscape_y;
	unsigned short* landscape_indeces;//4 meshes - 4 LODS
    unsigned int lod_shift[CHUNK_LOD_COUNT];
    Texture land_texture;
    GLuint land_texture_array;
    GLuint land_texture_map;


   // Model animated_models[4];
    GLSLProgram animated_models_shader;
    //VertexBuffer animated_models_vbo;
  //  void DrawAnimatedModels();
    void DrawAnimatedModel( const Model* m, const bool* active_animations,
    const float* animation_time, const float* v_matrix, const float* n_matrix, unsigned int v_shift, unsigned int i_shift   );

    void DrawSky();
    GLSLProgram sky_shader;

	Text text;

	unsigned int screen_x, screen_y;
	float scene_scale;

    Model monsters[MONSTER_COUNT];
    //Model weapons[4];
    unsigned int monsters_vertex_shift[MONSTER_COUNT];
    unsigned int monsters_index_shift[MONSTER_COUNT];
    //unsigned int weapons_vertex_shift[4];
    //unsigned int weapons_index_shift[4];
	Model static_objects[ STATIC_OBJECTS ];
    unsigned int model_vertex_shift[ STATIC_OBJECTS ];
    unsigned int model_index_shift[ STATIC_OBJECTS ];

    float view_matrix[16];
    float inv_view_matrix[16];
    float inv_view_normal_matrix[16];
    float sun_vector[3];

    void CalculateViewMatrix();
    void CalculateFPS();


};

inline void Renderer::SetSceneScale( float s )
{
	scene_scale= s;
}

inline void Renderer::ZoomIn()
{
	if( scene_scale < 1.0f )
		scene_scale*= 2.0f;
}

inline void Renderer::ZoomOut()
{
	if( scene_scale> 0.03125f )
		scene_scale*= 0.5f;
}


inline void Renderer::SetViewport( unsigned int x, unsigned int y )
{
    screen_x= x;
    screen_y= y;
    printf( "viewport: %d %d\n", x, y );
    text.SetViewport(x,y);
    glViewport( 0, 0, x, y );
}

#endif//RENDERER_H
