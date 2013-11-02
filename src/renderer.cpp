#include <time.h>

#include "renderer.h"
#include "shaders.h"
#include "math.h"
#include "landscape.h"
#include "texture_generation.h"
#include "model.h"
#include "model_generation.h"


void Renderer::InitTextures()
{

    glGenerateMipmap( GL_TEXTURE_2D );


    //texture array for all textures
    glGenTextures(1, &land_texture_array );
    glBindTexture( GL_TEXTURE_2D_ARRAY, land_texture_array );
    glTexImage3D( GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8,
                1024, 1024, 32,//size
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


    const Landscape* land= level->GetLandscape();

    //texture map for landscape
    glGenTextures( 1, &land_texture_map );
    glBindTexture( GL_TEXTURE_2D, land_texture_map );    
    glTexImage2D( GL_TEXTURE_2D, 0, GL_R8,
                  land->SizeX(), land->SizeY(), 0,
                  GL_RED,  GL_UNSIGNED_BYTE, land->Texturemap() );

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

}


void Renderer::InitVertexBuffers()
{
    GenCacodemon( &monsters[0] );
    GenRobot( &monsters[1] );
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



    /*GenRobot( &animated_models[0] );
    animated_models_vbo.GenVAO();
    animated_models_vbo.VertexData( animated_models[0].vertices,
        animated_models[0].vertex_count * sizeof(ModelVertex), sizeof(ModelVertex) );
    animated_models_vbo.IndexData( animated_models[0].indeces, animated_models[0].index_count * sizeof(short) );
    animated_models_vbo.VertexAttrib( 0, 3, GL_SHORT, false, 0 );//position
    animated_models_vbo.VertexAttrib( 1, 3, GL_SHORT, false, 6 );//texture coord
    animated_models_vbo.VertexAttrib( 2, 3, GL_BYTE, true, 6 + 6 );//normal
    animated_models_vbo.VertexAttribInt( 3, 1, GL_UNSIGNED_BYTE, 6 + 6 + 3 );*/

    /*GenShovel(&weapons[0]);
    GenPistol(&weapons[1] );
    GenMachineGun(&weapons[2] );
    v_count= 0, i_count= 0; 
    for( unsigned int i= 0; i< 3; i++ )
    {
        v_count+= weapons[i].vertex_count;
        i_count+= weapons[i].index_count;
    }
    player_weapon_vbo.GenVAO();
    player_weapon_vbo.VertexData( NULL, v_count * sizeof(ModelVertex), sizeof(ModelVertex) );
    player_weapon_vbo.IndexData( NULL, i_count * sizeof(short) );
    for( unsigned int i= 0, vs= 0, is= 0; i< 3; i++ )
    {
        weapons_vertex_shift[i]= vs;
        weapons_index_shift[i]= is;
        player_weapon_vbo.VertexSubData( weapons[i].vertices,
            weapons[i].vertex_count * sizeof(ModelVertex), vs * sizeof(ModelVertex) );
        player_weapon_vbo.IndexSubData( weapons[i].indeces,
            weapons[i].index_count * sizeof(short), is * sizeof(short) );

        vs+= weapons[i].vertex_count;
        is+= weapons[i].index_count;
     }
    player_weapon_vbo.VertexAttrib( 0, 3, GL_SHORT, false, 0 );//position
    player_weapon_vbo.VertexAttrib( 1, 3, GL_SHORT, false, 6 );//texture coord
    player_weapon_vbo.VertexAttrib( 2, 3, GL_BYTE, true, 6 + 6 );//normal*/





    GenTree( &static_objects[ OBJECT_SPRUCE ] );
    GenAchtungTable( &static_objects[ OBJECT_ACHTUNG_TABLE ] );
    GenSmallHealthPack( &static_objects[ OBJECT_SMALL_HEALTHPACK ] );
    GenMediumHealthPack( &static_objects[ OBJECT_MEDIUM_HEALTHPACK ] );
    GenLargeHealthPack( &static_objects[ OBJECT_LARGE_HEALTHPACK ] );
    GenShovel( &static_objects[OBJECT_SHOVEL] );
    GenPistol( &static_objects[OBJECT_PISTOL] );
    GenMachineGun( &static_objects[OBJECT_MACHINEGUN] );
    v_count= 0, i_count= 0; 
    for( unsigned int i= 0; i< STATIC_OBJECTS; i++ )
    {
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
    for( unsigned int i= 0, vs=0, is= 0; i< STATIC_OBJECTS; i++ )
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

}


void Renderer::Init()
{

    InitShaders();
    InitVertexBuffers();
    InitTextures();


   sun_vector[0]= 0.7f;
   sun_vector[1]= 0.5f;
   sun_vector[2]= 0.5f;
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
   
    Mat4Perspective( perspective, float(screen_x) / float(screen_y), 0.8f* MW_PI2, 0.0625f, 4096.0f );

    Mat4Identity( translate );
    Mat4Identity( rotate_z );
    Mat4Identity( this->view_matrix );
    Mat4Identity( this->inv_view_matrix );

    translate[12]= -player->Position()[0];
    translate[13]= -player->Position()[1];
    translate[14]= -player->Position()[2] - PLAYER_HEIGHT + 0.2f;
  
   
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
 

    landscape_shader.Bind();
    landscape_shader.UniformInt( 0, 0 );//heightmap
    landscape_shader.UniformInt( 3, 1 );//texture
    landscape_shader.UniformInt( 5, 2 );//map
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
    unsigned int triangle_count= 0;

   // float chunk_pos[3];
    for( int i= 0; i< (int)level->GetLandscape()->SizeX() / QUADS_PER_CHUNK; i++ )
        for( int j= 0; j< (int)level->GetLandscape()->SizeY() / QUADS_PER_CHUNK; j++ )
        {
           // chunk_pos[0]= float( i * QUADS_PER_CHUNK + QUADS_PER_CHUNK/2 );
            //chunk_pos[1]= float( j * QUADS_PER_CHUNK + QUADS_PER_CHUNK/2 );
            //chunk_pos[2]= 0.0f;
            //if( IsObjectBehindPlayer( chunk_pos, float(QUADS_PER_CHUNK) ) )
             //   continue;

            position_shift[0]= float( i * QUADS_PER_CHUNK );
            position_shift[1]= float( j * QUADS_PER_CHUNK );
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
            triangle_count+= ind_count[lod]/3;
        }
     
    char str[64];
    sprintf( str, "Triangles: %d", triangle_count );
    text.AddText( 0, 1, 1, Text::default_color, str );

    //glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );



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

    for( unsigned int i= 0; i< level->MonsterCount(); i++ )
    {
        const Monster* monster= level->GetMonster(i);
        const float* pos= monster->Position();
        unsigned char monster_id= monster->MonsterId();

        m_t[12]= pos[0];
        m_t[13]= pos[1];
        m_t[14]= pos[2];

        Mat4RotateXY( m_r, monster->AngleXY() );

        Mat4Mul( m_r, m_t, m );
        Mat4Mul( m, view_matrix );

        Mat4ToMat3( m_r );

        DrawAnimatedModel( &monsters[monster_id], monster->ActiveAnimations(), monster->ActiveAnimationsTime(),
            m, m_r ,monsters_vertex_shift[monster_id], monsters_index_shift[monster_id] );
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
    for( unsigned int k= 0; k< STATIC_OBJECTS; k++ )
    {
        for( unsigned int i=0; i< level->ObjectCount(k); i++ )
        {
            const StaticObject* obj= level->GetStaticObject(k, i );

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
                if( k >= OBJECT_SMALL_HEALTHPACK && k<= OBJECT_MACHINEGUN )
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

            glDrawElementsBaseVertex( GL_TRIANGLES, static_objects[k].index_count, GL_UNSIGNED_SHORT,
            (void*) (model_index_shift[k] *sizeof(short) ), model_vertex_shift[k] );
        }
    }
    
}

void Renderer::DrawWeapon()
{
    animated_models_shader.Bind();
    animated_models_shader.UniformVec3( 3, sun_vector );
    level_vbo.Bind();

    float m[16], m_t[16], m_p[16], m_r[16], m_s[16], m_rz[16];
    float m_n[16];

    Mat4Perspective( m_p, float(screen_x) / float(screen_y), 0.8f * 1.57f, 0.0625f, 8.0f  );
    Mat4Identity( m_t );
    Mat4Identity( m_s );
    Mat4Identity( m_rz );


    m_t[12]= 0.8f;
    m_t[13]= -0.9f;
    m_t[14]= -1.6f;//move weapon forward ( from camera )

    m_rz[5]= cos( -MW_PI2 );
    m_rz[6]= sin( -MW_PI2 );
    m_rz[9]= - m_rz[6];
    m_rz[10]= m_rz[5];//rotate weapon around camera x vector

    Mat4RotateXY( m_r, /*player->AngXY()*/0.0f );//rotate weapon around camera vector
 
    Mat4Mul( m_r, m_rz, m );
    Mat4Mul( m, m_t );
    Mat4Mul( m, m_p );
    m[14]= -0.99f;//compress z coordinate of result vertices ( in screen space )

    //calculate normal matrix
    Mat4ToMat3( m_rz );
    Mat3Mul( m_rz, inv_view_normal_matrix, m_n );


    unsigned char weapon_id= player->CurrentWeapon() + OBJECT_SHOVEL;
    bool anim_active[]= { player->WeaponAnimationIsActive(), false, false, false };
    float anim_time[]= { player->WeaponAnimationTime(), 0.0f, 0.0f, 0.0f };
    DrawAnimatedModel( &static_objects[weapon_id], anim_active, anim_time,
           m, m_n , model_vertex_shift[weapon_id], model_index_shift[weapon_id] );

  
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
}


void Renderer::DrawAnimatedModel( const Model* m, const bool* active_animations, const float* animation_time,
                  const float* v_matrix, const float* n_matrix, unsigned int v_shift, unsigned int i_shift  )
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

    animated_models_shader.UniformMat4Array( 1, MAX_MODEL_BONES, vertex_matrices );
    animated_models_shader.UniformMat3Array( 0, MAX_MODEL_BONES, normal_matrices );

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

void Renderer::DrawFirstFrame()
{
    text.AddText( 1, 2, 2, Text::default_color, "MicroW - 64k shooter" );
    text.AddText( 1, 3, 1, Text::default_color, "by \"Panzerschrek\"" );

    text.AddText( screen_x/LETTER_WIDTH/2, screen_y/LETTER_HEIGHT/2, 1, Text::default_color, "Loading..." );

    glDisable(GL_DEPTH_TEST);
    text.Draw();
    glEnable(GL_DEPTH_TEST);
}


void Renderer::DrawGUI()
{
    char str[128];
    sprintf( str, "Player: %5.2f %5.2f %5.2f", player->Position()[0], player->Position()[1], player->Position()[2] );
    text.AddText( 0, 7, 1, Text::default_color, str );
    sprintf( str, "on surface: %d\n", player->OnSurface() );
    text.AddText( 0, 8, 1, Text::default_color, str );
    sprintf( str, "velocity: %3.1f", player->Velocity() );
    text.AddText( 0, 9, 1, Text::default_color, str );

    sprintf( str, "Angle xy:%1.3f z:%1.3f", player->AngXY(), player->AngZ() );
    text.AddText( 0, 10, 1, Text::default_color, str );

    sprintf( str, "Health: %3d", player->Health() );
    unsigned int x, y;
    x= screen_x / LETTER_WIDTH - 13;
    y= screen_y / LETTER_HEIGHT - 3;
     text.AddText( x, y, 1, Text::default_color, str );

      y++;
       sprintf( str, "Ammo: %3d", player->Ammo() );
        text.AddText( x, y, 1, Text::default_color, str );

     //text.AddText( screen_x/LETTER_WIDTH/2 - 1, screen_y/LETTER_HEIGHT/2, 1, Text::default_color, "-^-" );//cross
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

    sky_shader.Bind();
    sky_shader.UniformMat4( 0,m );


    //use model of large healthpack becouse it is a cube
    int k= OBJECT_LARGE_HEALTHPACK;
    glDrawElementsBaseVertex( GL_TRIANGLES, static_objects[k].index_count, GL_UNSIGNED_SHORT,
            (void*) (model_index_shift[k] *sizeof(short) ), model_vertex_shift[k] );
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
    DrawWeapon();
    DrawMonsters();
    DrawItemGlow();


    
    CalculateFPS();

    DrawGUI();

}



