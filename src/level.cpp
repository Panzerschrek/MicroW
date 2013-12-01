#include "level.h"
#include "player.h"
#include "math.h"
#include "mw.h"
#include <time.h>
#include "texture_generation.h"
#include "main_loop.h"

void Level::PlaceTrees()
{
    Texture t( 10, 10 );
    t.Noise();
    unsigned char* t_data= (unsigned char*) t.Data();

    unsigned int count[]= { 1024-256, 128, 128, 64 };
  //  float height[]= { SPRUCE_HEIGHT, SPRUCE_HEIGHT
    for( unsigned int k= OBJECT_SPRUCE; k<= OBJECT_BUSH; k++ )
    {
    object_count[ k ]= count[ k - OBJECT_SPRUCE ];
    static_objects[k] = new StaticObject[ object_count[k] ];
    for( unsigned int i= 0; i< object_count[ k ]; i++ )
    {
        int x, y;
        x= 16 + 992 * rand() / RAND_MAX;
        y= 16 + 992 * rand() / RAND_MAX;

        if( landscape.CanPlaceObject( x-4, y-4, x+4, y+4 ) && ( t_data[ 4*(1024-x + (1024-y) * 1024) ] > 128  || k != OBJECT_SPRUCE ) )
        {
            landscape.PlaceObject( x-4, y-4, x+4, y+4, &static_objects[k][i] );

            static_objects[k][i].pos[0]= float(x)+0.5f;
            static_objects[k][i].pos[1]= float(y)+0.5f;

            static_objects[k][i].pos[2]= landscape.Height( static_objects[k][i].pos[0],
            static_objects[k][i].pos[1] ) - 0.2f;

            landscape.SetTexture( x, y, TEXTURE_DIRT, 4 );

            static_objects[k][i].angle= randf(MW_2PI);//MW_PI * 2.0f * float(rand())/float(RAND_MAX);

            static_objects[k][i].scale= randf( 1.3f, 0.6f );//0.7f + 0.5f*float(rand())/float(RAND_MAX);//scale
            static_objects[k][i].height= SPRUCE_HEIGHT;
            static_objects[k][i].radius= SPRUCE_RADIUS;

            static_objects[k][i].object_id= OBJECT_SPRUCE;
        }
        else
            i--;
    }

    }//k
}

void Level::PlaceAchtungTables()
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

void Level::PlaceHealthPacks()
{
    static unsigned int count[]= { 128, 64, 32 };
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
                landscape.SetMaximumHeight( x, y, x+1, y+1 );

            
                static_objects[k][i].pos[0]= float(x)+0.5f;
                static_objects[k][i].pos[1]= float(y)+0.5f;
                static_objects[k][i].pos[2]= landscape.Height( float(x)+0.5f, float(y)+0.5f ) + 1.0f;
                static_objects[k][i].scale= 1.0f;
                static_objects[k][i].height= 0.5f;
                static_objects[k][i].radius= -1.0f;// negative radius - no collision
                static_objects[k][i].object_id= k;
                static_objects[k][i].angle= randf( MW_2PI );//MW_2PI * float(rand()) /float(RAND_MAX);
             }
             else
                 i--;
         }//i
    }//k


}

void Level::PlaceAmmo()
{
    static unsigned int count[]= { 16, 44, 34, 30 };
    for( unsigned int k= OBJECT_BULLETS_AMMO_BOX; k<= OBJECT_GAUSS_BOLTS_AMMO_BOX; k++ )
    {
        object_count[k]= count[k-OBJECT_BULLETS_AMMO_BOX];
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
                landscape.SetMaximumHeight( x, y, x+1, y+1 );

            
                static_objects[k][i].pos[0]= float(x)+0.5f;
                static_objects[k][i].pos[1]= float(y)+0.5f;
                static_objects[k][i].pos[2]= landscape.Height( float(x)+0.5f, float(y)+0.5f ) + 1.0f;
                static_objects[k][i].scale= 1.0f;
                static_objects[k][i].height= 0.5f;
                static_objects[k][i].radius= -1.0f;// negative radius - no collision
                static_objects[k][i].object_id= k;
                static_objects[k][i].angle= randf( MW_2PI );//MW_2PI * float(rand()) /float(RAND_MAX);
             }
             else
                 i--;
         }//i
    }//k

}

