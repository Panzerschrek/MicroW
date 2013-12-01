#include <time.h>

#include "renderer.h"
#include "shaders.h"
#include "math.h"
#include "landscape.h"
#include "texture_generation.h"
#include "model.h"
#include "model_generation.h"
#include "particle_manager.h"
#include "main_loop.h"

void Renderer::InitTextures()
{

    glGenerateMipmap( GL_TEXTURE_2D );


    //texture array for all textures
    glGenTextures(1, &land_texture_array );
    glBindTexture( GL_TEXTURE_2D_ARRAY, land_texture_array );
    glTexImage3D( GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8,
                1024, 1024, NearestPOT<TEXTURE_COUNT>(),//size
                0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );

     //GENERATION OF TEXTURES
    for( unsigned int i= 0; i< TEXTURE_COUNT; i++ )
    {
        texture_gen_func[i]( &land_texture );
        glTexSubImage3D( GL_TEXTURE_2D_ARRAY, 0, 
                        0,0,i,//offset
                        1024, 1024, 1,//size
                        GL_RGBA, GL_UNSIGNED_BYTE, land_texture.Data() );
    }
    
    //glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); // it is initial value
    glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glGenerateMipmap( GL_TEXTURE_2D_ARRAY );


    Landscape* land= (Landscape*) level->GetLandscape();

    //texture map for landscape
    glGenTextures( 1, &land_texture_map );
    glBindTexture( GL_TEXTURE_2D, land_texture_map );    
    glTexImage2D( GL_TEXTURE_2D, 0, GL_R8,
                  land->SizeX(), land->SizeY(), 0,
                  GL_RED,  GL_UNSIGNED_BYTE, land->Texturemap() );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );


    //shadow map of landscape
    glGenTextures( 1, &land_shadow_map );
    glBindTexture( GL_TEXTURE_2D, land_shadow_map );    
    glTexImage2D( GL_TEXTURE_2D, 0, GL_R8,
                  land->SizeX(), land->SizeY(), 0,
                  GL_RED,  GL_UNSIGNED_BYTE, land->ShadowMap() );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    //landscape heightmap
    glGenTextures( 1, &landscape_heightmap_texture );
    glBindTexture( GL_TEXTURE_2D, landscape_heightmap_texture );    
    glTexImage2D( GL_TEXTURE_2D, 0, GL_R16F,
                  land->SizeX(), land->SizeY(), 0,
                  GL_RED,  GL_UNSIGNED_SHORT, land->Heightmap() );

   // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); // it is initial value
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glGenerateMipmap( GL_TEXTURE_2D );


    //new(item_glow_texture) Texture(128,128);
    GenItemGlow( &item_glow_texture );
    item_glow_texture.Move2GPU();



   /* Texture t( 11, 11 );
    GenClouds(&t);
    t.Move2GPU();*/
    Texture pt(7,7);
    glGenTextures( 1, &particles_texture_array );
    glBindTexture( GL_TEXTURE_2D_ARRAY, particles_texture_array );
     glTexImage3D( GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8,
                128, 128, 8,//size
                0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
     for( unsigned int i= 0; i< PARTICLE_TYPE_COUNT; i++ )
     {
         particle_texture_gen_func[i]( &pt );
          glTexSubImage3D( GL_TEXTURE_2D_ARRAY, 0, 
                        0,0,i,//offset
                        128, 128, 1,//size
                        GL_RGBA, GL_UNSIGNED_BYTE, pt.Data() );
     }
     glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
      glGenerateMipmap( GL_TEXTURE_2D_ARRAY );


     water_texture= new Texture(9,9);
     GenWaterWaves(water_texture);
     water_texture->Move2GPU();

     blood_texture= new Texture(10,10);
     GenBloodScreenTexture( blood_texture );
     blood_texture->Move2GPU();
   

     cloud_texture= new Texture(11,11);
     GenClouds( cloud_texture );
     cloud_texture->ToOneChannel(3);
     cloud_texture->Move2GPU();
}

