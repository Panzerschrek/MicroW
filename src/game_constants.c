#include "game_constants.h"
#include "sounds.h"

unsigned char healthpack_table[3]= {
10,
50,
100
};

//size of glow arond healthpacks - in 1/64 m
float pickup_size_table[OBJECT_MACHINEGUN+1-OBJECT_SMALL_HEALTHPACK]= {
0.5f,
1.2f,
1.7f,

0.6f,
0.6f,
1.0f
};



struct WeaponDescriptor weapon_descriptor_table[]= {
/*max_distance   delay*256   damage  speed  ammo_id   sound_id   ammo_count   delay_before_shot, delay_after_shot*/
    { 2, 52, 50, 255, AMMO_NOAMMO, SOUND_SHOVEL_HIT,  0,   0.1f, 0.2f },
    { 65535, 32, 10, 255, AMMO_BULLETS, SOUND_PISTOL_SHOT, 10, 0.0f, 0.01f  },
    { 65535, 4, 8, 255, AMMO_CHAINBULLETS, SOUND_BLASTER_SHOT, 40, 0.0f,  0.01f  }
};





struct MonsterDescriptor monster_descriptor_table[MONSTER_COUNT]={
/*radius   heigth   speed   melee_attack_delay   remote_attack_delay
delay_before_melee_attack   delay_after_melee_attack   delay_before_remote_attack   delay_after_remote_attack
max_health   melee_damage   remote_damage   flags*/
    { 1.0f, 1.0f, 2.5f, 1.0f, 2.0f,   0.1f, 0.1f, 0.4f, 0.2f,    60, 20, 20, 0x03 },
    { 0.5f, 1.0f, 3.0f, 1.0f, 3.0f,   0.75f, 0.25f, 0.5f, 0.5f,  40, 10, 10, 0x00 }
 
};