void Level::PlaceWeapons()
{
    static unsigned int count[]= { 1, 16, 24, 20, 18 };
    for( unsigned int k= OBJECT_SHOVEL; k<= OBJECT_GAUSS_GUN; k++ )
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
                landscape.SetMaximumHeight( x-1, y-1, x+2, y+2 );

            
                static_objects[k][i].pos[0]= float(x)+0.5f;
                static_objects[k][i].pos[1]= float(y)+0.5f;
                static_objects[k][i].pos[2]= landscape.Height( float(x)+0.5f, float(y)+0.5f ) + 1.0f;
                static_objects[k][i].scale= 1.0f;
                static_objects[k][i].height= 0.5f;
                static_objects[k][i].radius= -1.0f;// negative radius - no collision
                static_objects[k][i].object_id= k;
                static_objects[k][i].angle= randf(MW_2PI);//MW_2PI * float(rand()) /float(RAND_MAX);
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
    PlaceAmmo();

    bullet_count= 0;

	wave= 0;
	for( unsigned int i= 0; i< MONSTER_COUNT; i++ )
		wave_monsters_left[i]= attack_waves[0].monster_count[i];


}

void Level::NextWave()
{
	wave= (wave+1)%ATTACK_WAVES;
	for( unsigned int i= 0; i< MONSTER_COUNT; i++ )
		wave_monsters_left[i]= attack_waves[wave].monster_count[i];
}


void Level::SpawnMonsters()
{
	if( monster_count < attack_waves[wave].max_monsters )
	if(  MainLoop::FrameDT() > randf( attack_waves[wave].respawn_factor ) )
	{
		unsigned int chance[3];
		unsigned int total_monsters_left= 0;
		chance[0]= wave_monsters_left[0];
		total_monsters_left= wave_monsters_left[0];
		for( unsigned int i= 1; i< MONSTER_COUNT; i++ )
		{
			total_monsters_left+= wave_monsters_left[i];
			chance[i]= chance[i-1] + wave_monsters_left[i];
		}

		if( total_monsters_left == 0 )
		{
			if( monster_count == 0 )
				NextWave();
			return;
		}
		unsigned int r= total_monsters_left * rand()/RAND_MAX;
		unsigned int monster_id= 0;
		for( unsigned int i= 0; i< MONSTER_COUNT; i++ )
			if( r < chance[i] )
			{
				monster_id= i;
				break;
			}

		float pos[]= { player->Position()[0], player->Position()[1], 0.0f };
        pos[0]+= randf( -64.0f, 64.0f );
        pos[1]+= randf( -64.0f, 64.0f );
        pos[2]= 3.0f + landscape.Height( pos[0], pos[1] );
		if( monster_count < MAX_LEVEL_MONSTERS )
		{
			wave_monsters_left[ monster_id ]--;
			monsters[ monster_count ]= new Monster( player, this, monster_id, pos );
			monster_count++;
#ifdef MW_DEBUG
			printf( "spawn mosnter %d\n", monster_id );
			printf( "left: " );
			for( unsigned int i= 0; i< MONSTER_COUNT; i++ )
				printf( "%d ", wave_monsters_left[i] );
			printf( "\n" );
#endif
		}
	}
}

void Level::RestartLevel()
{
    for( unsigned int i= OBJECT_SMALL_HEALTHPACK; i<=OBJECT_GAUSS_GUN; i++ )
    {
        object_count[i]= 0;
        delete[] static_objects[i];
    }
    landscape.RegenHeightmap();
    PlaceHealthPacks();
    PlaceWeapons();
    PlaceAmmo();
}

void Level::AddBullet( const float* pos, const float* vel, unsigned char bullet_type  )
{
    if( bullet_count >= MAX_BULLETS )
        return;

    for( unsigned int i= 0; i< 3; i++ )
    {
        bullets[ bullet_count ].pos[i]= pos[i];
        bullets[ bullet_count ].vel[i]= vel[i];
    }
    bullets[ bullet_count ].bullet_type= bullet_type;
    if( bullet_type == BULLET_SNOW_BALL )
         bullets[ bullet_count ].gravity= true;
    else
        bullets[ bullet_count ].gravity= false;

    bullet_count++;
}