void Renderer::InitShaders()
{
    level_shader.SetAttribLocation( "v", 0 );//position
    level_shader.SetAttribLocation( "tc", 1 );//texture coord
    level_shader.SetAttribLocation( "n", 2 );//normal
    level_shader.Create( model_shader_v, model_shader_f );
    level_shader.FindUniform( "m" );//transform matrix, uniform No 0*/
    level_shader.FindUniform( "tex" );//uniform 1 - texture
    level_shader.FindUniform( "sun" );//uniform 2 - sun vector


    landscape_shader.SetAttribLocation("p", 0 );
    landscape_shader.Create( landscape_shader_v, landscape_shader_f );
    landscape_shader.FindUniform( "hm" );//uniform 0 - heightmap
    landscape_shader.FindUniform( "ps" );//uniform 1 - position shift
    landscape_shader.FindUniform( "m" );//uniform 2 - transform matrix
    landscape_shader.FindUniform( "tex" );//uniform 3 - texture
    landscape_shader.FindUniform( "a" );//uniform 4 - landscape amplitude
    landscape_shader.FindUniform( "tm" );//uniform 5 - texture map
    landscape_shader.FindUniform( "sun" );//uniform 6 - sun vector
    landscape_shader.FindUniform( "sm" );//uniform 7 - shadow map

    item_glow_shader.SetAttribLocation( "v", 0 );
    item_glow_shader.Create( item_glow_shader_v, item_glow_shader_f );
    item_glow_shader.FindUniform( "m" );//transform matirx
    item_glow_shader.FindUniform( "tex" );

    animated_models_shader.SetAttribLocation( "v", 0 );//position
    animated_models_shader.SetAttribLocation( "tc", 1 );//texture coord
    animated_models_shader.SetAttribLocation( "n", 2 );//normal
    animated_models_shader.SetAttribLocation( "b", 3 );//bone id
    animated_models_shader.Create( model_animation_shader_v, model_shader_f );
    animated_models_shader.FindUniform( "nm" );//normal matrices 0
    animated_models_shader.FindUniform( "vm" );//vertex matirces 1
    animated_models_shader.FindUniform( "tex" );//uniform 2 - texture
    animated_models_shader.FindUniform( "sun" );//uniform 3 - sun vector

    sky_shader.SetAttribLocation( "v", 0 );
    sky_shader.Create( sky_shader_v, sky_shader_f );
    sky_shader.FindUniform( "m" );
    sky_shader.FindUniform( "sun" );
    sky_shader.FindUniform( "tex" );
    sky_shader.FindUniform( "cs" );//cloud shift - 3

    particle_shader.SetAttribLocation( "v", 0 );
    particle_shader.Create( particle_shader_v, particle_shader_f );
    particle_shader.FindUniform( "m" );//transform matirx
    particle_shader.FindUniform( "tex" );


    water_shader.SetAttribLocation( "v", 0 );
    water_shader.Create( water_shader_v, water_shader_f );
    water_shader.FindUniform( "m" );//transform matirx
    water_shader.FindUniform( "h" );//water level
    water_shader.FindUniform( "cam" );//camera position
    water_shader.FindUniform( "sun" );
    water_shader.FindUniform( "tex" );
    water_shader.FindUniform( "t" );//time - #5


    weapon_shader.SetAttribLocation( "v", 0 );//position
    weapon_shader.SetAttribLocation( "tc", 1 );//texture coord
    weapon_shader.SetAttribLocation( "n", 2 );//normal
    weapon_shader.SetAttribLocation( "b", 3 );//bone id
    weapon_shader.Create( weapon_shader_v, weapon_shader_f );
    weapon_shader.FindUniform( "nm" );//normal matrices 0
    weapon_shader.FindUniform( "vm" );//vertex matirces 1
    weapon_shader.FindUniform( "tex" );//uniform 2 - texture
    weapon_shader.FindUniform( "ivm" );//uniform 3 - inverce view matrix
    weapon_shader.FindUniform( "pm" );//uniform 4 - projection matirx
    weapon_shader.FindUniform( "sun" );//uniform 5 - sun vector
    weapon_shader.FindUniform( "cam" );//6 - camera position

    blood_shader.Create( blood_shader_v, blood_shader_f );
    blood_shader.FindUniform( "bf" );//blood factor - 0
     blood_shader.FindUniform( "tex" );//1
}


void Renderer::InitVertexBuffers()
{
    GenCacodemon( &monsters[0] );
    GenRobot( &monsters[1] );
    GenSnowMan( &monsters[2] );
    unsigned int v_count= 0, i_count= 0; 
     for( unsigned int i= 0; i< MONSTER_COUNT; i++ )
    {
        v_count+= monsters[i].vertex_count;
        i_count+= monsters[i].index_count;
    }

    monster_vbo.GenVAO();
    monster_vbo.VertexData( NULL, v_count * sizeof(ModelVertex), sizeof(ModelVertex) );
    monster_vbo.IndexData( NULL,i_count * sizeof(short) );
     for( unsigned int i= 0, vs= 0, is= 0; i< MONSTER_COUNT; i++ )
    {
        monsters_vertex_shift[i]= vs;
        monsters_index_shift[i]= is;
        monster_vbo.VertexSubData( monsters[i].vertices,
            monsters[i].vertex_count * sizeof(ModelVertex), vs * sizeof(ModelVertex) );
        monster_vbo.IndexSubData( monsters[i].indeces,
            monsters[i].index_count * sizeof(short), is * sizeof(short) );

        vs+= monsters[i].vertex_count;
        is+= monsters[i].index_count;
     }
    monster_vbo.VertexAttrib( 0, 3, GL_SHORT, false, 0 );//position
    monster_vbo.VertexAttrib( 1, 3, GL_SHORT, false, 6 );//texture coord
    monster_vbo.VertexAttrib( 2, 3, GL_BYTE, true, 6 + 6 );//normal
    monster_vbo.VertexAttribInt( 3, 1, GL_UNSIGNED_BYTE, 6 + 6 + 3 );//bone id




    v_count= 0, i_count= 0; 
    for( unsigned int i= 0; i< STATIC_OBJECTS + BULLET_TYPE_COUNT + 1; i++ )
    {
        static_objects_generation_func[i]( static_objects + i );
        v_count+= static_objects[i].vertex_count;
        i_count+= static_objects[i].index_count;
    }
    level_vbo.GenVAO();
    level_vbo.VertexData( NULL, sizeof(ModelVertex) * v_count, sizeof(ModelVertex) );
    level_vbo.IndexData( NULL, i_count * sizeof(short) );
    level_vbo.VertexAttrib( 0, 3, GL_SHORT, false, 0 );//position
    level_vbo.VertexAttrib( 1, 3, GL_SHORT, false, 6 );//texture coord
    level_vbo.VertexAttrib( 2, 3, GL_BYTE, true, 6 + 6 );//normal
    level_vbo.VertexAttribInt( 3, 1, GL_UNSIGNED_BYTE, 6 + 6 + 3 );//bone id
    for( unsigned int i= 0, vs=0, is= 0; i< STATIC_OBJECTS + BULLET_TYPE_COUNT + 1; i++ )
    {
       model_vertex_shift[i]= vs;
       model_index_shift[i]= is;

       level_vbo.VertexSubData( static_objects[i].vertices, 
       static_objects[i].vertex_count * sizeof(ModelVertex), vs * sizeof(ModelVertex) );
       level_vbo.IndexSubData( static_objects[i].indeces,
       static_objects[i].index_count *sizeof(short), is * sizeof(short) );

       vs+= static_objects[i].vertex_count;
       is+= static_objects[i].index_count;
    }
    

    GenLandscapeMesh();
    landscape_vbo.GenVAO();
    landscape_vbo.VertexData( landscape_vertices,
                              sizeof(LandscapeVertex) * (QUADS_PER_CHUNK + 1 )* (QUADS_PER_CHUNK + 1 ) * 2,
                              sizeof(LandscapeVertex) );
    landscape_vbo.IndexData( landscape_indeces, 2 * QUADS_PER_CHUNK * QUADS_PER_CHUNK * 6 * sizeof(short) );
    landscape_vbo.VertexAttrib( 0, 2, GL_UNSIGNED_SHORT, false, 0 );//attrib 0 - position


    item_glow_vbo.GenVAO();
    item_glow_vbo.VertexData( NULL, 2048 * sizeof(float)*4, sizeof(float)*4 );
    item_glow_vbo.VertexAttrib( 0, 4, GL_FLOAT, false, 0 );// 1 attrib - position
    item_glow_vertices= new float[ 2048 * 4 ];

    particles_vbo.GenVAO();
    particles_vbo.VertexData( NULL, MAX_PARTICLES * 4 * sizeof(float), 4*sizeof(float) );
    particles_vbo.VertexAttrib( 0, 4, GL_FLOAT, false, 0 );// 1 attrib - position
}


