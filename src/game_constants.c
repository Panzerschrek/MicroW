#include "game_constants.h"
#include "sounds.h"

unsigned char healthpack_table[3]= {
10,
50,
100,
};

const unsigned char ammo_pack_size_table[]={
0, 20, 50, 10, 40 };

//size of glow arond healthpacks - in 1/64 m
float pickup_size_table[OBJECT_GAUSS_GUN+1-OBJECT_SMALL_HEALTHPACK]= {
//health
0.5f,
1.2f,
1.7f,
//ammo
0.7f,
1.0f,
1.0f,
1.0f,
//weapon
0.6f,
0.6f,
1.0f,
1.0f,
1.0f,
};

//must be integer
float particle_size_table[]= {
48.0f,
64.0f,
120.0f,
240.0f,
48.0f,
48.0f,
};

unsigned char bullet_damage_table[]= {
30,//rocket
25,//snowball
35//fireball
};

int particle_gravity_factor[]= {
    0, 1, 0, 0, 1, 0 };


struct WeaponDescriptor weapon_descriptor_table[]= {
/*max_distance   delay*64   damage_bullet  speed  ammo_id   sound_id   ammo_count   blast_radius   dispertion   delay_before_shot, delay_after_shot*/
    { 2, 52, 50, 255, AMMO_NOAMMO, SOUND_SHOVEL_HIT,  0,            0.0f,  0.0f,  0.1f, 0.2f },
    { 65535, 32, 10, 255, AMMO_BULLETS, SOUND_PISTOL_SHOT, 20,      0.0f,  0.00872f, 0.0f, 0.25f  },
    { 65535, 8, 8, 255, AMMO_CHAINBULLETS, SOUND_PISTOL_SHOT,50,   0.0f,  0.02617f, 0.0f,  0.125f  },
    { 65535, 64, BULLET_ROCKET, 20, AMMO_ROCKETS, SOUND_ROCKET_SHOT, 10,        4.0f,  0.00872f, 0.0f, 0.4f },
    { 65535, 12, 12, 255, AMMO_GAUSS_BOLTS, SOUND_BLASTER_SHOT, 40,   0.0f,  0.01745f, 0.0f,  0.1875f },
};





struct MonsterDescriptor monster_descriptor_table[MONSTER_COUNT]={
/*radius   heigth   speed   melee_attack_delay   remote_attack_delay
delay_before_melee_attack   delay_after_melee_attack   delay_before_remote_attack   delay_after_remote_attack
max_health   melee_damage   remote_damage   cost   flags*/
    { 1.0f, 2.0f, 4.5f, 1.0f, 2.0f,   0.1f, 0.1f, 0.4f, 0.2f,    60, 20, 20, 40, 0x03 },
    { 0.5f, 2.5f, 4.0f, 1.0f, 3.0f,   0.75f, 0.25f, 0.75f, 0.75f,  100, 10, 10, 60, 0x00 },
    { 1.0f, 3.0f, 3.5f, 1.0f, 5.0f,   0.5f, 0.5f, 0.1f, 0.8f,  80, 30, 10, 80, 0x00 },
 
};


struct AttackWave attack_waves[]= {
	/* monsters(3), respawn factor, max_monsters*/
	 5, 1, 2, 3.0f,	6,
	 10, 6, 8, 2.0f,	10,
	 8, 4, 15, 1.5f,	15,
	 8, 16, 8, 1.0f,	12,
};