unsigned int Level::RemoveBullet( unsigned int i )
{
    if( i < bullet_count -1 )//remove bullet
        bullets[i]= bullets[ bullet_count -1 ];
    bullet_count--;
    i--;
    return i;
}

void Level::RocketBlust( Bullet* b )
{
#define ROCKET_DAMAGE 40.0f
    float l, dmg;
    for( unsigned int i= 0; i< monster_count; i++ )
    {
       l= Distance( b->pos, monsters[i]->Position() )+1.0f;
       dmg= ROCKET_DAMAGE / (l*l);
        if( dmg > 2.0f )
            monsters[i]->Damage( int(dmg) );
    }
    l= Distance( b->pos, player->Position() ) + 1.0f;
    dmg= ROCKET_DAMAGE / (l*l);
    if( dmg > 2.0f )
        player->Damage( int(dmg) );

    particle_manager.AddBlust( b->pos );
    sound_system->PlaySound( b->pos, 1.0f, SOUND_BLAST );

}

void Level::BulletsTick()
{
    float dt= MainLoop::FrameDT();
    float tmp_pos[3];
    float tmp_vec[3];
    for( unsigned int i= 0; i< bullet_count; i++ )
    {
        if( bullets[i].gravity )
        {
            bullets[i].vel[2]+= GRAVITY * dt;
            bullets[i].pos[2]+= GRAVITY * dt * dt * 0.5f;
        }

        float vel[3]= { randf( -0.5f, 0.5f ), randf( -0.5f, 0.5f ), randf( -0.5f, 0.5f ) };
        if( bullets[i].bullet_type == BULLET_ROCKET  )
        {
            if( rand()&1 )
                particle_manager.AddParticle( bullets[i].pos, vel, PARTICLE_WEAPON_SMOKE, 1.5f );
            else
                particle_manager.AddParticle( bullets[i].pos, vel, PARTICLE_BLUST, 0.0625f );

        }
       /* else if( bullets[i].bullet_type == BULLET_FIRE_BALL )
        {
            if( rand()&1 )
            {
                float vel[3]= { randf( -1.5f, 1.5f ), randf( -1.5f, 1.5f ), randf( 0.5f, 2.5f ) };
               // float pos[3]= { randf( -0.2f, 0.2f ), randf( -0.2f, 0.2f ), randf( -0.2f, 0.2f ) };
               // Vec3Add( pos, bullets[i].pos );
               // Vec3Add( vel, bullets[i].vel );
                particle_manager.AddParticle( bullets[i].pos, vel, PARTICLE_BLUST, 0.0625f  );
            }
        }*/
        for( unsigned int j= 0; j< 3; j++ )
            bullets[i].pos[j]+= bullets[i].vel[j] * dt;
    }

    for( unsigned int i= 0; i< bullet_count; i++ )
    {
        tmp_vec[0]= bullets[i].vel[0];
        tmp_vec[1]= bullets[i].vel[1];
        tmp_vec[2]= bullets[i].vel[2];
        Vec3Normalize(tmp_vec);
        float l= Vec3Len( bullets[i].vel ) * dt;
        for( unsigned int j= 0; j< monster_count; j++ )
        {
            if( monsters[j]->IsCollision( bullets[i].pos, tmp_vec, tmp_pos ) )
            {
                if( Distance( bullets[i].pos, tmp_pos ) <=  l )
                {
                    if( bullets[i].bullet_type == BULLET_ROCKET )
                        RocketBlust( bullets + i );
                    else if( bullets[i].bullet_type == BULLET_SNOW_BALL )
                        particle_manager.AddSnowSplatter( bullets[i].pos );

                    monsters[j]->Damage( bullet_damage_table[ bullets[i].bullet_type ] );
                        float blood_vec[]= { bullets[i].vel[0], bullets[i].vel[1], bullets[i].vel[2] };
                          Vec3Normalize( blood_vec );
                    Vec3Mul( blood_vec, -1.0f );
                    particle_manager.AddBloodSplatter( tmp_pos, blood_vec );

                    i= RemoveBullet(i);
                }
            }//is collision
        }//j

        if( FindCollisionWithCylinder( bullets[i].pos, tmp_vec, player->Position(), PLAYER_RADIUS, PLAYER_HEIGHT, tmp_pos ) )
        {
            if( Distance( bullets[i].pos, tmp_pos ) <=  l )
            {
                if( bullets[i].bullet_type == BULLET_ROCKET )
                       RocketBlust( bullets + i );
                else if( bullets[i].bullet_type == BULLET_SNOW_BALL )
                        particle_manager.AddSnowSplatter( bullets[i].pos );

                player->Damage( bullet_damage_table[ bullets[i].bullet_type ] );
  
                i= RemoveBullet(i);
            }
        }
    }//i

    for( unsigned int i= 0; i< bullet_count; i++ )
    {
        if( Distance( bullets[i].pos, player->Position() ) > 512.0f ||
            landscape.Height( bullets[i].pos[0], bullets[i].pos[1] ) >= bullets[i].pos[2] )
        {
            if( bullets[i].bullet_type == BULLET_ROCKET )
                RocketBlust( bullets + i );
            else if( bullets[i].bullet_type == BULLET_SNOW_BALL )
                        particle_manager.AddSnowSplatter( bullets[i].pos );

             i= RemoveBullet(i);
        }

    }
}