void Renderer::Init()
{

    sun_vector[0]= 0.7f;
    sun_vector[1]= 0.5f;
    sun_vector[2]= 0.5f;

    DrawLoading(0);
    ( (Landscape*) level->GetLandscape() )->GenShadowMap( sun_vector );

    DrawLoading(1);
    InitShaders();
    DrawLoading(2);
    InitVertexBuffers();
    DrawLoading(3);
    InitTextures();
    DrawLoading(4);
}

Renderer::Renderer( Level* l, Player* p ):
    level(l), player(p), 
        land_texture( 10, 10 ),
        item_glow_texture(6,6),
        item_sprite_count(0)
{
    int v[4];
    glGetIntegerv( GL_VIEWPORT, v );
    screen_x= v[2];
    screen_y= v[3];
    fov= 0.8f * MW_PI2;
}


bool Renderer::IsObjectBehindPlayer( const float* obj_pos, float obj_radius )
{
    float vec_to_object[3];
    float player_vec[3];

  
    player_vec[0]= cos( player->AngXY()+ MW_PI2 );
    player_vec[1]= sin( player->AngXY()+ MW_PI2  );
    player_vec[2]= sin( player->AngZ() );
    player_vec[0]*= cos( player->AngZ() );
    player_vec[1]*= cos( player->AngZ() );

    vec_to_object[0]= obj_pos[0];
    vec_to_object[1]= obj_pos[1];
    vec_to_object[2]= obj_pos[2];
    
    Vec3Sub( vec_to_object, player->Position() );

    float l= -Vec3Dot( vec_to_object, player_vec );

    return l > obj_radius;

}

void Renderer::CalculateViewMatrix()
{
    float translate[16], rotate_xy[16], rotate_z[16], perspective[16];
   
    Mat4Perspective( perspective, float(screen_x) / float(screen_y), fov, 0.0625f, 4096.0f );

    Mat4Identity( translate );
    Mat4Identity( rotate_z );
    Mat4Identity( this->view_matrix );
    Mat4Identity( this->inv_view_matrix );

    translate[12]= -player->Position()[0];
    translate[13]= -player->Position()[1];
    translate[14]= -player->Position()[2] - PLAYER_HEIGHT;
  
   
    Mat4RotateXY( rotate_xy, -player->AngXY() );

    rotate_z[5]= cos( -player->AngZ() - MW_PI2 );
    rotate_z[6]= sin( -player->AngZ() - MW_PI2);
    rotate_z[9]= - rotate_z[6];
    rotate_z[10]= rotate_z[5];
  //  rotate_z[14]= 0.0f;
   
    Mat4Mul( view_matrix, translate );
    Mat4Mul( view_matrix, rotate_xy );
    Mat4Mul( view_matrix, rotate_z );
    Mat4Mul( view_matrix, perspective );


    //calculate inverse view matrix
    rotate_z[5]= cos( player->AngZ() + MW_PI2 );
    rotate_z[6]= sin( player->AngZ() + MW_PI2 );
    rotate_z[9]= - rotate_z[6];
    rotate_z[10]= rotate_z[5];
    //rotate_z[14]= 0.0f;

    Mat4RotateXY( rotate_xy, player->AngXY() );

    translate[12]= player->Position()[0];
    translate[13]= player->Position()[1];
    translate[14]= player->Position()[2] + PLAYER_HEIGHT - 0.2f;

    Mat4Mul( inv_view_matrix, rotate_z );
    Mat4Mul( inv_view_matrix, rotate_xy );
    memcpy( inv_view_normal_matrix, inv_view_matrix, 16 * sizeof(float) );
    Mat4ToMat3( inv_view_normal_matrix );
    Mat4Mul( inv_view_matrix, translate );


    
}


