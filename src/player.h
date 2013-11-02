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

    void Tick( float dt );
    void Shot();
    
    int Health();
    unsigned int Ammo();

    void Accelerate( const float* vec );
    void Move(float dt );
    void Jump();
    void SetPosition( const float* new_pos );
    void RotateXY( float a );
    void RotateZ( float a );

    void Damage( int dmg );
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

    float WeaponAnimationTime() const;
    bool WeaponAnimationIsActive() const;

private:
    Level* level;
    float pos[3];
    float vel[3];
    float ang_xy;
    float ang_z;

    bool on_surface;

    int health;

    bool weapons[20];
    unsigned int ammo[ AMMO_COUNT ];
    char current_weapon;
    unsigned int last_shot_time;

    bool in_attack;
    bool alrady_shot;
    bool weapon_animation_is_active;
    float weapon_animation_time;
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

inline void Player::Damage( int dmg )
{
    health-= dmg;
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

#endif// PLAYER_H
