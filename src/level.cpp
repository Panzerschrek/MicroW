#include "level.h"
#include "player.h"
#include "math.h"
#include "mw.h"
#include <time.h>
#include "texture_generation.h"


inline void Level::PlaceTrees()
{
    const unsigned int k= OBJECT_SPRUCE;
    object_count[ k ]= 1024;
    static_objects[k] = new StaticObject[ object_count[k] ];
    for( unsigned int i= 0; i< object_count[ k ]; i++ )
    {
        int x, y;
        x= 16 + 992 * rand() / RAND_MAX;
        y= 16 + 992 * rand() / RAND_MAX;

        if( landscape.CanPlaceObject( x-4, y-4, x+4, y+4 ) )
        {
            landscape.PlaceObject( x-4, y-4, x+4, y+4, &static_objects[k][i] );

            static_objects[k][i].pos[0]= float(x)+0.5f;
            static_objects[k][i].pos[1]= float(y)+0.5f;

            static_objects[k][i].pos[2]= landscape.Height( static_objects[k][i].pos[0],
            static_objects[k][i].pos[1] ) - 0.2f;

            landscape.SetTexture( x, y, TEXTURE_DIRT, 4 );

            static_objects[k][i].angle= MW_PI * 2.0f * float(rand())/float(RAND_MAX);

            static_objects[k][i].scale= 0.7f + 0.5f*float(rand())/float(RAND_MAX);//scale
            static_objects[k][i].height= SPRUCE_HEIGHT;
            static_objects[k][i].radius= SPRUCE_RADIUS;

            static_objects[k][i].object_id= OBJECT_SPRUCE;
        }
        else
            i--;
    }
}

inline void Level::PlaceAchtungTables()
{
   const unsigned int k= OBJECT_ACHTUNG_TABLE;
     object_count[ k ]= 64;
    static_objects[ k ] = new StaticObject[ object_count[k] ];
    float a= MW_PI * 0.0f, da= MW_PI * 2.0f / float(64);
    for( unsigned int i= 0; i< object_count[k]; i++, a+= da )
    {
        float x, y;
        static_objects[k][i].pos[0]= x= COMBAT_ZONE_RADIUS * cos(a) + 512.0f;
        static_objects[k][i].pos[1]= y= COMBAT_ZONE_RADIUS * sin(a) + 512.0f; 

        static_objects[k][i].pos[2]= landscape.Height( x,y );

        landscape.PlaceObject( int(x),int(y), int(x), int(y), &static_objects[k][i] );

        static_objects[k][i].angle= a - MW_PI * 0.5f;

        static_objects[k][i].scale= 1.0f;//scale
        static_objects[k][i].height= ACHTUNG_TABLE_HEIGHT;
        static_objects[k][i].radius= ACHTUNG_TABLE_RADIUS;
        static_objects[k][i].object_id= OBJECT_ACHTUNG_TABLE;
    }
}

inline void Level::PlaceHealthPacks()
{
    static unsigned int count[]= { 128, 48, 16 };
    for( unsigned int k= OBJECT_SMALL_HEALTHPACK; k<= OBJECT_LARGE_HEALTHPACK; k++ )
    {
        object_count[k]= count[k-OBJECT_SMALL_HEALTHPACK];
        static_objects[k]= new StaticObject[ object_count[k] ];
        for( unsigned int i= 0; i< object_count[k]; i++ )
        {
            int x, y;
            x= 16 + 992 * rand() / RAND_MAX;
            y= 16 + 992 * rand() / RAND_MAX;
        
            if( landscape.CanPlaceObject( x-1, y-1, x+1, y+1 ) )
            {
                landscape.PlaceObject( x-1, y-1, x+1, y+1, &static_objects[k][i] );
                landscape.SetTexture( x, y, x, y, TEXTURE_TILE );
                landscape.SetMaximumHeight( x-1, y-1, x+1, y+1 );

            
                static_objects[k][i].pos[0]= float(x)+0.5f;
                static_objects[k][i].pos[1]= float(y)+0.5f;
                static_objects[k][i].pos[2]= landscape.Height( float(x)+0.5f, float(y)+0.5f ) + 1.0f;
                static_objects[k][i].scale= 1.0f;
                static_objects[k][i].height= 0.5f;
                static_objects[k][i].radius= -1.0f;// negative radius - no collision
                static_objects[k][i].object_id= k;
                static_objects[k][i].angle= MW_2PI * float(rand()) /float(RAND_MAX);
             }
             else
                 i--;
         }//i
    }//k


}