void Level::RemoveObject( StaticObject* obj )
{
    unsigned int k= obj->object_id;
    unsigned int i= obj - static_objects[k];

    if( i < object_count[k] - 1 )
    {
		StaticObject* replaced_object= &static_objects[k][ object_count[k] - 1];
		unsigned int x= (unsigned int)replaced_object->pos[0];
		unsigned int y= (unsigned int)replaced_object->pos[1];
		landscape.PlaceObject( x, y, x, y, &static_objects[k][i] );// now, landscape have valid pointer to object
        static_objects[k][i]= *replaced_object;
    }
    object_count[k]--;
}

void Level::SetPlayer( Player* player )
{
    this->player= player;
       float pos[]= {512.0f, 512.0f, 0.0f };
    player->SetPosition( pos );

    monster_count= 0;
  /*  pos[1]= 540.0f;
    pos[2]= 96.0f;
    monsters[0]= new Monster( player, this, MONSTER_SNOWMAN, pos );

    for( unsigned int i= 0; i< 20; i++ )
    {
        pos[0]= randf( 400.0f, 600.0f );
        pos[1]= randf( 400.0f, 600.0f );
        pos[2]= landscape.Height( pos[0], pos[1] );
        monsters[i]= new Monster( player, this, rand()%MONSTER_COUNT, pos );
    }monster_count= 20;*/
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
        collide_pos[0]+= vec[0]*0.5f;
        collide_pos[1]+= vec[1]*0.5f;
        collide_pos[2]+= vec[2]*0.5f;
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

void Level::PlayerShot( const float* pos, const float* vec, unsigned char weapon_id )
{
     float landscape_collision_pos[3];
     float dst_2_landscape;
     float dst_2_nearest_monster= 1e4f;
     float monster_collision_pos[3];
     float tmp_pos[3];
     int monster_id= -1;

     sound_system->PlaySound( pos, 1.0f, weapon_descriptor_table[weapon_id].sound_id, true );
     if( weapon_descriptor_table[ weapon_id ].speed < 255 )
     {
         tmp_pos[0]= vec[0];
         tmp_pos[1]= vec[1];
         tmp_pos[2]= vec[2];
         Vec3Mul( tmp_pos, float(weapon_descriptor_table[ weapon_id ].speed) ); 
         AddBullet( pos, tmp_pos, weapon_descriptor_table[ weapon_id ].damage_bullet );
         return;
     }
    // player->Velocity( tmp_pos );
    // Vec3Add( tmp_pos, vec );
    // particle_manager.AddWeaponSmoke( pos, tmp_pos );

     if( FindCollisionWithLandscape( pos, vec, landscape_collision_pos ) )
         dst_2_landscape= Distance( landscape_collision_pos, pos );
     else
        dst_2_landscape= 1e4f;


     for( unsigned int i= 0; i< monster_count; i++ )
            if(  monsters[i]->IsCollision( pos, vec, tmp_pos ) )
            {
                float dst= Distance( tmp_pos, pos );
                
                if( dst<  dst_2_nearest_monster )
                {
                    monster_id= i;
                    monster_collision_pos[0]= tmp_pos[0];
                    monster_collision_pos[1]= tmp_pos[1];
                    monster_collision_pos[2]= tmp_pos[2];

                    dst_2_nearest_monster= dst;
                }
            }

	if( monster_id != -1 && dst_2_nearest_monster < dst_2_landscape )
	{
		if( dst_2_nearest_monster < float(weapon_descriptor_table[ weapon_id ].max_distance) )
		{
			monsters[ monster_id ]->Damage( weapon_descriptor_table[weapon_id].damage_bullet );
			tmp_pos[0]=- vec[0];
			tmp_pos[1]=- vec[1];
            tmp_pos[2]=- vec[2];
            particle_manager.AddBloodSplatter( monster_collision_pos, tmp_pos );
             }
        }
    else if( dst_2_landscape < 1e4f )
	{
		landscape_collision_pos[2]+= 0.2f;
		if( dst_2_landscape < float(weapon_descriptor_table[ weapon_id ].max_distance ) )
			particle_manager.AddDirtSplatter( landscape_collision_pos );
	}

    if( weapon_id == WEAPON_GAUSS_GUN  )
    {
       // tmp_pos[0]= pos[0];tmp_pos[1]= pos[1]; tmp_pos[2]= pos[2]- 0.25f;
        if( dst_2_landscape >= 1e4f )
        {
            landscape_collision_pos[0]= pos[0];
            landscape_collision_pos[1]= pos[1];
            landscape_collision_pos[2]= pos[2]-0.2f;
            tmp_pos[0]= vec[0]; tmp_pos[1]= vec[1]; tmp_pos[2]= vec[2];
            Vec3Mul( tmp_pos, 128.0f );
            Vec3Add( landscape_collision_pos, tmp_pos );
			if( dst_2_nearest_monster < 1e4f )
				dst_2_landscape= dst_2_nearest_monster+100.0f;
        }
        tmp_pos[0]= pos[0]; tmp_pos[1]= pos[1]; tmp_pos[2]= pos[2] - 0.2f;
        particle_manager.AddGaussShot( tmp_pos, 
            dst_2_nearest_monster < dst_2_landscape ? monster_collision_pos : landscape_collision_pos );
    }

}

void Level::MonsterShot( const float* pos, const float* vec, unsigned char monster_id )
{
    float landscape_collision_pos[3];
    float dst_2_landscape;
    
    float player_collision_pos[3];

    if( monster_id == MONSTER_SNOWMAN )
    {
        float vel_vec[]= { vec[0], vec[1], vec[2] };
        Vec3Mul( vel_vec, 25.0f );
        AddBullet( pos, vel_vec, BULLET_SNOW_BALL );
        return;
    }
    else if( monster_id == MONSTER_ROBOT )
    {
        float tmp_pos[3];
        tmp_pos[0]= vec[0];
        tmp_pos[1]= vec[1];
        tmp_pos[2]= vec[2];
        Vec3Mul( tmp_pos, float(weapon_descriptor_table[ WEAPON_ROCKET_LAUNCHER ].speed) ); 
        AddBullet( pos, tmp_pos, weapon_descriptor_table[ WEAPON_ROCKET_LAUNCHER ].damage_bullet );
        sound_system->PlaySound( pos, 1.0f, SOUND_ROCKET_SHOT );
        return;
     }
    else if( monster_id == MONSTER_CACODEMON )
    {
        float vel_vec[]= { vec[0], vec[1], vec[2] };
        Vec3Mul( vel_vec, 20.0f );
        AddBullet( pos, vel_vec, BULLET_FIRE_BALL );
        return;

    }

    sound_system->PlaySound( pos, 1.0f, 1 );

    if( FindCollisionWithLandscape( pos, vec, landscape_collision_pos ) )
         dst_2_landscape= Distance( landscape_collision_pos, pos );
     else
        dst_2_landscape= 1e6f;

     if( FindCollisionWithCylinder( pos, vec, player->Position(), PLAYER_RADIUS, PLAYER_HEIGHT, player_collision_pos ) )
         {
            if( Distance( player_collision_pos, pos ) < dst_2_landscape )
            {
                player->Damage( monster_descriptor_table[ monster_id ].remote_damage );
                //particle_manager.AddParticle( player_collision_pos, NULL, 0, 1.0f );
                float tmp_vec[]= { -vec[0], -vec[1], -vec[2] };
                particle_manager.AddBloodSplatter( player_collision_pos, tmp_vec );
            }
            else
                goto add_landscape_particle;
         }
     else
         goto add_landscape_particle;
         

     return;

add_landscape_particle:
  particle_manager.AddDirtSplatter( landscape_collision_pos );
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
                    if( obj->object_id >= OBJECT_SMALL_HEALTHPACK &&  obj->object_id <= OBJECT_GAUSS_GUN )
                    {
                        float h= obj->pos[2];
                        h-= player->Position()[2];
                        if( h * h < 2.0f )
                        {
                            if( obj->object_id<= OBJECT_LARGE_HEALTHPACK )
                            {
                                if( player->Health() < PLAYER_MAX_HEALTH )
                                {
                                    int h= healthpack_table[ obj->object_id - OBJECT_SMALL_HEALTHPACK ];
                                    player->AddHealth( h ); 
                                }
                                else
                                    continue;
                            }
                            else if( obj->object_id<= OBJECT_GAUSS_BOLTS_AMMO_BOX )
                            {
                                unsigned char ammo_id= obj->object_id - OBJECT_BULLETS_AMMO_BOX + 1;
                                player->GiveAmmo( ammo_id, ammo_pack_size_table[ammo_id] );
                            }
                            else if( obj->object_id <= OBJECT_GAUSS_GUN )
                            {
                                player->GiveWeapon( obj->object_id - OBJECT_SHOVEL );
                            }
                            landscape.PlaceObject( i,j, i,j, NULL );
                            RemoveObject( obj );
                        }//h
                    }// if can pickup
            }//ij
    
}

