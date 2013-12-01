#include "main_loop.h"
#include "gl_functions.h"
#include <time.h>
#include "math.h"

MainLoop* MainLoop::current_main_loop= NULL;
unsigned int MainLoop::current_time= 0;
unsigned int MainLoop::prev_time= 0;

inline void MainLoop::ResizeWindow()
{
#ifdef MW_OS_WINDOWS
      RECT rect;
        GetClientRect( current_main_loop->hwnd, &rect );
     
        if( current_main_loop->renderer != NULL )
        {
            renderer->SetViewport( rect.right, rect.bottom);
            screen_x= rect.right;
            screen_y= rect.bottom;
        }
#else
#endif
}



#ifdef MW_OS_WINDOWS


#define KEY(x) (65 + x - 'A' )
LRESULT CALLBACK MainLoop::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    if( uMsg == WM_CLOSE )
        PostQuitMessage(0);
    switch( uMsg )
    {
    case WM_SIZE:
      current_main_loop->ResizeWindow();
     break;
    case WM_CLOSE:
        exit(0);

    case WM_KEYUP:
        if( wParam < 256 )
            current_main_loop->keys[ wParam ]= false;
        break;

    case WM_SETFOCUS:
        current_main_loop->use_mouse= true;
        ShowCursor( false );
        break;
    case WM_KILLFOCUS:
        current_main_loop->use_mouse= false;
        ShowCursor( true );
        break;

    case WM_KEYDOWN:
    {
        if( wParam < 256 )
            current_main_loop->keys[ wParam ]= true;

		switch( wParam )
		{
            case KEY(' '):
            current_main_loop->player->Jump();
            break;

           /* case KEY('M'):
            ShowCursor( current_main_loop->use_mouse );
            current_main_loop->use_mouse = !current_main_loop->use_mouse;
            break;*/

			case VK_ESCAPE:
			exit(0);
            break;

			default:
                 if( wParam <= 57 && wParam>= 49 )
                    current_main_loop->player->SetWeapon( wParam - 49 );
			break;
		}

      
    }//key down
    break;

    case WM_LBUTTONDOWN://shot button
           // current_main_loop->player->Shot();
            current_main_loop->mouse_keys[0]= true;
            break;
     case WM_LBUTTONUP://shot button
         current_main_loop->mouse_keys[0]= false;
         break;
     case WM_RBUTTONUP:
         current_main_loop->renderer->Zoom();
         break;

        case WM_MOUSEWHEEL:
            if( GET_WHEEL_DELTA_WPARAM(wParam) > 0 )
                current_main_loop->player->PreviousWeapon();
            else
                current_main_loop->player->NextWeapon();
        break;

#ifdef MRPG_DEBUG
        printf( "key: %d\n", wParam );
#endif
        break;


    default:
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

#endif//win



inline void MainLoop::InitOGL()
{
#ifdef MW_OS_WINDOWS

    int border_size, top_border_size, bottom_border_size;
    static const char* WINDOW_NAME= "MW";

    window_class.cbSize = sizeof(WNDCLASSEX);
    window_class.style = CS_OWNDC;
    window_class.lpfnWndProc = WindowProc;
    window_class.cbClsExtra = 0;
    window_class.cbWndExtra = 0;
    window_class.hInstance =  0;
    window_class.hIcon = LoadIcon( 0 , IDI_APPLICATION);
    window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    window_class.lpszMenuName = NULL;
    window_class.lpszClassName = WINDOW_NAME;
    window_class.hIconSm = LoadIcon(NULL, IDI_APPLICATION);


    if ( ! RegisterClassEx( &window_class ) )
        goto display_error;


    border_size=  GetSystemMetrics(SM_CXFIXEDFRAME);
    bottom_border_size= GetSystemMetrics(SM_CYFIXEDFRAME);
    top_border_size= bottom_border_size + GetSystemMetrics(SM_CYCAPTION);

       border_x= border_size * 2;
       border_y= top_border_size + bottom_border_size;
    hwnd  = CreateWindowEx(0,
                           WINDOW_NAME,
                           WINDOW_NAME,
                           /*WS_OVERLAPPED|WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU*/WS_OVERLAPPEDWINDOW,
                           0,
                           0,
                           screen_x + border_size * 2,
                           screen_y + top_border_size + bottom_border_size,
                           NULL,
                           NULL,
                           /*h_instance*/0,
                           NULL);

    if ( ! hwnd )
        goto display_error;

    ShowWindow( hwnd, SW_SHOWNORMAL );
    hdc= GetDC( hwnd );


    wglMakeCurrent( 0, 0 );
    PIXELFORMATDESCRIPTOR pfd;
    int format;

    ZeroMemory( &pfd, sizeof(pfd) );
    pfd.nVersion= 1;
    pfd.dwFlags= PFD_DRAW_TO_WINDOW|
                 PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType= PFD_TYPE_RGBA;
    pfd.cColorBits= 32;
    pfd.cDepthBits= 32;
    pfd.cStencilBits= 8;
    pfd.iLayerType= PFD_MAIN_PLANE;

    format= ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, format, &pfd);

    hrc= wglCreateContext( hdc );
    wglMakeCurrent( hdc, hrc );


     PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB= NULL;
    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

     wglMakeCurrent( NULL, NULL );
    wglDeleteContext( hrc );

     int attribs[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_FLAGS_ARB,        /* WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB*/ 0x0002,
        /*WGL_CONTEXT_PROFILE_MASK_ARB*/0x9126,  /*WGL_CONTEXT_CORE_PROFILE_BIT_ARB*/0x00000001,
        0
    };

    hrc= wglCreateContextAttribsARB( hdc, 0, attribs );
    wglMakeCurrent( hdc, hrc );

    //Tun off vsync
    /*PFNWGLSWAPINTERVALEXTPROC wglSwapInterval= (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress( "wglSwapIntervalEXT" );
    wglSwapInterval(0);
    */

#else
    int dblBuf[]  = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};

    XVisualInfo         *vi;
    Colormap             cmap;
    XSetWindowAttributes swa;
    GLXContext           cx;
    int                  dummy;


    dpy = XOpenDisplay(NULL);
    if (dpy == NULL )
        goto display_error;


    if(!glXQueryExtension(dpy, &dummy, &dummy))
        goto display_error;

    if ( ( vi= glXChooseVisual(dpy, DefaultScreen(dpy), dblBuf) ) == NULL)
        goto display_error;

    cx = glXCreateContext(dpy, vi, /* no shared dlists */ None,
                          /* direct rendering if possible */ GL_TRUE );
    if (cx == NULL)
        goto display_error;


    cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.border_pixel = 0;

    swa.event_mask = KeyPressMask    | ExposureMask
                     | ButtonPressMask | StructureNotifyMask;
    win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0,
                        screen_x, screen_y, 0, vi->depth, InputOutput, vi->visual,
                        CWBorderPixel | CWColormap | CWEventMask, &swa);
    XSetStandardProperties(dpy, win, "MW", "MW", None,
                           NULL, 0, NULL);



    glXMakeCurrent(dpy, win, cx);
    XMapWindow(dpy, win);