inline void Level::PlaceWeapons()
{
    static unsigned int count[]= { 1, 64, 32 };
    for( unsigned int k= OBJECT_SHOVEL; k<= OBJECT_MACHINEGUN; k++ )
    {
        object_count[k]= count[k-OBJECT_SHOVEL];
        static_objects[k]= new StaticObject[ object_count[k] ];
        for( unsigned int i= 0; i< object_count[k]; i++ )
        {
            int x, y;
            x= 16 + 992 * rand() / RAND_MAX;
            y= 16 + 992 * rand() / RAND_MAX;
        
            if( landscape.CanPlaceObject( x-2, y-2, x+2, y+2 ) )
            {
                landscape.PlaceObject( x-2, y-2, x+2, y+2, &static_objects[k][i] );
                landscape.SetTexture( x, y, 1, TEXTURE_TILE );
                landscape.SetMaximumHeight( x-2, y-2, x+2, y+2 );

            
                static_objects[k][i].pos[0]= float(x)+0.5f;
                static_objects[k][i].pos[1]= float(y)+0.5f;
                static_objects[k][i].pos[2]= landscape.Height( float(x)+0.5f, float(y)+0.5f ) + 1.0f;
                static_objects[k][i].scale= 1.0f;
                static_objects[k][i].height= 0.5f;
                static_objects[k][i].radius= -1.0f;// negative radius - no collision
                static_objects[k][i].object_id= k;
                static_objects[k][i].angle= MW_2PI * float(rand()) /float(RAND_MAX);
             }
             else
                 i--;
         }//i
    }//k
}


Level::Level() :
landscape( 1024, 1024 )
{
    PlaceAchtungTables();
    PlaceTrees();
    PlaceHealthPacks();
    PlaceWeapons();




}

void Level::RemoveObject( StaticObject* obj )
{
    unsigned int k= obj->object_id;
    unsigned int i= obj - static_objects[k];

    if( i < object_count[k] - 1 )
    {
        static_objects[k][i]= static_objects[k][ object_count[k] - 1];
    }
    object_count[k]--;
}

void Level::SetPlayer( Player* player )
{
    this->player= player;
       float pos[]= {512.0f, 512.0f, 0.0f };
    player->SetPosition( pos );

    monster_count= 2;
    pos[1]= 520.0f;
    pos[2]= 96.0f;
    monsters[0]= new Monster( player, this, MONSTER_ROBOT, pos );
    pos[1]= 500.0f;
    monsters[1]= new Monster( player, this, MONSTER_CACODEMON, pos );
}

Level::~Level()
{

}


bool Level::FindCollisionWithLandscape( const float* pos, const float* vec, float* collide_pos )
{
    collide_pos[0]= pos[0];
    collide_pos[1]= pos[1];
    collide_pos[2]= pos[2];
    float h;
    int i= 0;
    while(i<1000)
    {
        h= landscape.Height( collide_pos[0], collide_pos[1] );
        if( h >= collide_pos[2] )
            break;
        collide_pos[0]+= vec[0];
        collide_pos[1]+= vec[1];
        collide_pos[2]+= vec[2];
        i++;
    }
    if( i >= 1000 )
        return false;
    return true;
}

bool Level::FindCollisionWithObject( const float* pos, const float* vec, float* collide_pos )
{
    float vec2_object[3];
    float vec2_near_point[3];
    float dot, l1, l2;
    for( unsigned int k= 0; k< STATIC_OBJECTS; k++ )
        for( unsigned int i= 0; i< object_count[k]; i++ )
        {
            if( static_objects[k][i].radius < 0.0f )
                continue;
            vec2_object[0]= static_objects[k][i].pos[0] - pos[0];
            vec2_object[1]= static_objects[k][i].pos[1] - pos[1];
            vec2_object[2]= static_objects[k][i].pos[2] - pos[2];

            dot= vec2_object[0]*vec[0] + vec2_object[1]*vec[1] + vec2_object[2]*vec[2];

            vec2_near_point[0]= vec[0] * dot;
            vec2_near_point[1]= vec[1] * dot;
            vec2_near_point[2]= vec[2] * dot;

            l1= Vec3Len( vec2_object );
            l2= Vec3Len( vec2_near_point );

            if( static_objects[k][i].radius > sqrt( l1*l1 - l2*l2 ) )
            {
                collide_pos[0]= static_objects[k][i].pos[0];//pos[0] + vec2_object[0];
                collide_pos[1]= static_objects[k][i].pos[1];//pos[1] + vec2_object[1];
                collide_pos[2]= static_objects[k][i].pos[2];//pos[2] + vec2_object[2];
                return true;
            }

        }
        return false;
}