void  Level::KillMonster( Monster* m )
{
	if( m->MonsterId() == MONSTER_SNOWMAN )
	{
		float pos[]= { m->Position()[0], m->Position()[1], m->Position()[2] };
		for( unsigned int i= 0; i< 3; i++ )
		{
			particle_manager.AddSnowSplatter( pos );
			pos[2]+= 1.0f;
		}
	}
	else if( m->MonsterId() == MONSTER_ROBOT )
	{
		float pos[]= { m->Position()[0], m->Position()[1], m->Position()[2]+1.0f };
		particle_manager.AddBlust( pos );
	}
}

void Level::PhysTick()
{
    if( player->Health() <= 0 )
        if( MainLoop::CurrentTime() - player->DeathTime()  > CLOCKS_PER_SEC * 8 )
        {
           // RestartLevel();
            player->Spawn();
            float pos[3]= { 512.0f, 512.0f, landscape.Height( 512.0f, 512.0f ) };
            player->SetPosition( pos );
        }
    
    static unsigned int prev_time= 0;
    float dt= MainLoop::FrameDT();//float( MainLoop::CurrentTime() - prev_time ) / float( CLOCKS_PER_SEC );
    prev_time= MainLoop::CurrentTime();


    BulletsTick();
 
    for( unsigned int i= 0; i< monster_count; )
    {
        if( monsters[i]->Helath() <= 0 )
        {
			KillMonster( monsters[i] );
            player->AddPoints( monster_descriptor_table[ monsters[i]->MonsterId() ].cost );
			delete monsters[i];
			
			if( i < monster_count - 1 )
				monsters[i]= monsters[ monster_count - 1 ]; 
			monster_count--;
        }
        else
		{
             monsters[i]->Tick();
			 i++;
		}
    }

	SpawnMonsters();

    CollideMonsters();


   
    /*if( monster_count == 0 )
    {
        float pos[]= { 512, 512, 78 };
        monsters[0]= new Monster( player, this, 0, pos);
        pos[2]+= 6.0f;
        monsters[1]= new Monster( player, this, 0, pos);
        monster_count=2;
    }*/

    player->Tick();
     CollidePlayer();
    
    float v[3];
    v[0]= player->Position()[0] - 512.0f;
    v[1]= player->Position()[1] - 512.0f;
    v[2]= 0;
	if( Vec3Len(v) > COMBAT_ZONE_RADIUS + 2.0f && player->Health() > 0 )
    {
       // v[0]= 512.0f;
        //v[1]= 512.0f;
        player->Damage( PLAYER_MAX_HEALTH );
		particle_manager.AddBlust( player->Position() );
		sound_system->PlaySound( player->Position(), 1.0f, SOUND_BLAST );
        //player->SetPosition(v);
    }
 
    PickUp();

    for( unsigned int k= OBJECT_SMALL_HEALTHPACK; k<= OBJECT_GAUSS_GUN; k++ )
        for( unsigned int i= 0; i< object_count[k]; i++ )
        {
            static_objects[k][i].angle+= MW_PI2 * dt;
            if( static_objects[k][i].angle >= MW_2PI )
                static_objects[k][i].angle-= MW_2PI;
        }
 

	particle_manager.Tick();
}

