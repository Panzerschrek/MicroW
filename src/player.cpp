#include "player.h"
#include "math.h"
#include "game_constants.h"
#include "time.h"

Player::Player(Level * lev) :
        level(lev)

{
    health= 100;

    ang_xy= 0.0f;
    ang_z= 0.0f;
    vel[0]= 0.0f;
    vel[1]= 0.0f;
    vel[2]= 0.0f;
    for( int i= 0; i< sizeof(weapons); i++ )
        weapons[i]= false;
    weapons[ WEAPON_SHOVEL ]= true;
    current_weapon= WEAPON_SHOVEL;
    last_shot_time= 0;

    for( int i= 0; i< AMMO_COUNT; i++ )
        ammo[i]= 0;

    weapon_animation_is_active= false;
}

void Player::Shot()
{
   unsigned int current_time= clock();
   float dt= float(current_time - last_shot_time)/float(CLOCKS_PER_SEC);
   if( dt > float(weapon_descriptor_table[ current_weapon ].delay/64.0f))
   {
       last_shot_time= current_time;
       /*float p[]= { pos[0], pos[1], pos[2] + PLAYER_HEIGHT };
       float v[3];
       v[2]= sin( ang_z );
       v[0]= v[1]= cos( ang_z );
       v[1]*= cos( ang_xy );
       v[0]*= -sin( ang_xy );

       unsigned char ammo_type= weapon_descriptor_table[ current_weapon ].ammo_id;
       if( ammo[ammo_type] >0  || ammo_type == AMMO_NOAMMO )
       {
           if( ammo_type != AMMO_NOAMMO )
               ammo[ammo_type]--;
           level->Shot( p, v, current_weapon );
          // printf( "shot: %f\n", dt * 1000.0f );
       }*/
       in_attack= true;
       alrady_shot= false;
       weapon_animation_is_active= true;
       weapon_animation_time= 0.0f;
   }
}

void Player:: NextWeapon()
{
    do
    {
        current_weapon++;
        current_weapon%= WEAPON_COUNT;
    }
    while( !weapons[ current_weapon ] );

}

void Player::PreviousWeapon()
{
    do
    {
        current_weapon--;
        if( current_weapon < 0 )
            current_weapon= WEAPON_COUNT-1;
    }
    while( !weapons[ current_weapon ] );
}


void Player::Accelerate( const float* vec )
{
    float l;
    if( on_surface )
    {
        l= sqrt( vel[0] * vel[0] + vel[1] * vel[1] );
        if( l <  MAX_GROUND_SPEED )
        {
            vel[0]+= vec[0] * ( GROUND_ACCELERATION + GROUND_FRICTION );
            vel[1]+= vec[1] * ( GROUND_ACCELERATION + GROUND_FRICTION );
        }  
    }
    else
    {
        vel[0]+= vec[0] * ( AIR_ACCELERATION + AIR_FRICTION );
        vel[1]+= vec[1] * ( AIR_ACCELERATION + AIR_FRICTION );
        l= sqrt( vel[0] * vel[0] + vel[1] * vel[1] );
        if( l <  MAX_AIR_SPEED )
        {
            vel[0]+= vec[0] * ( AIR_ACCELERATION + AIR_FRICTION );
            vel[1]+= vec[1] * ( AIR_ACCELERATION + AIR_FRICTION );
        }  

    }

}

void Player::Jump()
{
    if( on_surface )
    {
        vel[2]+= JUMP_DELTA_V;
        pos[2]+= 0.05f;
    }
}

void Player::Velocity( float* v )
{
    v[0]= vel[0];
    v[1]= vel[1];
    v[2]= vel[2];
}

void Player::SetVelocity( float* v )
{
    vel[0]= v[0];
    vel[1]= v[1];
    vel[2]= v[2];
}


void Player::Tick( float dt )
{
    Move(dt);
    weapon_animation_time+= dt;
    if( in_attack )
    {
        unsigned int current_time= clock();
        float s_dt= float( clock() - last_shot_time ) / float(CLOCKS_PER_SEC);
        float shot_end_time= weapon_descriptor_table[ current_weapon ].delay_before_shot + 
                    weapon_descriptor_table[ current_weapon ].delay_after_shot;
 
        if( s_dt >= weapon_descriptor_table[ current_weapon ].delay_before_shot )
        {
            if( ! alrady_shot )
            {
                alrady_shot= true;
                
                float p[]= { pos[0], pos[1], pos[2] + PLAYER_HEIGHT };
                float v[3];
                v[2]= sin( ang_z );
                v[0]= v[1]= cos( ang_z );
                v[1]*= cos( ang_xy );
                v[0]*= -sin( ang_xy );

                unsigned char ammo_type= weapon_descriptor_table[ current_weapon ].ammo_id;
                if( ammo[ammo_type] >0  || ammo_type == AMMO_NOAMMO )
                {
                    if( ammo_type != AMMO_NOAMMO )
                    ammo[ammo_type]--;
                    level->Shot( p, v, current_weapon );
                }
            }
        }// if time to shot
        if( s_dt > shot_end_time )
        {
            in_attack= false;
            weapon_animation_is_active= false;
        }
    }//in shot
}


void Player::Move( float dt )
{
     vel[2]+= GRAVITY * dt;
    if( fabs( vel[2] ) > MAX_VERTICAL_SPEED )
        vel[2]*= MAX_VERTICAL_SPEED * fabs(vel[2]);



    float l= sqrt( vel[0] * vel[0] + vel[1] * vel[1] );
    float vel_noralized[]= { vel[0], vel[1], vel[2] };
    Vec3Normalize( vel_noralized );
    if( on_surface )
    {
        if( l > dt * GROUND_FRICTION )
        {
           vel[0]-= dt * GROUND_FRICTION * vel_noralized[0];
           vel[1]-= dt * GROUND_FRICTION * vel_noralized[1];
        }
        else
            vel[0]= vel[1]= 0.0f;

    }
    else
    {
        if( l > dt * AIR_FRICTION  )
        {
           vel[0]-= dt * AIR_FRICTION * vel_noralized[0];
           vel[1]-= dt * AIR_FRICTION * vel_noralized[1];
        }
        else
            vel[0]= vel[1]= 0.0f;
    }

    pos[0]+= vel[0]*dt;
    pos[1]+= vel[1]*dt;
    pos[2]+= vel[2]*dt;

    float h= level->GetLandscape()->Height( pos[0], pos[1] );
    if( pos[2] <= h )
    {
        on_surface= true;
        pos[2]= h;
        vel[2]= 0.0f;
    }
    else
        on_surface= false;


}

void Player::SetPosition( const float* new_pos )
{
    pos[0]= new_pos[0];
    pos[1]= new_pos[1];
    pos[2]= new_pos[2];
}

void Player::RotateXY( float a )
{
    ang_xy+= a;
    if( ang_xy > MW_PI )
        ang_xy-= 2.0f * MW_PI;
    else if( ang_xy < -MW_PI )
        ang_xy+= 2.0f * MW_PI;
}
 
void Player::RotateZ( float a )
{
    ang_z+= a;
    if( ang_z > MW_PI * 0.5f )
        ang_z= MW_PI * 0.5f;
    else if( ang_z < -MW_PI * 0.5f )
        ang_z= -MW_PI * 0.5f;
}


float Player::Velocity()
{
    float v[]= { vel[0], vel[1], 0.0f };
    return Vec3Len( v );
}
