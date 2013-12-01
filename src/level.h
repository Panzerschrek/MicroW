#ifndef LEVEL_H
#define LEVEL_H
#include "landscape.h"
#include "game_constants.h"
#include "sound_system.h"
#include "monster.h"
#include "particle_manager.h"

class Player;

struct StaticObject
{
    float pos[3];
    float angle;
    float scale;
    float height;
    float radius;
    unsigned char object_id;
    char reseved[3];
};

struct Bullet
{
    float pos[3];
    float vel[3];
    bool gravity;
    unsigned char bullet_type;
};
#define MAX_BULLETS 96





class Level
{
public:
    Level();
    ~Level();
    void SetPlayer( Player* player );
    void SetSoundSystem( SoundSystem* ss );


    void PhysTick();
    void CollidePlayer();

    const Landscape* GetLandscape();
    const ParticleManager* GetParticleManager();

    const StaticObject* GetStaticObject( unsigned char object_id, unsigned int i) const;
    const Monster* GetMonster( unsigned int i) const;
    const Bullet* GetBullets() const;
    unsigned int BulletCount() const;
    unsigned int ObjectCount( unsigned char object_id ) const;
    unsigned int MonsterCount() const;
	unsigned int Wave() const;

   // void Shot( const float* pos, const float* vec, unsigned char weapon_id, bool from_player= false );//if shot not from player, weapon id = monster_id
    void PlayerShot( const float* pos, const float* vec, unsigned char weapon_id );
    void MonsterShot( const float* pos, const float* vec, unsigned char monster_id );
    SoundSystem* GetSoundSystem();

private:

    void PlaceTrees();
    void PlaceAchtungTables();
    void PlaceHealthPacks();
    void PlaceAmmo();
    void PlaceWeapons();

    void RestartLevel();

    void RemoveObject( StaticObject* obj );

    void PickUp();
    void CollideMonsters();

	void  KillMonster( Monster* m );

    ParticleManager particle_manager;
	Landscape landscape;
    Player* player;
    SoundSystem* sound_system;
    Monster* monsters[ MAX_LEVEL_MONSTERS ];
    unsigned int monster_count;

    StaticObject* static_objects[ STATIC_OBJECTS ];//position + xy angle + scale + height + radius
    unsigned int object_count[ STATIC_OBJECTS ];

	unsigned int wave;
	unsigned int wave_monsters_left[ MONSTER_COUNT ];
	void NextWave();
	void SpawnMonsters();

    Bullet bullets[ MAX_BULLETS ];
    unsigned int bullet_count;
    void AddBullet( const float* pos, const float* vel, unsigned char bullet_type );
    void BulletsTick();
    unsigned int RemoveBullet( unsigned int i );
    void RocketBlust( Bullet* b );

    bool FindCollisionWithLandscape( const float* pos, const float* vec, float* collide_pos );
    bool FindCollisionWithObject( const float* pos, const float* vec, float* collide_pos );

};

inline const Landscape* Level::GetLandscape()
{
	return (const Landscape*) &landscape;
}

inline const ParticleManager* Level::GetParticleManager()
{
    return &particle_manager;
}

inline const StaticObject* Level::GetStaticObject(unsigned char object_id, unsigned int i) const
{
    return static_objects[object_id]  + i;
}

inline unsigned int Level::ObjectCount( unsigned char object_id ) const
{
    return object_count[ object_id ];
}

inline void Level::SetSoundSystem( SoundSystem* ss )
{
    sound_system= ss;
}

inline const Monster* Level::GetMonster( unsigned int i) const
{
    return (const Monster*) monsters[i];
}

inline unsigned int Level::MonsterCount() const
{
    return monster_count;
}

inline SoundSystem* Level::GetSoundSystem()
{
    return sound_system;
}

inline const Bullet* Level::GetBullets() const
{
    return bullets;
}

inline unsigned int Level::BulletCount() const
{
    return bullet_count;
}

inline unsigned int Level::Wave() const
{
	return wave;
}
#endif // LEVEL_H
