#include "player.h"
#include "math.h"
#include "game_constants.h"
#include "time.h"
#include "main_loop.h"

Player::Player(Level * lev) :
        level(lev)

{
   Spawn();
}

void Player::Spawn()
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
    current_weapon= WEAPON_PISTOL;
    last_shot_time= 0;

    for( int i= 0; i< AMMO_COUNT; i++ )
        ammo[i]= 0;

    weapon_animation_is_active= false;

    weapons[ WEAPON_PISTOL ]= true;
    ammo[ AMMO_BULLETS ]= 20;

    blood_factor= 0.0f;
    score= 0;
}

void Player::Damage( int dmg )
{
    if( health <= 0 )
        return;
    health-= dmg;
    if( dmg > 0 )
    {
        blood_factor+= float(dmg)*0.03f;
        if( blood_factor > 1.0f )blood_factor= 1.0f;
    }
    if( health <= 0 )
        death_time= MainLoop::CurrentTime();
}

void Player::AddHealth( int h )
{
    health+= h;
    if( health > PLAYER_MAX_HEALTH )
        health= PLAYER_MAX_HEALTH;
}

void Player::Shot()
{
    if( health <= 0 )
        return;

   unsigned int current_time= clock();
   float dt= float(current_time - last_shot_time)/float(CLOCKS_PER_SEC);
   if( dt > float(weapon_descriptor_table[ current_weapon ].delay/64.0f))
   {
       last_shot_time= current_time;
       unsigned char ammo_type= weapon_descriptor_table[ current_weapon ].ammo_id;
       if( ammo[ammo_type] >0  || ammo_type == AMMO_NOAMMO )
        {
            in_attack= true;
            alrady_shot= false;
            weapon_animation_is_active= true;
            weapon_animation_time= 0.0f;
       }
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
    if( health <= 0 )
        return;

    float l;
    if( on_surface )
    {
        l= sqrt( vel[0] * vel[0] + vel[1] * vel[1] );
        if( l <  MAX_GROUND_SPEED )
        {
            float n[3];
            float vec_n[3]= { vec[0], vec[1], vec[2] };
            level->GetLandscape()->Normal( pos[0], pos[1], n );
            float a= Vec3Dot( n, vec );
            Vec3Mul( n, a );
            Vec3Sub( vec_n, n );
            
            vel[0]+= vec_n[0] * ( GROUND_ACCELERATION + GROUND_FRICTION );
            vel[1]+= vec_n[1] * ( GROUND_ACCELERATION + GROUND_FRICTION );
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
    vel[2]+= vec[2]* (AIR_ACCELERATION+AIR_FRICTION);

}

void Player::Jump()
{
    if( health <= 0 )
        return;
    float dh= pos[2] - level->GetLandscape()->Height( pos[0], pos[1] );
    if( dh * dh < 0.01f )
    {
        vel[2]+= JUMP_DELTA_V;
        pos[2]+= 0.05f;
    }
    else if( pos[2] < level->GetLandscape()->WaterLevel() )
    {
        float a[]= { 0.0f, 0.0f, MainLoop::FrameDT() };
        Accelerate(a);
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


void Player::DeathTick()
{
    //rotate camera ang move up - soul effect
    float dt= MainLoop::FrameDT();
    pos[2]+= 1.5f * dt;
    ang_xy+= MW_2PI * (-0.0625f) * dt;
    ang_z= ang_z * 0.998f -MW_PI2 * 0.002f;
}

void Player::Tick()
{
    if( health <= 0 )
    {
        DeathTick();
        return;
    }

    float dt= MainLoop::FrameDT();

    Move(dt);
    weapon_animation_time+= dt;
    if( in_attack )
    {
        unsigned int current_time= clock();
        float s_dt= float( MainLoop::CurrentTime() - last_shot_time ) / float(CLOCKS_PER_SEC);
        float shot_end_time= weapon_descriptor_table[ current_weapon ].delay_before_shot + 
                    weapon_descriptor_table[ current_weapon ].delay_after_shot;
 
        if( s_dt >= weapon_descriptor_table[ current_weapon ].delay_before_shot )
        {
            if( ! alrady_shot )
            {
                alrady_shot= true;
                
                float p[]= { pos[0], pos[1], pos[2] + PLAYER_HEIGHT };
                float v[3];
                float da= weapon_descriptor_table[current_weapon].dispertion;
                float da_xy= randf( da, -da ), da_z= randf( da, -da );
                v[2]= sin( ang_z + da_z );
                v[0]= v[1]= cos( ang_z + da_z );
                v[1]*= cos( ang_xy + da_xy );
                v[0]*= -sin( ang_xy + da_xy );

                unsigned char ammo_type= weapon_descriptor_table[ current_weapon ].ammo_id;
                //if( ammo[ammo_type] >0  || ammo_type == AMMO_NOAMMO )
                //{
                    if( ammo_type != AMMO_NOAMMO )
                    ammo[ammo_type]--;
                    level->PlayerShot( p, v, current_weapon );
                //}
            }
        }// if time to shot
        if( s_dt > shot_end_time )
        {
            in_attack= false;
            weapon_animation_is_active= false;
        }
    }//in shot


    blood_factor*= 0.98f;
}


void Player::Move( float dt )
{
    if( pos[2] > level->GetLandscape()->WaterLevel() )
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
    if( health <= 0 )
        return;
    ang_xy+= a;
    if( ang_xy > MW_PI )
        ang_xy-= 2.0f * MW_PI;
    else if( ang_xy < -MW_PI )
        ang_xy+= 2.0f * MW_PI;
}
 
void Player::RotateZ( float a )
{
    if( health <= 0 )
        return;
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