void Renderer::DrawLandscape()
{
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, landscape_heightmap_texture );
    glActiveTexture( GL_TEXTURE1 );
    glBindTexture( GL_TEXTURE_2D_ARRAY, land_texture_array );
    glActiveTexture( GL_TEXTURE2 );
    glBindTexture( GL_TEXTURE_2D, land_texture_map );
    glActiveTexture( GL_TEXTURE3 );
    glBindTexture( GL_TEXTURE_2D, land_shadow_map );
 

    landscape_shader.Bind();
    landscape_shader.UniformInt( 0, 0 );//heightmap
    landscape_shader.UniformInt( 3, 1 );//texture
    landscape_shader.UniformInt( 5, 2 );//map
    landscape_shader.UniformInt( 7, 3 );//shadow map
    landscape_shader.UniformMat4( 2, view_matrix );
    landscape_shader.UniformFloat( 4, level->GetLandscape()->Amplitude() );

    //static const float sun[]= { -0.1f, 0.5f, 1.0f };
    landscape_shader.UniformVec3( 6, sun_vector );
    CalculateViewMatrix();

    float position_shift[]= { -32.0f, -32.0f, 0.0f };

    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    landscape_vbo.Bind();


    int player_x, player_y;
    player_x= (int) player->Position()[0] / QUADS_PER_CHUNK;
    player_y= (int) player->Position()[1] / QUADS_PER_CHUNK;
    int r;
#ifdef MW_DEBUG
    unsigned int triangle_count= 0, chunk_count= 0;
#endif
    float chunk_pos[3];
    for( int i= 0; i< (int)level->GetLandscape()->SizeX() / QUADS_PER_CHUNK; i++ )
        for( int j= 0; j< (int)level->GetLandscape()->SizeY() / QUADS_PER_CHUNK; j++ )
        {

            position_shift[0]= float( i * QUADS_PER_CHUNK );
            position_shift[1]= float( j * QUADS_PER_CHUNK );

            chunk_pos[0]= position_shift[0] + float(QUADS_PER_CHUNK/2);
            chunk_pos[1]= position_shift[1] + float(QUADS_PER_CHUNK/2);
            chunk_pos[2]= level->GetLandscape()->Height( chunk_pos[0], chunk_pos[1] );
            if( IsObjectBehindPlayer( chunk_pos, 96.0f ) )
                continue;
            landscape_shader.UniformVec3( 1, position_shift );

            unsigned int lod;
            static const unsigned int ind_count[]= { 24576, 6144, 1536, 384, 96 };

            r= (i -player_x)*(i -player_x)+(j -player_y)*(j -player_y);
            if( r < 2*2 )
               lod= 0;
            else if( r < 4*4 )
               lod= 1;
            else if( r < 8*8 )
               lod= 2;
            else if( r< 16*16 )
                lod= 3;
            else 
               lod= 4;
            glDrawElements( GL_TRIANGLES, ind_count[lod], GL_UNSIGNED_SHORT, (void*)(lod_shift[lod]* sizeof(short)) );
#ifdef MW_DEBUG
            triangle_count+= ind_count[lod]/3;
            chunk_count++;
#endif
        }
     
#ifdef MW_DEBUG
    char str[64];
    sprintf( str, "Triangles: %d", triangle_count );
    text.AddText( 0, 1, 1, Text::default_color, str );
    sprintf( str, "Chunks: %d", chunk_count );
    text.AddText( 0, 2, 1, Text::default_color, str );
#endif

    //glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void Renderer::DrawWater()
{
    water_texture->Bind(0);

    glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_BLEND );
    landscape_vbo.Bind();

    water_shader.Bind();
    water_shader.UniformMat4( 0, view_matrix );
    water_shader.UniformFloat( 1, level->GetLandscape()->WaterLevel() );
    float cam_pos[]= { player->Position()[0], player->Position()[1], player->Position()[2] + PLAYER_HEIGHT };
    water_shader.UniformVec3( 2, cam_pos );
    float sun[]= { sun_vector[0], sun_vector[1], sun_vector[2] };
    Vec3Normalize(sun);
    water_shader.UniformVec3( 3, sun );
    water_shader.UniformInt( 4, 0 );//texture
    water_shader.UniformFloat( 5, float(MainLoop::CurrentTime())/float(CLOCKS_PER_SEC ) );

    glDrawArrays( GL_TRIANGLES, (QUADS_PER_CHUNK + 1 )* (QUADS_PER_CHUNK + 1 ) * 2 - 6, 6 );

    glDisable( GL_BLEND );
}

