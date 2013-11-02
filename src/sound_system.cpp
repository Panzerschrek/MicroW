#include "sound_system.h"

#include "main_loop.h"
#include "math.h"
#include "sounds.h"
#include "time.h"

SoundSystem::SoundSystem()
{
#ifdef MW_OS_WINDOWS
    
    if( DirectSoundCreate8( &DSDEVID_DefaultPlayback, &direct_sound, NULL ) )
        exit( 1488 );
        
    direct_sound->SetCooperativeLevel( MainLoop::GetHWND(), DSSCL_PRIORITY );

    DSBUFFERDESC primary_buffer_info;
    ZeroMemory(&primary_buffer_info, sizeof(DSBUFFERDESC));
    primary_buffer_info.dwSize= sizeof(DSBUFFERDESC);
    primary_buffer_info.dwBufferBytes= 0;
    primary_buffer_info.guid3DAlgorithm= GUID_NULL;
    primary_buffer_info.lpwfxFormat= NULL;
    primary_buffer_info.dwFlags= DSBCAPS_PRIMARYBUFFER | /*DSBCAPS_CTRLVOLUME*/DSBCAPS_CTRL3D;


    direct_sound->CreateSoundBuffer( &primary_buffer_info, &primary_sound_buffer, NULL );

    primary_sound_buffer->QueryInterface(IID_IDirectSound3DListener8,
                                 (LPVOID *)&listener);

    primary_sound_buffer->Play( 0, 0, DSBPLAY_LOOPING );


    current_sounds_count= 0;

#else
#endif

    for( unsigned int i= 0; i< SOUND_COUNT; i++ )
        GenSound(i);

}




void SoundSystem::GenSound( unsigned char sound_id )
{
    unsigned int sound_len;
    short* sound_data;
    sound_len= SoundGenFunc[sound_id]( &sound_data );
    sound_length[ sound_id ]= float(sound_len)/ 22050.0f;

#ifdef MW_OS_WINDOWS
    WAVEFORMATEX sound_format;
    ZeroMemory(&sound_format, sizeof(WAVEFORMATEX));
    sound_format.nSamplesPerSec= 22050;
    sound_format.wFormatTag= WAVE_FORMAT_PCM;
    sound_format.nChannels= 1;
    sound_format.nAvgBytesPerSec= 22050 * 2;
    sound_format.wBitsPerSample= 16;
    sound_format.nBlockAlign= 2;

    DSBUFFERDESC secondary_buffer_info;
    ZeroMemory( &secondary_buffer_info, sizeof(DSBUFFERDESC) );
    secondary_buffer_info.dwSize= sizeof(DSBUFFERDESC);
    secondary_buffer_info.dwFlags= /*DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN |*/ DSBCAPS_CTRL3D;
    secondary_buffer_info.dwBufferBytes= sound_len * 2;
    secondary_buffer_info.lpwfxFormat= &sound_format;

    

    direct_sound->CreateSoundBuffer( &secondary_buffer_info, &buffers[ sound_id ], 0 ); 

    char* buff_data1, *buff_data2= NULL;
    unsigned int buff_size1, buff_size2= 0;
    if( buffers[ sound_id ]->Lock( 0, sound_len * 2,
        (LPVOID*)&buff_data1, 
        (LPDWORD)&buff_size1,
        (LPVOID*)&buff_data2,
        (LPDWORD)&buff_size2,
        0 ) != DS_OK )
        printf( "bufer lock error\n" );

    memcpy( buff_data1, sound_data, sound_len * 2 );

    if( buffers[ sound_id ]->Unlock( buff_data1, buff_size1, NULL, 0 ) != DS_OK )
        printf( "ds unlock error\n" );
#else
#endif

}


void SoundSystem::SetListenerAngle( float xy_angle, float z_angle )
{
    float listener_v[3];
    listener_v[1]= cos( xy_angle );
    listener_v[0]= -sin( xy_angle );
    listener_v[2]= 0.0f;
    listener->SetOrientation( listener_v[0], listener_v[1], listener_v[2],
        0.0f, 0.0f, -1.0f, DS3D_IMMEDIATE ); 
}



void SoundSystem::Tick()
{
#ifdef MW_OS_WINDOWS

    unsigned int current_time= clock();
    float dt;
    for( int i= 0; i< (int)current_sounds_count; i++ )
    {
        dt= float( current_time - sounds_init_time[i] ) / float(CLOCKS_PER_SEC);
        if( dt > sound_length[sound_parent[i]] )
        {
            secondary_buffers[i]->Stop();
            secondary_buffers3d[i]->Release();
            secondary_buffers[i]->Release();
            current_sounds_count--;
        }
    }

     for( int i= 0; i< (int)current_sounds_count; )
     {
        dt= float( current_time - sounds_init_time[i] ) / float(CLOCKS_PER_SEC);
        if( dt > sound_length[sound_parent[i]] )
        {
            if( i != current_sounds_count - 1 )
            {
                secondary_buffers[i]= secondary_buffers[ current_sounds_count - 1 ];
                sounds_init_time[i]= sounds_init_time[ current_sounds_count - 1 ];
                secondary_buffers3d[i]= secondary_buffers3d[ current_sounds_count - 1 ];
            }
             current_sounds_count--;
        }
        else
            i++;
     }
    
     listener->SetPosition( player_pos[0], player_pos[1], player_pos[2],DS3D_IMMEDIATE );
#else
#endif
}


void SoundSystem::PlaySound(const float *pos, float volume, unsigned char sound_id, bool sound_from_player )
{
#ifdef MW_OS_WINDOWS

    if( current_sounds_count >= MAX_PARALLEL_SOUNDS )
        return;

    direct_sound->DuplicateSoundBuffer( buffers[ sound_id ], &secondary_buffers[current_sounds_count]  );
    
    secondary_buffers[current_sounds_count]->QueryInterface( IID_IDirectSound3DBuffer8, (LPVOID*) &secondary_buffers3d[current_sounds_count]  );
    sounds_init_time[current_sounds_count]= clock();

    sound_parent[current_sounds_count]= sound_id;
    secondary_buffers3d[current_sounds_count]->SetMaxDistance( 1000.0f, DS3D_IMMEDIATE );
    secondary_buffers3d[current_sounds_count]->SetMinDistance( 10.0f, DS3D_IMMEDIATE );
     if( sound_from_player )
     {
        secondary_buffers3d[current_sounds_count]->SetMode( DS3DMODE_HEADRELATIVE, DS3D_IMMEDIATE );
        secondary_buffers3d[current_sounds_count]->SetPosition( 0.0f, 0.0f, 0.0f, DS3D_IMMEDIATE );
     }
     else
         secondary_buffers3d[current_sounds_count]->SetPosition( pos[0], pos[1], pos[2], DS3D_IMMEDIATE );
    secondary_buffers[current_sounds_count]->Play( 0, 0, 0 );
    current_sounds_count++;
#else
#endif
}