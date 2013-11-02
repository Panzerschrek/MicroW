#ifndef LEVEL_H
#define LEVEL_H
#include "landscape.h"
#include "game_constants.h"
#include "sound_system.h"
#include "monster.h"

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

    const StaticObject* GetStaticObject( unsigned char object_id, unsigned int i) const;
    const Monster* GetMonster( unsigned int i) const;
    unsigned int ObjectCount( unsigned char object_id ) const;
    unsigned int MonsterCount() const;

    void Shot( const float* pos, const float* vec, unsigned char weapon_id );

private:

    void PlaceTrees();
    void PlaceAchtungTables();
    void PlaceHealthPacks();
    void PlaceWeapons();

    void RemoveObject( StaticObject* obj );

    void PickUp();

	Landscape landscape;
    Player* player;
    SoundSystem* sound_system;
    Monster* monsters[ MAX_LEVEL_MONSTERS ];
    unsigned int monster_count;

    StaticObject* static_objects[ STATIC_OBJECTS ];//position + xy angle + scale + height + radius
    unsigned int object_count[ STATIC_OBJECTS ];

    bool FindCollisionWithLandscape( const float* pos, const float* vec, float* collide_pos );
    bool FindCollisionWithObject( const float* pos, const float* vec, float* collide_pos );

};

inline const Landscape* Level::GetLandscape()
{
	return (const Landscape*) &landscape;
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
#endif // LEVEL_H