void Level::CollideMonsters()
{
    float pos[2][3];
    float h[2];
    float z_min[2], z_max[2];
    float r[2];
    MonsterDescriptor* md;
    float vec_2_next_monster[3];
	if( monster_count == 0 )
		return;
    for( unsigned int i= 0, i_end= monster_count - 1; i< i_end; i++ )
    {
        pos[0][0]= monsters[i]->Position()[0];
        pos[0][1]= monsters[i]->Position()[1];
        pos[0][2]= monsters[i]->Position()[2];
        md= monster_descriptor_table + monsters[i]->MonsterId();
        if( MONSTER_IS_SPHERICAL( md->flags ) )
        {
            pos[0][2]-= md->radius;
            h[0]= md->radius * 2.0f;
        }
        else
            h[0]= md->heigth;
        
        z_min[0]= pos[0][2];
        z_max[0]= pos[0][2] + h[0];
        r[0]= md->radius;

        pos[0][2]= 0.0f;
        for( unsigned int j= i+1; j< monster_count; j++ )
        {
            pos[1][0]= monsters[j]->Position()[0];
            pos[1][1]= monsters[j]->Position()[1];
            pos[1][2]= monsters[j]->Position()[2];
            md= monster_descriptor_table + monsters[j]->MonsterId();
            if( MONSTER_IS_SPHERICAL( md->flags ) )
            {
                pos[1][2]-= md->radius;
                h[1]= md->radius * 2.0f;
            }
            else
                h[1]= md->heigth;
        
            r[1]= md->radius;
            z_min[1]= pos[1][2];
            z_max[1]= pos[1][2] + h[1];

            pos[1][2]= 0.0f;
            if( Distance( pos[0], pos[1] ) < ( r[0]+r[1] ) )
            {
                if( !( z_min[0] > z_max[1] || z_min[1] > z_max[0] ) )
                {
                    vec_2_next_monster[0]= monsters[j]->Position()[0];
                    vec_2_next_monster[1]= monsters[j]->Position()[1];
                    Vec3Sub( vec_2_next_monster, monsters[i]->Position() );
                    vec_2_next_monster[2]= 0.0f;
                    Vec3Normalize( vec_2_next_monster );
                    Vec3Mul( vec_2_next_monster, r[0]+r[1] );

                    pos[1][0]= monsters[i]->Position()[0];
                    pos[1][1]= monsters[i]->Position()[1];
                    pos[1][2]= monsters[j/*<---*/]->Position()[2];
                    Vec3Add( pos[1], vec_2_next_monster );
                    monsters[j]->SetPosition( pos[1] );

                }
            }//distance
        }//j
    }//i
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

    for( unsigned int i= 0; i< monster_count; i++ )
    {
        unsigned char id= monsters[i]->MonsterId() ;
        r_min= monster_descriptor_table [id ].radius;

        vec[0]= player->Position()[0] - monsters[i]->Position()[0];
        vec[1]= player->Position()[1] - monsters[i]->Position()[1];

        min[1]= monsters[i]->Position()[2];
        if( MONSTER_IS_SPHERICAL( monster_descriptor_table[id].flags ) )
        {
            min[1]-= monster_descriptor_table[id].radius;
            max[1]= min[1] +  monster_descriptor_table[id].radius * 2.0f;
        }
        else
            max[1]= min[1] + monster_descriptor_table[id].heigth;
        if(! ( max[0] < min[1] || max[1] < min[0]) )
        {
            r= sqrt( vec[0] * vec[0] + vec[1] * vec[1] );
            if( r < r_min )
            {
          
                pos[2]= player->Position()[2];
                pos[0]= monsters[i]->Position()[0] + vec[0] * r_min / r;
                pos[1]= monsters[i]->Position()[1]  + vec[1] * r_min / r;
                player->SetPosition(pos);
          
            }
        }
  

    }
}