void Renderer::GenLandscapeMesh()
{

    landscape_vertices= new LandscapeVertex[ (QUADS_PER_CHUNK + 1 ) * ( QUADS_PER_CHUNK + 1 ) * 2 ];
    landscape_indeces= new unsigned short[ 2 * (QUADS_PER_CHUNK * QUADS_PER_CHUNK * 6) ];
    LandscapeVertex* v;

    unsigned int i, j, k, p, s;
    unsigned int quads_per_chunk= QUADS_PER_CHUNK;
    unsigned short* ind= landscape_indeces;
    LandscapeVertex* lod_v= landscape_vertices;
    unsigned int v_shift= 0;
    for( k= 0; k< CHUNK_LOD_COUNT; k++ )
    {
    	s= QUADS_PER_CHUNK / quads_per_chunk;

        for( i= 0; i< quads_per_chunk + 1; i++ )
            for( j= 0; j< quads_per_chunk + 1; j++ )
            {
                v= lod_v + i + j * (quads_per_chunk+1);
                v->pos[0]= i*s;
                v->pos[1]= j*s;
            }

        for( i= 0; i< quads_per_chunk; i++ )
            for( j= 0; j< quads_per_chunk; j++ )
            {
                /*
                    1----23
                    |.../.|
                    |../..|
                    |./...|
                    |/....|
                    05----4

                     23----4
                     |.../.|
                     |../..|
                     |./...|
                     |/....|
                     1----05
                    */

                p= 6 * ( quads_per_chunk * j + i);
               
                if( (i+j)&1 )
                {
                    ind[p]= v_shift + i + j * ( quads_per_chunk + 1 );
                    ind[ p + 1 ]= ind[ p ] + ( quads_per_chunk + 1 );
                    ind[ p + 2 ]= ind[ p + 1 ] + 1;

                    ind[ p + 3 ]= ind[ p + 2 ];
                    ind[ p + 4 ]= ind[ p ] + 1;
                    ind[ p + 5 ]= ind[ p ];
                }
                else
                {
                    ind[p]= v_shift + i + j * ( quads_per_chunk + 1 ) + 1;
                    ind[ p + 1 ]= ind[ p ] - 1;
                    ind[ p + 2 ]= ind[ p + 1 ] + ( quads_per_chunk + 1 );

                    ind[ p + 3 ]= ind[ p + 2 ];
                    ind[ p + 4 ]= ind[ p + 3 ] + 1;
                    ind[ p + 5 ]= ind[ p ];
                }

            }

        lod_shift[k]= ind - landscape_indeces;
        v_shift+= (quads_per_chunk + 1)*(quads_per_chunk + 1);
        lod_v+= (quads_per_chunk + 1) * (quads_per_chunk + 1);
        ind+= quads_per_chunk * quads_per_chunk * 6;
        quads_per_chunk>>=1;
    }

    //last quad 
    v= landscape_vertices + (QUADS_PER_CHUNK + 1 )* (QUADS_PER_CHUNK + 1 ) * 2 - 6;
    v[0].pos[0]= 0;
    v[0].pos[1]= 0;
    v[1].pos[0]= 1024;
    v[1].pos[1]= 0;
    v[2].pos[0]= 1024;
    v[2].pos[1]= 1024;

    v[3].pos[0]= 0;
    v[3].pos[1]= 0;
    v[4].pos[0]= 0;
    v[4].pos[1]= 1024;
    v[5].pos[0]= 1024;
    v[5].pos[1]= 1024;
}


void Renderer::CalculateFPS()
{
    static unsigned int last_fps_time= 0;
    static unsigned int frames= 0;
    static unsigned int last_fps= 0;

    frames++;
    
    unsigned int current_time= clock();

    if( (current_time - last_fps_time )/CLOCKS_PER_SEC >= 1 )
    {
        last_fps= frames;
        frames= 0;
        last_fps_time= current_time;
    }

    char str[32];
    sprintf( str, "fps: %d", last_fps );

    text.AddText( 0, 0, 1, Text::default_color, str );
}


void Renderer::DrawMonsters()
{
    monster_vbo.Bind();

    animated_models_shader.Bind();
    animated_models_shader.UniformInt( 2, 1 );//texture 
    animated_models_shader.UniformVec3( 3, sun_vector );

    float m_t[16], m_r[16], m[16];
    Mat4Identity( m_t );

#ifdef MW_DEBUG
    unsigned int monster_count = 0;
#endif
    for( unsigned int i= 0; i< level->MonsterCount(); i++ )
    {
        const Monster* monster= level->GetMonster(i);
        const float* pos= monster->Position();
        unsigned char monster_id= monster->MonsterId();

        if( IsObjectBehindPlayer( pos, 8.0f ) )
            continue;
        m_t[12]= pos[0];
        m_t[13]= pos[1];
        m_t[14]= pos[2];

        Mat4RotateXY( m_r, monster->AngleXY() );

        Mat4Mul( m_r, m_t, m );
        Mat4Mul( m, view_matrix );

        Mat4ToMat3( m_r );

        DrawAnimatedModel( &monsters[monster_id], monster->ActiveAnimations(), monster->ActiveAnimationsTime(),
            m, m_r ,monsters_vertex_shift[monster_id], monsters_index_shift[monster_id],  &animated_models_shader );
#ifdef MW_DEBUG
        monster_count++;
#endif
    }
    #ifdef MW_DEBUG
    char str[64];
    sprintf( str, "monsters: %d", monster_count );
    text.AddText( 0, 4, 1, Text::default_color, str );
#endif
}

