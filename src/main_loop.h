#ifndef MAIN_LOOP_H
#define MAIN_LOOP_H
#include "mw.h"
#include "renderer.h"
#include "player.h"
#include "level.h"
#include "sound_system.h"

class MainLoop
{
public:
    MainLoop( Level* l, Player* p );
    void SetSoundSystem( SoundSystem* ss );
    ~MainLoop() {}

    void Loop();


private:

	void InitOGL();
	void SetupOGLState();
    void MovePlayer();
    void ResizeWindow();

	int screen_x, screen_y;


	Player* player;
	Renderer* renderer;
    SoundSystem* sound_system;
	Level* level;

    bool keys[256];
    bool mouse_keys[2];
    bool use_mouse;

#ifdef MW_OS_WINDOWS


	HWND hwnd;
	HDC hdc;
	HGLRC hrc;
	WNDCLASSEX window_class;

    int border_x, border_y;

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
    static HWND GetHWND();
#else
    XEvent     event;
    Display   *dpy;
    Window     win;

#endif
public://hack
	static MainLoop* current_main_loop;
};

inline  HWND MainLoop::GetHWND()
{
    return current_main_loop->hwnd;
}

inline void MainLoop::SetSoundSystem( SoundSystem* ss )
{
    sound_system= ss;
}

#endif//MAIN_LOOP_H
