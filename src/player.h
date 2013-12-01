#ifndef PLAYER_H
#define PLAYER_H

#include "level.h"

class Player
{
public:
    Player(Level * lev);
    ~Player()
    {

    }

    void Tick();
    void Shot();
    
    unsigned int DeathTime()const;
    int Health();
    unsigned int Ammo();
    unsigned int AmmoType() const;
    unsigned int Score()const;

    void Accelerate( const float* vec );
    void Move(float dt );
    void Jump();
    void SetPosition( const float* new_pos );
    void RotateXY( float a );
    void RotateZ( float a );

    void AddPoints( unsigned int p );
    void Damage( int dmg );
    void AddHealth( int h );
    bool OnSurface();
    float Velocity();
    void Velocity( float* v );
    void SetVelocity( float* v );
    const float* Position();
    float AngXY();
    float AngZ();

    char CurrentWeapon();

    void NextWeapon();
    void PreviousWeapon();
    void SetWeapon( unsigned int i );
    void GiveWeapon( unsigned char weapon_id );
    void GiveAmmo( unsigned int ammo_id, unsigned int count );
    void Spawn();

    float WeaponAnimationTime() const;
    bool WeaponAnimationIsActive() const;

    float BloodFactor();

private:
    Level* level;
    float pos[3];
    float vel[3];
    float ang_xy;
    float ang_z;
    float blood_factor;

    bool on_surface;

    int health;
    unsigned int score;

    bool weapons[20];
    unsigned int ammo[ AMMO_COUNT ];
    char current_weapon;
    unsigned int last_shot_time;

    bool in_attack;
    bool alrady_shot;
    bool weapon_animation_is_active;
    float weapon_animation_time;

    void DeathTick();
    unsigned int death_time;
}; // class Player


inline bool Player::OnSurface()
{
    return on_surface;
}
inline const float* Player::Position()
{
     return pos;
}

inline float Player::AngXY()
{
    return ang_xy;
}

inline float Player::AngZ()
{
    return ang_z;
}

inline int Player::Health()
{
    return health;
}


inline char Player::CurrentWeapon()
{
    return current_weapon;
}


inline unsigned int Player::Ammo()
{
    return ammo[ weapon_descriptor_table[current_weapon].ammo_id ];
}

inline void Player::SetWeapon( unsigned int i )
{
    if( weapons[i] )
        current_weapon= i;
}

inline void Player::GiveWeapon( unsigned char weapon_id )
{
    current_weapon= weapon_id;
    weapons[weapon_id]= true;
    ammo[ weapon_descriptor_table[weapon_id].ammo_id ]+= weapon_descriptor_table[weapon_id].ammo_count;
}

inline float Player::WeaponAnimationTime() const
{
    return weapon_animation_time;
}
inline bool Player::WeaponAnimationIsActive() const
{
    return weapon_animation_is_active;
}

inline void Player::GiveAmmo( unsigned int ammo_id, unsigned int count )
{
    ammo[ammo_id]+= count;
}


inline float Player::BloodFactor()
{
    return blood_factor;
}
inline unsigned int Player::Score()const
{
    return score;
}
inline void Player::AddPoints( unsigned int p )
{
    score+= p;
}
inline unsigned int Player::AmmoType()const
{
    return weapon_descriptor_table[ current_weapon ].ammo_id;
}

inline unsigned int Player::DeathTime()const
{
    return death_time;
}

#endif// PLAYER_H