void Renderer::DrawBullets()
{
    level_shader.Bind();
    level_shader.UniformInt( 1, 1 );//texture 
    level_vbo.Bind();

    float m[16], mt[16], mr_xy[16], mr_z[16], m_n[16];
    Mat4Identity( mt );
    for( unsigned int i= 0; i< level->BulletCount(); i++ )
    {
        const Bullet* bullet= level->GetBullets() + i;
        mt[12]= bullet->pos[0];
        mt[13]= bullet->pos[1];
        mt[14]= bullet->pos[2];

        Mat4Identity( mr_xy );
        Mat4Identity( mr_z );
       // mr_z[5]= 0.0f;
        //mr_z[6]= -1.0f;
       // mr_z[9]= 1.0f;
       // mr_z[10]= 0.0f;
        float a= Vec3ZAngle( bullet->vel ) - MW_PI2;
        mr_z[5]= cos( a );
        mr_z[6]= sin( a );
        mr_z[9]= -mr_z[6];
        mr_z[10]= mr_z[5];
        Mat4RotateXY( mr_xy, Vec3XYAngle( bullet->vel ) - MW_PI2 );
        Mat4Mul( mr_z, mr_xy, m );
        memcpy( m_n, m, sizeof(float) * 16 );
        Mat4Mul( m, mt );
        Mat4Mul( m, view_matrix );

        float sun[3];
        Mat4Transpose( m_n );
        Vec3Mat4Mul( sun_vector, m_n, sun );
        level_shader.UniformMat4( 0, m );
        level_shader.UniformVec3(2, sun );

        unsigned int k= STATIC_OBJECTS + BULLET_ROCKET + bullet->bullet_type;
         glDrawElementsBaseVertex( GL_TRIANGLES, static_objects[k].index_count, GL_UNSIGNED_SHORT,
            (void*) (model_index_shift[k] *sizeof(short) ), model_vertex_shift[k] );

    }
}

void Renderer::DrawStaticModels()
{
    float m[16], m_t[16], m_r[16], m_s[16];
    Mat4Identity( m_t );
    Mat4Identity( m_s );


    level_shader.Bind();
    level_shader.UniformInt( 1, 1 );//texture 


    level_vbo.Bind();
    float sun[3];
    float vec2_object[3];
    float len;
#ifdef MW_DEBUG
    unsigned int models= 0;
#endif
    for( unsigned int k= 0; k< STATIC_OBJECTS; k++ )
    {
        for( unsigned int i=0; i< level->ObjectCount(k); i++ )
        {
            const StaticObject* obj= level->GetStaticObject(k, i );

            if( IsObjectBehindPlayer( obj->pos, 30.0f ) )
                continue;
            vec2_object[0]= obj->pos[0];
            vec2_object[1]= obj->pos[1];
            vec2_object[2]= obj->pos[2];
            Vec3Sub( vec2_object, player->Position() );
            len= Vec3Len( vec2_object ) + 0.00390625f;
            len= obj->height/len;
            if( len < 0.00390625f )// 1m object not visible in distance 256m
            {
                 continue;
            }

            if( len < 0.03125f && len >  0.00390625f )
                if( OBJECT_IS_PICKUP(k) )
                {
                        //draw glow around items
                    item_glow_vertices[item_sprite_count*4]= obj->pos[0];
                    item_glow_vertices[item_sprite_count*4+1]= obj->pos[1];
                    item_glow_vertices[item_sprite_count*4+2]= obj->pos[2];
                    item_glow_vertices[item_sprite_count*4+3]= pickup_size_table[k-OBJECT_SMALL_HEALTHPACK]*float(screen_y);
                    item_sprite_count++;
                }


            m_t[12]= obj->pos[0];
            m_t[13]= obj->pos[1];
            m_t[14]= obj->pos[2];
            m_s[0]= m_s[5]= m_s[10]= obj->scale;

            Mat4RotateXY( m_r, obj->angle );

            
            Mat4Mul( m_r, m_s , m );
            Mat4Mul( m, m_t );
            Mat4Mul( m, view_matrix );
            level_shader.UniformMat4( 0, m );

            Mat4RotateXY( m_r, -obj->angle  );
            Vec3Mat4Mul( sun_vector, m_r, sun );
            level_shader.UniformVec3( 2, sun );

            if( k == OBJECT_SPRUCE && Distance( obj->pos, player->Position() ) > 60.0f )
            {
                    int kk= STATIC_OBJECTS + BULLET_TYPE_COUNT;
                    glDrawElementsBaseVertex( GL_TRIANGLES, static_objects[kk].index_count, GL_UNSIGNED_SHORT,
                    (void*) (model_index_shift[kk] *sizeof(short) ), model_vertex_shift[kk] );
            }
            else
                glDrawElementsBaseVertex( GL_TRIANGLES, static_objects[k].index_count, GL_UNSIGNED_SHORT,
                (void*) (model_index_shift[k] *sizeof(short) ), model_vertex_shift[k] );
#ifdef MW_DEBUG
            models++;
#endif
        }
    }

#ifdef MW_DEBUG
    char str[64];
    sprintf( str, "static models: %d", models );
    text.AddText( 0, 3, 1, Text::default_color, str );
#endif
}

