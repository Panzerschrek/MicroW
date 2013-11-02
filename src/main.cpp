#include "mw.h"
#include "main_loop.h"
#include "renderer.h"

#include "math.h"

#include "sound_system.h"

//#include <MMSystem.h>
//#pragma comment(lib,"winmm.lib")

int main()
{
  //  mciSendString("play serious_sam3.mid",NULL,0,0);
  //  Sleep(1000);
  //  mciSendString( "pause serious_sam3.mid",NULL,0,0 );
  
    Level* level= new Level();
    Player* player= new Player(level);
    level->SetPlayer( player );
    
    MainLoop* main_loop= new MainLoop( level, player );

    SoundSystem* sound_system= new SoundSystem();
    main_loop->SetSoundSystem( sound_system );
    level->SetSoundSystem( sound_system );

    
    float v[]= { -5.0f, 3.0f, 0.0f };
    printf( "angle %f\n", Vec3XYAngle(v) * 180.0f / MW_PI );
	while(1)
		main_loop->Loop();
}