void Level::Shot( const float* pos, const float* vec, unsigned char weapon_id )
{
    float p[3];
    float l= 10000000.0f, l2;
    float vec_to_collision_point[3];
 
    sound_system->PlaySound( player->Position(), 1.0f, weapon_descriptor_table[weapon_id].sound_id, true );

     if( FindCollisionWithLandscape( pos, vec, p ) )
     {
         Vec3Sub( p, pos );
         vec_to_collision_point[0]= p[0];
         vec_to_collision_point[1]= p[1];
         vec_to_collision_point[2]= p[2];
         l= Vec3Len( vec_to_collision_point );
     }

     for( unsigned int i= 0; i< monster_count; i++ )
        if(  monsters[i]->IsCollision( pos, vec, p ) )
        {
            Vec3Sub( p, pos );
            l2= Vec3Len( p );
            if( l2 < l )
            {
                if( l2 < float(weapon_descriptor_table[ weapon_id ].max_distance) )
                    monsters[i]->Damage( weapon_descriptor_table[weapon_id].damage );
            }
        }
}

void Level::PickUp()
{
  
        int x, y;
        x= (int) player->Position()[0];
        y= (int) player->Position()[1];
        for( int i= x-1; i<=x+1; i++ )
            for( int j= y-1; j<=y+1; j++ )
            {
                StaticObject* obj= landscape.GetObject( i, j );
                if( obj != NULL )
                    if( obj->object_id >= OBJECT_SMALL_HEALTHPACK &&  obj->object_id <= OBJECT_MACHINEGUN )
                    {
                        float h= obj->pos[2];
                        h-= player->Position()[2];
                        if( h * h < 2.0f )
                        {
                            if( obj->object_id<= OBJECT_LARGE_HEALTHPACK )
                            {
                                int dmg= healthpack_table[ obj->object_id - OBJECT_SMALL_HEALTHPACK ];
                                player->Damage( -dmg );
                            }
                            else if( obj->object_id <= OBJECT_MACHINEGUN )
                            {
                                player->GiveWeapon( obj->object_id - OBJECT_SHOVEL );
                            }
                            landscape.PlaceObject( i,j, i,j, NULL );
                            RemoveObject( obj );
                        }//h
                    }// if can pickup
            }//ij
    
}

void Level::PhysTick()
{
    static unsigned int prev_time= 0;
    unsigned int current_time= clock();
    float dt= float( current_time - prev_time ) / float( CLOCKS_PER_SEC );
    prev_time= current_time;


 
    for( unsigned int i= 0; i< monster_count; i++ )
    {
        if( monsters[i]->Helath() <= 0 )
        {
            float pos[]= { 512, 512, 78 };
            delete monsters[i];
            monsters[i]= new Monster( player, this, rand()%MONSTER_COUNT, pos);
        }
        else
             monsters[i]->Tick(dt);
    }
   
    /*if( monster_count == 0 )
    {
        float pos[]= { 512, 512, 78 };
        monsters[0]= new Monster( player, this, 0, pos);
        pos[2]+= 6.0f;
        monsters[1]= new Monster( player, this, 0, pos);
        monster_count=2;
    }*/

    player->Tick(dt);
     CollidePlayer();
    
    float v[3];
    v[0]= player->Position()[0] - 512.0f;
    v[1]= player->Position()[1] - 512.0f;
    v[2]= 0;
    if( Vec3Len(v) > COMBAT_ZONE_RADIUS + 2.0f )
    {
        v[0]= 512.0f;
        v[1]= 512.0f;
        player->Damage( 100500 );
        player->SetPosition(v);
    }
 
    PickUp();

    for( unsigned int k= OBJECT_SMALL_HEALTHPACK; k<= OBJECT_MACHINEGUN; k++ )
        for( unsigned int i= 0; i< object_count[k]; i++ )
        {
            static_objects[k][i].angle+= MW_PI2 * dt;
            if( static_objects[k][i].angle >= MW_2PI )
                static_objects[k][i].angle-= MW_2PI;
        }
 
}

void Level::CollidePlayer()
{
    float r, r_min;
    float vec[3];
   // float vel[3];
    float pos[3];
    float min[2], max[2];
    min[0]= player->Position()[2];
    max[0]= min[0] + PLAYER_HEIGHT;
    for( unsigned int k= 0; k< STATIC_OBJECTS; k++ )
    for( unsigned int i= 0; i< object_count[k]; i++ )
    {
        if( static_objects[k][i].radius < 0.0f )
            continue;
        r_min= static_objects[k][i].radius+ PLAYER_RADIUS;

        vec[0]= player->Position()[0] - static_objects[k][i].pos[0];
        vec[1]= player->Position()[1] - static_objects[k][i].pos[1];

        min[1]= static_objects[k][i].pos[2];
        max[1]= min[1] + static_objects[k][i].height;//height

        if(! ( max[0] < min[1] || max[1] < min[0]) )
        {
            r= sqrt( vec[0] * vec[0] + vec[1] * vec[1] );
            if( r < r_min )
            {
          
                pos[2]= player->Position()[2];
                pos[0]= static_objects[k][i].pos[0] + vec[0] * r_min / r;
                pos[1]= static_objects[k][i].pos[1] + vec[1] * r_min / r;
                player->SetPosition(pos);
          
            }
        }
    }
}