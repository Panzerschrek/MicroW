#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H


#define MAX_PARTICLES 1024

#define PARTICLE_BLOOD_SPLATTER 0
#define PARTICLE_DIRT_SPLATTER 1
#define PARTICLE_WEAPON_SMOKE 2
#define PARTICLE_BLUST 3
#define PARTICLE_SNOW_SPLATTER 4
#define PARTICLE_GAUSS 5
#define PARTICLE_TYPE_COUNT 6

struct Particle
{
    float pos[3];
    float vel[3];
    unsigned char particle_id;
    unsigned int death_time;
};

class ParticleManager
{
public:
    ParticleManager();
    ~ParticleManager(){}

    void AddParticle( const float* pos, const float* vel, unsigned char particle_id, float life_time );
    void AddBloodSplatter( const float* pos, const float* vec );
    void AddDirtSplatter( const float* pos );
    void AddWeaponSmoke( const float* pos, const float* vec );
    void AddBlust( const float* pos );
    void AddSnowSplatter( const float* pos );
    void AddGaussShot( const float* pos_begin, const float* pos_end );
    void Tick();
    const float* GetPositionBuffer()const;
    unsigned int ParticleCount() const;

private:
    Particle* particles;
    unsigned int particle_count;
    float* particle_pos_buffer;
};


inline const float* ParticleManager::GetPositionBuffer()const
{
    return particle_pos_buffer;
}
inline unsigned int ParticleManager::ParticleCount() const
{
    return particle_count;
}


#endif//PARTICLE_MANAGER_H