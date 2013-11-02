#ifndef SOUNDS_H
#define SOUNDS_H

extern   unsigned char crowbar_sound[];
extern   unsigned char chaingun_sound[];
extern   unsigned char shot357_sound[];
extern   unsigned char ambient7_sound[];
extern   unsigned char quad_sound[];

unsigned int GenBlasterShot( short** data );
unsigned int GenShovelHit( short** data );

extern unsigned int (*SoundGenFunc[])( short**); 

#define SOUND_SHOVEL_HIT 0
#define SOUND_PISTOL_SHOT 1
#define SOUND_BLASTER_SHOT 2
#define SOUND_COUNT 3
#endif//SOUNDS_H