void Renderer::DrawWeapon()
{
    weapon_shader.Bind();
    weapon_shader.UniformVec3( 5, sun_vector );
    weapon_shader.UniformInt( 2, 1 );
    level_vbo.Bind();

    float m[16], m_t[16], m_p[16], m_r[16], m_s[16], m_rz[16];
  //  float to_global_m[16];
    float m_n[16];

    Mat4Perspective( m_p, float(screen_x) / float(screen_y), fov, 0.0625f, 8.0f  );
    Mat4Identity( m_t );
    Mat4Identity( m_s );
    Mat4Identity( m_rz );


    m_t[12]= 1.0f;
    m_t[13]= -1.1f;
    m_t[14]= -1.8f;//move weapon forward ( from camera )

    m_rz[5]= cos( -MW_PI2 );
    m_rz[6]= sin( -MW_PI2 );
    m_rz[9]= - m_rz[6];
    m_rz[10]= m_rz[5];//rotate weapon around camera x vector

    Mat4RotateXY( m_r, /*player->AngXY()*/0.0f );//rotate weapon around camera vector
 
    Mat4Mul( m_r, m_rz, m );
    Mat4Mul( m, m_t );
   // Mat4Mul( m, inv_view_matrix, to_global_m );
    Mat4Mul( m, m_p );

    //calculate normal matrix
    Mat4ToMat3( m_rz );
    Mat3Mul( m_rz, inv_view_normal_matrix, m_n );

   // weapon_shader.Bind();
    weapon_shader.UniformMat4( 4, m );
    weapon_shader.UniformMat4( 3, inv_view_matrix );
    Mat4Identity(m);


    glDepthRange( 0.0f, 0.1f );

    unsigned char weapon_id= player->CurrentWeapon() + OBJECT_SHOVEL;
    bool anim_active[]= { player->WeaponAnimationIsActive(), false, false, false };
    float anim_time[]= { player->WeaponAnimationTime(), 0.0f, 0.0f, 0.0f };
    DrawAnimatedModel( &static_objects[weapon_id], anim_active, anim_time,
           m, m_n , model_vertex_shift[weapon_id], model_index_shift[weapon_id], &weapon_shader );

    glDepthRange( 0.0f, 1.0f );
}

void Renderer::DrawItemGlow()
{
    glBlendFunc( GL_ONE, GL_ONE );
    glEnable( GL_BLEND );
    glEnable( GL_PROGRAM_POINT_SIZE );

    item_glow_texture.Bind(0);

    item_glow_shader.Bind();
    item_glow_shader.UniformMat4( 0, view_matrix );
    item_glow_shader.UniformInt(1,0);

    item_glow_vbo.Bind();
    item_glow_vbo.VertexSubData( item_glow_vertices, item_sprite_count * 4 * sizeof(float), 0 );

    glDrawArrays( GL_POINTS, 0, item_sprite_count );

    item_sprite_count= 0;

    glDisable( GL_BLEND );
}

void Renderer::DrawParticles()
{
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D_ARRAY, particles_texture_array );

    particle_shader.Bind();
    particle_shader.UniformMat4( 0, view_matrix );
    particle_shader.UniformInt( 1, 0 );//texture

    particles_vbo.Bind();
    unsigned int p_count= level->GetParticleManager()->ParticleCount();
    particles_vbo.VertexSubData( level->GetParticleManager()->GetPositionBuffer(), 
                                p_count * 4 * sizeof(float), 0 );

    glDrawArrays( GL_POINTS, 0, p_count );

    glDisable( GL_BLEND );
}

void Renderer::DrawAnimatedModel( const Model* m, const bool* active_animations, const float* animation_time,
                  const float* v_matrix, const float* n_matrix, unsigned int v_shift, unsigned int i_shift, GLSLProgram* shader  )
{
    float vertex_matrices[16*MAX_MODEL_BONES];
    float normal_matrices[9*MAX_MODEL_BONES];
    for( unsigned int i= 0; i< MAX_MODEL_BONES; i++ )
    {
       memcpy( vertex_matrices + 16*i, v_matrix, 16*sizeof(float) );
       memcpy( normal_matrices + 9*i, n_matrix, 9*sizeof(float) );

    }


    const ModelAnimation* anim;
   // if( m->animation_count != 0 )
    for( unsigned int i= 0; i< MAX_MODEL_ANIMATIONS; i++ )
    {
        if( !active_animations[i] || !m->is_animation[i] )
            continue;

        anim= & (m->animations[i]);
        for( unsigned int b= 0, j= 0; b< MAX_MODEL_BONES; b++ )
        {
            if( anim->bones[b] )
            {
                unsigned int frame= int( float(anim->frames) * animation_time[i] / anim->duration )%anim->frames;
                Mat4Mul( anim->v_matrices + ( frame*anim->bone_count + j )*16, v_matrix, vertex_matrices + 16*b );
                Mat3Mul( anim->n_matrices + ( frame*anim->bone_count + j )*9, n_matrix, normal_matrices + 9*b );
                j++;
            }
            else
                continue;
        }
    }

    shader->UniformMat4Array( 1, MAX_MODEL_BONES, vertex_matrices );
    shader->UniformMat3Array( 0, MAX_MODEL_BONES, normal_matrices );

    glDrawElementsBaseVertex( GL_TRIANGLES, m->index_count, GL_UNSIGNED_SHORT,
        (void*)(i_shift*sizeof(short)), v_shift );
}

/*void Renderer::DrawAnimatedModels()
{//return;
   monster_vbo.Bind();

    animated_models_shader.Bind();
    animated_models_shader.UniformInt( 2, 1 );//texture 


    float m[16],n_m[16];
    Mat4Identity(m);
    Mat4Identity(n_m);
    Mat4ToMat3(n_m);
    m[12]= 512.0f;
    m[13]= 512.0f;
    m[14]= level->GetLandscape()->Height(512.0f,512.0f);
    Mat4Mul(m,view_matrix);
    bool active[]= {false, false, false, false };
    float time=  float(clock())/float(CLOCKS_PER_SEC);
    float t2[]= { time, time, time, time };
    animated_models_shader.UniformVec3( 3, sun_vector );
    DrawAnimatedModel( &monsters[1], active, t2, m, n_m, monsters_vertex_shift[1], monsters_index_shift[1] );

   
}*/


