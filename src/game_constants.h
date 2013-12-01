#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

#ifdef __cplusplus
extern "C"
{
#endif//__cplusplus


#define OBJECT_SPRUCE 0
#define OBJECT_TREE0 1
#define OBJECT_TREE1 2
#define OBJECT_BUSH 3
#define OBJECT_ACHTUNG_TABLE 4
#define OBJECT_SMALL_HEALTHPACK 5
#define OBJECT_MEDIUM_HEALTHPACK 6
#define OBJECT_LARGE_HEALTHPACK 7
#define OBJECT_BULLETS_AMMO_BOX 8
#define OBJECT_CHAINBULLETS_AMMO_BOX 9
#define OBJECT_ROCKETS_AMMO_BOX 10
#define OBJECT_GAUSS_BOLTS_AMMO_BOX 11
#define OBJECT_SHOVEL 12
#define OBJECT_PISTOL 13
#define OBJECT_MACHINEGUN 14
#define OBJECT_ROCKET_LAUNCHER 15
#define OBJECT_GAUSS_GUN 16
#define STATIC_OBJECTS 17


#define OBJECT_IS_PICKUP(x) ( (x) >= OBJECT_SMALL_HEALTHPACK && (x) <= OBJECT_GAUSS_GUN )

#define GRAVITY (-9.81f)

#define MAX_GROUND_SPEED 6.0f
#define GROUND_ACCELERATION 25.0f
#define GROUND_FRICTION 25.0f

#define MAX_AIR_SPEED 30.0f
#define AIR_ACCELERATION 0.5f
#define AIR_FRICTION 0.5f

#define MAX_VERTICAL_SPEED 20.0f

#define JUMP_DELTA_V 4.5f

#define COMBAT_ZONE_RADIUS 480.0f



// object phys parametrs
#define SPRUCE_RADIUS 0.6f
#define SPRUCE_HEIGHT 18.0f

#define PLAYER_HEIGHT 1.74f
#define PLAYER_RADIUS 0.5f

#define ACHTUNG_TABLE_RADIUS 0.2f
#define ACHTUNG_TABLE_HEIGHT 0.8f

//weapons
#define WEAPON_SHOVEL 0
#define WEAPON_PISTOL 1
#define WEAPON_MACHINEGUN 2
#define WEAPON_ROCKET_LAUNCHER 3
#define WEAPON_GAUSS_GUN 4
#define WEAPON_COUNT 5

#define AMMO_NOAMMO 0
#define AMMO_BULLETS 1
#define AMMO_CHAINBULLETS 2
#define AMMO_ROCKETS 3
#define AMMO_GAUSS_BOLTS 4
#define AMMO_COUNT 5

extern const unsigned char ammo_pack_size_table[]; 
struct WeaponDescriptor
{
    unsigned short max_distance;
    unsigned char delay;//in 1/64 s
    unsigned char damage_bullet;//if speed < 255 it is bullet_id 
    unsigned char speed;//speed of shell. 255= infinity
    unsigned char ammo_id;
    unsigned char sound_id;
    unsigned char ammo_count;
    float blast_radius;
    float dispertion;//dispertion angle
    float delay_before_shot;
    float delay_after_shot;
};
extern struct WeaponDescriptor weapon_descriptor_table[];




#define MONSTER_CACODEMON 0
#define MONSTER_ROBOT 1
#define MONSTER_SNOWMAN 2
#define MONSTER_COUNT 3
#define MAX_LEVEL_MONSTERS 64


//monster flags
#define MONSTER_CAN_FLY(x)          (x&1)
#define MONSTER_IS_SPHERICAL(x)     (x&2)
#define MONSTER_IS_CYLINDRICAL(x)  (!(x&2))
//monster flags

struct MonsterDescriptor
{
    float radius;
    float heigth;
    float speed;
    float melee_attack_delay;
    float remote_attack_delay;

    float delay_before_melee_attack;
    float delay_after_melee_attack;
    float delay_before_remote_attack;
    float delay_after_remote_attack;

    unsigned short max_health;
    unsigned short melee_damage;
    unsigned short remote_damage;
    unsigned int cost;
    unsigned char flags;

};
extern struct MonsterDescriptor monster_descriptor_table[];



//health 
#define PLAYER_MAX_HEALTH 200
extern unsigned char healthpack_table[];
extern float pickup_size_table[];


#define MONSTER_STATE_MOVE_TO_TARGET 0
#define MONSTER_STATE_REMOTE_ATTACK 1
#define MONSTER_STATE_MELEE_ATTACK 2


#define BULLET_ROCKET 0
#define BULLET_SNOW_BALL 1
#define BULLET_FIRE_BALL 2
#define BULLET_TYPE_COUNT 3
extern unsigned char bullet_damage_table[];


extern float particle_size_table[];
extern int particle_gravity_factor[];



struct AttackWave
{
	unsigned int monster_count[MONSTER_COUNT];
	float respawn_factor;//spawned monsters per second
	unsigned int max_monsters;//max monsters in one time

};

extern struct AttackWave attack_waves[];
#define ATTACK_WAVES 4

#ifdef __cplusplus
}//extern "C"
#endif//__cplusplus

#endif//GAME_CONSTANTS_H