#endif

    int ver[2];
    glGetIntegerv( GL_MAJOR_VERSION, ver );
    glGetIntegerv( GL_MINOR_VERSION, ver + 1 );
    if( ver[0] * 10 + ver[1] < MW_GL_VERSION )
        exit(1025);


    return;
display_error:
    exit(1024);
}


void MainLoop::SwapBuffers()
{
    ::SwapBuffers( current_main_loop->hdc );
}

inline void MainLoop::SetupOGLState()
{
    //gl state initialisation
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL );
    glClearDepth(1.0f);
   // glClearColor(0.019f, 0.654f, 0.811f, 0.0f );
    glViewport(0, 0, screen_x, screen_y );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glCullFace( GL_FRONT );
    glEnable( GL_CULL_FACE );
}

char ToUpperCase( char c )
{
    if( c >= 'a' && c <= 'z' )
        c-=32;
    return c;
}

void MainLoop::LoadConfig()
{   
    FILE* f= fopen( "config.cfg", "rb" );
    if( f == NULL )
        return;

    char str[64];
    char str2[64];
    while( fscanf( f, "%s", str ) > 0 )
    {
        fscanf( f, "%s", str2 );

        if( !strcmp( str, "move_forward" ) )
            key_forward= str2[0];
        else if( !strcmp( str, "move_backpedal" ) )
            key_back= str2[0];
        else if( !strcmp( str, "move_left" ) )
            key_left= str2[0];
        else if( !strcmp( str, "move_right" ) )
            key_right= str2[0];
        else if( !strcmp( str, "screen_width" ) )
        {
            screen_x= atoi(str2);
            if( screen_x > 2048 )
                screen_x= 2048;
        }
        else if( !strcmp( str, "screen_height" ) )
        {
            screen_y= atoi(str2);
            if( screen_y > 2048 )
                screen_y= 2048;
        }
        else if( !strcmp( str, "mouse_speed" ) )
        {
            mouse_speed= float( atof(str2) ); 
            if( mouse_speed < 1.0f )
                mouse_speed= 1.0f;
            else if( mouse_speed > 10.0f )
                mouse_speed= 10.0f;
        }
    }
    fclose(f);

    key_forward= ToUpperCase(key_forward);
    key_back= ToUpperCase(key_back);
    key_left= ToUpperCase(key_left);
    key_right= ToUpperCase(key_right);

}