void Renderer::DrawLoading( unsigned int stage )
{
    static const char* strings[]= {
        "Generate terrain shadowmap...",
        "Compile shaders...",
        "Generate models...",
        "Generate textures...", "Ready!" };

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    text.AddText( 1, 2, 2, Text::default_color, "MicroW - 64k shooter" );
    text.AddText( 1, 3, 1, Text::default_color, "by \"Panzerschrek\"" );

    text.AddText( screen_x/LETTER_WIDTH/2, screen_y/LETTER_HEIGHT/2, 1, Text::default_color, strings[ stage ] );

    glDisable(GL_DEPTH_TEST);
    text.Draw();
    glEnable(GL_DEPTH_TEST);

    MainLoop::SwapBuffers();
}

void Renderer::DrawDeathScreen()
{
    unsigned int x=screen_x / LETTER_WIDTH/2 - 7,
        y= screen_y / LETTER_HEIGHT/2 + 4;
    text.AddText( x, y, 2, Text::default_color, "You Die" );
    char str[32];
    sprintf( str, "Score: %d", player->Score() );
    text.AddText( x-2, y+2, 2, Text::default_color, str );
}

void Renderer::DrawGUI()
{
    char str[128];
    /*sprintf( str, "Player: %5.2f %5.2f %5.2f", player->Position()[0], player->Position()[1], player->Position()[2] );
    text.AddText( 0, 7, 1, Text::default_color, str );
    sprintf( str, "on surface: %d\n", player->OnSurface() );
    text.AddText( 0, 8, 1, Text::default_color, str );
    sprintf( str, "velocity: %3.1f", player->Velocity() );
    text.AddText( 0, 9, 1, Text::default_color, str );

    sprintf( str, "Angle xy:%1.3f z:%1.3f", player->AngXY(), player->AngZ() );
    text.AddText( 0, 10, 1, Text::default_color, str );*/

    unsigned int x, y;
    x= screen_x / LETTER_WIDTH - 17;
    y= screen_y / LETTER_HEIGHT - 4;

    sprintf( str, "Score: %d", player->Score() );
    text.AddText( 1, y, 1, Text::default_color, str );
	sprintf( str, "Round: %d", level->Wave() + 1);
	text.AddText( 1, y+1, 1, Text::default_color, str );

    if( player->AmmoType() != AMMO_NOAMMO )
    {
        sprintf( str, "Ammo: %3d", player->Ammo() );
        text.AddText( x+4, y, 1, Text::default_color, str );
    }
    else
        text.AddText( x+4, y, 1, Text::default_color, "Ammo: inf." );
    y+=2;

    static unsigned char hc[]= { 
        255, 32, 32, 32,
        255, 255, 32, 32,
        255, 255, 255, 32 };
    text.AddText( x, y, 1, Text::default_color, "Health: " );
    unsigned char* color;
    if( player->Health() <= 35 ) color= hc;
    else if( player->Health() <= 75 )color= hc+4;
    else color= hc+8; 
    sprintf( str, "%d", player->Health() );
    text.AddText( x+8, y, 1 + (int)(player->Health()<45), color, str );

    text.DrawCross();

    glDisable(GL_DEPTH_TEST);
    text.Draw();
    glEnable(GL_DEPTH_TEST);
}

void Renderer::DrawSky()
{
    level_vbo.Bind();

    float m[16];
    Mat4Identity(m);
    m[12]= player->Position()[0];
    m[13]= player->Position()[1];
    m[14]= player->Position()[2] - PLAYER_HEIGHT + 0.2f;
    Mat4Mul(m,view_matrix);

    cloud_texture->Bind(0);

    sky_shader.Bind();
    sky_shader.UniformMat4( 0,m );
    sky_shader.UniformInt( 2, 0 );
    float cloud_shift[]= { 0.005f * float(MainLoop::CurrentTime())/float( CLOCKS_PER_SEC), 0.0f, 0.0f };
    cloud_shift[1]= cloud_shift[0] * 0.25f;
    sky_shader.UniformVec3( 3, cloud_shift );

    float sun[3]= { sun_vector[0], sun_vector[1], sun_vector[2] };
    Vec3Normalize(sun);
    sky_shader.UniformVec3( 1, sun );


    //use model of medium healthpack becouse it is a shpere
    int k= OBJECT_MEDIUM_HEALTHPACK;
    glDrawElementsBaseVertex( GL_TRIANGLES, static_objects[k].index_count, GL_UNSIGNED_SHORT,
            (void*) (model_index_shift[k] *sizeof(short) ), model_vertex_shift[k] );
}

void Renderer::DrawBlood()
{
    if( player->BloodFactor() < 0.01f )
        return;

    glEnable( GL_BLEND );
    glDisable( GL_DEPTH_TEST );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    blood_texture->Bind(0);

    blood_shader.Bind();
    blood_shader.UniformFloat( 0, player->BloodFactor() );
    blood_shader.UniformInt( 1, 0 );

    glDrawArrays( GL_TRIANGLES, 0, 6 );

    glDisable( GL_BLEND );
    glEnable( GL_DEPTH_TEST );
}

void Renderer::Draw()
{

    CalculateViewMatrix();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable( GL_CULL_FACE );
    DrawLandscape();
    glDisable( GL_CULL_FACE );
    DrawSky();
    DrawStaticModels();
    DrawBullets();
    if( player->Health() > 0 )
        DrawWeapon();

    DrawMonsters();

    DrawWater();
    DrawItemGlow();
    DrawParticles();
    DrawBlood();


    if( player->Health() <= 0 )
        DrawDeathScreen();
    DrawGUI();
    CalculateFPS();
}



