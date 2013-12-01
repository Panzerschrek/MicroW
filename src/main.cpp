#include "mw.h"
#include "main_loop.h"
#include "renderer.h"

#include "math.h"

#include "sound_system.h"

//#include <MMSystem.h>
//#pragma comment(lib,"winmm.lib")

#ifdef MW_OS_WINDOWS
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
#else
int main()
#endif
{
    SetFloatingPointRoundingToTruncate();
  //  mciSendString("play midi/god_sight.mid",NULL,0,0);
  //  Sleep(1000);

    Level* level= new Level();
    Player* player= new Player(level);
    level->SetPlayer( player );
    
    MainLoop* main_loop= new MainLoop( level, player );

    SoundSystem* sound_system= new SoundSystem();
    main_loop->SetSoundSystem( sound_system );
    level->SetSoundSystem( sound_system );

   
	while(1)
		main_loop->Loop();
}
