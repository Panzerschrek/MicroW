#include "particle_manager.h"
#include "main_loop.h"
#include "math.h"

ParticleManager::ParticleManager()
{
    particles= new Particle[ MAX_PARTICLES ];
    particle_pos_buffer= new float[ 4 * MAX_PARTICLES ];
    particle_count= 0;
}



void ParticleManager::AddParticle( const float* pos, const float* vel, unsigned char particle_id, float life_time )
{
    if( particle_count == MAX_PARTICLES )
        return;

    for( unsigned int k= 0; k< 3; k++ )
    {
        particles[particle_count].pos[k]= pos[k];
        if( vel != NULL )
            particles[particle_count].vel[k]= vel[k];
        else 
            particles[particle_count].vel[k]= 0.0f;
    }

    particles[particle_count].particle_id= particle_id;
    particles[particle_count].death_time= MainLoop::CurrentTime() + int( life_time * float(CLOCKS_PER_SEC) );

    particle_count++;
}

void ParticleManager::AddBloodSplatter( const float* pos, const float* vec )
{
    float vel[3];
    float eps[3];
    vel[0]= vec[0] * 4.0f;
    vel[1]= vec[1] * 4.0f;
    vel[2]= vec[2] * 4.0f;

    for( int i= 0; i< 12; i++ )
    {
        for( unsigned int k= 0; k< 3; k++ )
            eps[k]= randf( -0.4f, 0.4f );
        Vec3Add( eps, vel );
        AddParticle( pos, eps, PARTICLE_BLOOD_SPLATTER, 0.5f );
    }
}

void ParticleManager::AddWeaponSmoke( const float* pos, const float* vec )
{
    float vel[3];
    float eps[3];
    float new_pos[3]={ pos[0], pos[1], pos[2] };
    Vec3Add( new_pos, vec );
    vel[0]= vec[0] * 4.0f;
    vel[1]= vec[1] * 4.0f;
    vel[2]= vec[2] * 4.0f;

    for( int i= 0; i< 12; i++ )
    {
        for( unsigned int k= 0; k< 3; k++ )
            eps[k]= randf( -0.4f, 0.4f );
        Vec3Add( eps, vel );
        AddParticle( new_pos, eps, PARTICLE_WEAPON_SMOKE, 0.5f );
    }
}

void ParticleManager::AddBlust( const float* pos )
{
    float vel[3];
    for( int i= 0; i< 48; i++ )
    {
        vel[0]= randf(-8.0f,8.0f);
        vel[1]= randf(-8.0f,8.0f);
        vel[2]= randf(-8.0f,8.0f);
        AddParticle( pos, vel, PARTICLE_WEAPON_SMOKE + (rand()&1), 0.3f + randf(0.3f) );

    }
}

void ParticleManager::AddSnowSplatter( const float* pos )
{
    float vel[3];
    for( int i= 0; i< 16; i++ )
    {
        vel[0]= randf(-4.0f,4.0f);
        vel[1]= randf(-4.0f,4.0f);
        vel[2]= randf(6.0f);
        AddParticle( pos, vel, PARTICLE_SNOW_SPLATTER, 0.4f + randf(0.3f) );

    }
}

void ParticleManager::AddGaussShot( const float* pos_begin, const float* pos_end )
{
    float vec[]= { pos_end[0], pos_end[1], pos_end[2] };
    Vec3Sub( vec, pos_begin );
    unsigned int n= int( Vec3Len( vec ) )/2;
    n= min( n, 128 );
    Vec3Normalize( vec );
    Vec3Mul( vec, 2.0f );

    float pos[]= { pos_begin[0], pos_begin[1], pos_begin[2] };
    float vel[3];
    for( unsigned int i= 0; i< n; i++ )
    {
        Vec3Add( pos, vec );
        vel[0]= randf( -0.3f, 0.3f );
        vel[1]= randf( -0.3f, 0.3f );
        vel[2]= randf( -0.3f, 0.3f );
        AddParticle( pos, vel, PARTICLE_GAUSS, randf( 0.5f, 1.0f ) );
    }
}

void ParticleManager::AddDirtSplatter( const float* pos )
{
    float vel[3]= { 0.0f, 0.0f, 1.0f };
    float eps[3];

    for( int i= 0; i< 16; i++ )
    {
        eps[0]= randf( -0.4f, 0.4f );
        eps[1]= randf( -0.4f, 0.4f );
        eps[2]= randf( 0.6f );
        Vec3Add( eps, vel );
        AddParticle( pos, eps, PARTICLE_DIRT_SPLATTER, 1.0f );
    }
}

void ParticleManager::Tick()
{
    //kill particles
    for( unsigned int i= 0; i< particle_count; )
    {
        if( particles[i].death_time <= MainLoop::CurrentTime() )
        {
            if( i < particle_count - 1 )
                particles[i]= particles[particle_count - 1];
            particle_count--;
        }
        else
            i++;
    }

    //move particles and write positions into buffer
    float dt= MainLoop::FrameDT();
    for( unsigned int i= 0; i< particle_count; i++ )
    {
        for( unsigned int k= 0; k< 3; k++ )
        {
            particles[i].pos[k]+= dt * particles[i].vel[k];
            particle_pos_buffer[i*4+k]= particles[i].pos[k];
        }
        if( particle_gravity_factor[ particles[i].particle_id ]  )
            particles[i].vel[2]+= GRAVITY * MainLoop::FrameDT();
        particle_pos_buffer[i*4+3]= float(particles[i].particle_id) + particle_size_table[particles[i].particle_id] * 16.0f;
    }
}