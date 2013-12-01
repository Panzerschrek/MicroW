#ifndef MAIN_LOOP_H
#define MAIN_LOOP_H
#include "mw.h"
#include "renderer.h"
#include "player.h"
#include "level.h"
#include "sound_system.h"
#include "time.h"

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
    char key_forward, key_back, key_left, key_right, key_jump, key_down;
    float mouse_speed;
    void LoadConfig();
public://hack
	static MainLoop* current_main_loop;

private:
    static unsigned int current_time;
    static unsigned int prev_time;
public:
    static float FrameDT() { return float( current_time - prev_time )/float(CLOCKS_PER_SEC); }
    static unsigned int PrevFrameTime() { return prev_time; }
    static unsigned int CurrentTime(){ return current_time; }
    static void SwapBuffers();

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
