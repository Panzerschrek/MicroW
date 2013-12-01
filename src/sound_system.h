#ifndef SOUND_SYSTEM_H
#define SOUND_SYSTEM_H
#include "mw.h"

#ifdef MW_OS_WINDOWS
#include <dsound.h>
#else
#endif
#include "sounds.h"




#define MAX_PARALLEL_SOUNDS 12

class SoundSystem
{
public:
    SoundSystem();
    ~SoundSystem(){}

    void PlaySound( const float* pos, float volume, unsigned char sound_id, bool sound_from_player= false );
    void GenSound( unsigned char sound_id );
    void SetListenerPos( const float* pos );
    void SetListenerAngle( float xy_angle, float z_angle= 0.0f );

    void Tick();
private:

    float player_pos[3];
#ifdef MW_OS_WINDOWS
    LPDIRECTSOUND8 direct_sound;
    LPDIRECTSOUNDBUFFER primary_sound_buffer;
    LPDIRECTSOUND3DLISTENER listener;

    LPDIRECTSOUNDBUFFER buffers[SOUND_COUNT];
    float sound_length[SOUND_COUNT];
   // LPDIRECTSOUND3DBUFFER buffers3d[SOUND_COUNT];

    LPDIRECTSOUNDBUFFER secondary_buffers[ MAX_PARALLEL_SOUNDS ];
    LPDIRECTSOUND3DBUFFER secondary_buffers3d[ MAX_PARALLEL_SOUNDS ];

    unsigned int sound_parent[MAX_PARALLEL_SOUNDS];
    unsigned int sounds_init_time[MAX_PARALLEL_SOUNDS];
    float sounds_pos[MAX_PARALLEL_SOUNDS][3];
    unsigned int current_sounds_count;

#else
#endif


};

inline void SoundSystem::SetListenerPos( const float* pos )
{
    player_pos[0]= pos[0];
    player_pos[1]= pos[1];
    player_pos[2]= pos[2];
}
#endif//SOUND_SYSTEM_H