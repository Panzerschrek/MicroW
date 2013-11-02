#ifndef MONSTER_H
#define MONSTER_H

#include "model.h"

class Level;
class Player;

class Monster
{
public:
    Monster(){}
    Monster( Player* p, Level* l, unsigned char monster_id, const float* pos );
    ~Monster(){}
    void Damage( int dmg );
    int Helath();
    const float* Position() const;
    float AngleXY() const;
    unsigned char MonsterId()const;

    void Tick( float dt );
    bool IsCollision( const float* pos, const float* vec, float* collision_pos );

    const bool* ActiveAnimations() const;
    const float* ActiveAnimationsTime() const;

private:

    void Attack();
    void AttackTick();
    void SelectTarget();

    Level* level;
    Player* player;

    int health;
    float pos[3];
    float target_pos[3];
    float angle_xy, angle_z;

    unsigned char monster_id;
    unsigned int last_attack_time;
    unsigned int last_target_select_time;

    bool playable_animations[ANIMATION_KIND];//true if animation active
    float playable_animations_time[ANIMATION_KIND];//time ( from 0.0f ) of current animations

    bool in_melee_attack;
    bool alrady_caused_damage;
};


inline void Monster::Damage( int dmg )
{
    health-= dmg;
}

inline int Monster::Helath()
{
    return health;
}

inline const float* Monster::Position() const
{
    return (const float*) pos;
}

inline unsigned char Monster::MonsterId()const
{
    return monster_id;
}
inline float Monster::AngleXY() const
{
    return angle_xy;
}

inline const bool* Monster::ActiveAnimations() const
{
    return playable_animations;
}
inline const float* Monster::ActiveAnimationsTime() const
{
    return playable_animations_time;
}

#endif//MONSTER_H