MainLoop::MainLoop( Level* l, Player* p ):
    level(l), player(p), use_mouse(false), renderer(NULL), sound_system(NULL)
{

    key_forward= 'W';
    key_back= 'S';
    key_left= 'A';
    key_right= 'D';
    mouse_speed= 1.0f;
    screen_x= 1024;
    screen_y= 768;
    LoadConfig();

    current_main_loop= this;

    for( int i= 0; i< 256; i++ )
        keys[i]= false;
    mouse_keys[0]= mouse_keys[1]= false;

    InitOGL();
    GetGLFunctions();
    SetupOGLState();

    renderer= new Renderer( level, player );

    int t0= clock();
    renderer->Init();
    int dt= clock() - t0;
    const int delay= CLOCKS_PER_SEC*3;
    if( dt < delay )
#ifdef MW_OS_WINDOWS
        Sleep( (delay-dt)*1000/CLOCKS_PER_SEC );
#else
        usleep( (delay-dt)*1000000/CLOCKS_PER_SEC );
#endif
}

void MainLoop::Loop()
{

#ifdef MW_OS_WINDOWS

    MSG msg;
    while (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
    {

        TranslateMessage(&msg);
        DispatchMessage(&msg);

    }
#else
    XEvent               event;
    while(XPending(dpy))
    {


        XNextEvent(dpy, &event);

        switch (event.type)
        {
        case KeyPress:
        {
            KeySym     keysym;
            XKeyEvent *kevent;
            char       buffer[1];

            kevent = (XKeyEvent *) &event;


            if( XLookupString((XKeyEvent *)&event,buffer,1,&keysym,NULL) != 1 )
                break;


            switch (keysym)
            {
            case XK_Up:

                break;

            case XK_Down:
                break;

			case XK_Escape:
			case XK_Q:
			case XK_q:
				exit(0);

            default:
                break;
            }
            break;
        }//key press

        case ButtonPress:
        {
            switch (event.xbutton.button)
            {
            case Button4:
                renderer->ZoomIn();
                break;

            case Button5:
                renderer->ZoomOut();
                break;

            default:
                break;
            }
        }//mouse button press
        break;

        case ConfigureNotify:
            // XResizeWindow( dpy, win, screen_x, screen_y );

        case Expose:
            break;

        }

    }

#endif
    prev_time= current_time;
    current_time= clock();
    level->PhysTick();
    MovePlayer();
    if( sound_system != NULL )
    {
        sound_system->SetListenerPos( player->Position() );
        sound_system->SetListenerAngle( player->AngXY() );
          sound_system->Tick();
    }

    renderer->Draw();
#ifdef MW_OS_WINDOWS
    ::SwapBuffers(hdc);
#else
    glXSwapBuffers(dpy, win);
#endif


    usleep(1000);//1 ms

}


#ifndef KEY
#define KEY(x) (65 + x - 'A' )
#endif
void MainLoop::MovePlayer()
{
    static unsigned int prev_time= 0;
    unsigned int dt= clock() - prev_time;
    float f_dt= float( dt ) / float( CLOCKS_PER_SEC );

    prev_time+= dt;

    float move_vec[3]= { 0.0f, 0.0f, 0.0f };

    const float speed_xy= 1.0f;
    const float speed_z= 1.0f;

    if( keys[ KEY( key_forward ) ] )
        move_vec[1]+= speed_xy;
    if( keys[ KEY( key_back ) ] )
        move_vec[1]-= speed_xy;
    if( keys[ KEY( key_left ) ] )
        move_vec[0]-= speed_xy;
    if( keys[ KEY( key_right ) ] )
        move_vec[0]+= speed_xy;
    if( keys[ KEY('C') ] )
        move_vec[2]-= speed_xy;
   
      

     move_vec[0]*= f_dt;
     move_vec[1]*= f_dt;
     move_vec[2]*= f_dt;

     float rot_mat[16];
     
     Mat4RotateXY( rot_mat, player->AngXY()  );

     Vec3Mat4Mul( move_vec, rot_mat );

     player->Accelerate( move_vec );

     const float rot_speed_xy= 0.2f * mouse_speed;
     const float rot_speed_z= 0.2f * mouse_speed;

     if( keys[ VK_UP ] )
         player->RotateZ( f_dt * rot_speed_z );
     if( keys[ VK_DOWN ] )
         player->RotateZ( -f_dt * rot_speed_z );
     if( keys[ VK_LEFT ] )
         player->RotateXY( f_dt * rot_speed_xy );
     if( keys[ VK_RIGHT ] )
         player->RotateXY( -f_dt * rot_speed_xy );

     if( use_mouse )
     {
        
#ifdef MW_OS_WINDOWS
         POINT p;
         GetCursorPos(&p);
         float dx= float( p.x - screen_x/2 ), dy= float( p.y - screen_y/2 );
         SetCursorPos( screen_x/2, screen_y/2 );
       
#else
         
#endif
         player->RotateXY( dx * (-0.005f) );
         player->RotateZ( dy * (-0.005f) );


     }

     if( mouse_keys[0] )
         player->Shot();
}
