/* FILE NAME: ANIM.C
 * PROGRAMMER: AG4
 * DATE: 09.06.2017
 * PURPOSE: Creating animation library.
 */

#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include "units.h"
#include "anim.h"

#pragma comment(lib, "winmm")

#define AG4_MAX_UNITS 1000

#define AG4_GET_JOYSTICK_AXIS(A) \
  (2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / (jc.w ## A ## max - jc.w ## A ## min - 1) - 1)

ag4ANIM AG4_Anim;
INT AG4_MouseWheel;

static UINT64
  AG4_StartTime,    /* Start program time */
  AG4_OldTime,      /* Time from program start to previous frame */
  AG4_OldTimeFPS,   /* Old time FPS measurement */
  AG4_PauseTime,    /* Time during pause period */
  AG4_TimePerSec,   /* Timer resolution */
  AG4_FrameCounter; /* Frames counter */

/* animation's initialisation */
BOOL AG4_AnimInit( HWND hWnd )
{
  LARGE_INTEGER t;
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};

  memset(&AG4_Anim, 0, sizeof(ag4ANIM));

  AG4_Anim.hWnd = hWnd;
  AG4_Anim.hDC = GetDC(hWnd);

  /* Timer initialization */
  QueryPerformanceFrequency(&t);
  AG4_TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  AG4_StartTime = AG4_OldTime = AG4_OldTimeFPS = t.QuadPart;
  AG4_PauseTime = 0;
  AG4_FrameCounter = 0;

  /* Store window and create memory device context */
  AG4_Anim.IsPause = FALSE;
  AG4_Anim.FPS = 50;

  /* OpenGL initialization: setup pixel format */
  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(AG4_Anim.hDC, &pfd);
  DescribePixelFormat(AG4_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(AG4_Anim.hDC, i, &pfd);

  /* OpenGL initialization: setup rendering context */
  AG4_Anim.hGLRC = wglCreateContext(AG4_Anim.hDC);
  wglMakeCurrent(AG4_Anim.hDC, AG4_Anim.hGLRC);
  if (glewInit() != GLEW_OK || !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(AG4_Anim.hGLRC);
    ReleaseDC(AG4_Anim.hWnd, AG4_Anim.hDC);
    memset(&AG4_Anim, 0, sizeof(ag4ANIM));
    return FALSE;
  }
 
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.3, 0.5, 0.7, 1);

  AG4_RndInit();
  return TRUE;
}

/* animation's deinitialisation */
VOID AG4_AnimClose( VOID )
{
  INT i;
  
  for (i = 0; i < AG4_Anim.NumOfUnits; i++)
  {  
    AG4_Anim.Units[i]->Close(AG4_Anim.Units[i], &AG4_Anim);
    free(AG4_Anim.Units[i]);
  }

  AG4_RndClose();

  /* Delete OpenGL data */
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(AG4_Anim.hGLRC);
  /* Delete GDI data */
  ReleaseDC(AG4_Anim.hWnd, AG4_Anim.hDC);
  AG4_Anim.NumOfUnits = 0;
}

/* change frame's size */
VOID AG4_AnimResize( INT W, INT H )
{
  AG4_Anim.W = W;
  AG4_Anim.H = H;

  glViewport(0, 0, W, H);

  AG4_RndSetProj();
}

/* copy frame */
VOID AG4_AnimCopyFrame( VOID )
{
  SwapBuffers(AG4_Anim.hDC);
}

/* build frame*/
VOID AG4_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER t;
  POINT pt;
  static FLT ShdTime;
  
  /*** Handle timer ***/
  AG4_FrameCounter++;                    /* increment frame counter (for FPS) */
  QueryPerformanceCounter(&t);           /* obtain current timer value */
  /* Global time */
  AG4_Anim.GlobalTime = (DBL)(t.QuadPart - AG4_StartTime) / AG4_TimePerSec;
  AG4_Anim.GlobalDeltaTime = (DBL)(t.QuadPart - AG4_OldTime) / AG4_TimePerSec;
  /* Time with pause */
  if (AG4_Anim.IsPause)
  {
    AG4_PauseTime += t.QuadPart - AG4_OldTime;
    AG4_Anim.DeltaTime = 0;
  }
  else
  {
    AG4_Anim.Time = (DBL)(t.QuadPart - AG4_PauseTime - AG4_StartTime) / AG4_TimePerSec;
    AG4_Anim.DeltaTime = AG4_Anim.GlobalDeltaTime;
  }
  /* FPS */
  if (t.QuadPart - AG4_OldTimeFPS > AG4_TimePerSec)
  {
    AG4_Anim.FPS = (DBL)AG4_FrameCounter * AG4_TimePerSec / (t.QuadPart - AG4_OldTimeFPS);
    AG4_OldTimeFPS = t.QuadPart;
    AG4_FrameCounter = 0;
  }
  AG4_OldTime = t.QuadPart;

  /* Keyboard */
  GetKeyboardState(AG4_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    AG4_Anim.Keys[i] >>= 7;
    AG4_Anim.KeysClick[i] = AG4_Anim.Keys[i] && !AG4_Anim.KeysOld[i];
  }
  memcpy(AG4_Anim.KeysOld, AG4_Anim.Keys, 256);

  /* Mouse */
  GetCursorPos(&pt);
  ScreenToClient(AG4_Anim.hWnd, &pt);
  AG4_Anim.Mdx = pt.x - AG4_Anim.Mx;
  AG4_Anim.Mdy = pt.y - AG4_Anim.My;
  AG4_Anim.Mx = pt.x;
  AG4_Anim.My = pt.y;

  AG4_Anim.Mdz = AG4_MouseWheel;
  AG4_Anim.Mz += AG4_MouseWheel;
  AG4_MouseWheel = 0;

  /*** Joystick handle ***/
  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;

    /* Get joystick info */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(JOYCAPS)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Axes */
        AG4_Anim.Jx = AG4_GET_JOYSTICK_AXIS(X);
        AG4_Anim.Jy = AG4_GET_JOYSTICK_AXIS(Y);
        AG4_Anim.Jz = AG4_GET_JOYSTICK_AXIS(Z);
        AG4_Anim.Jr = AG4_GET_JOYSTICK_AXIS(R);

        /* Buttons */
        for (i = 0; i < 32; i++)
        {
          AG4_Anim.JBut[i] = (ji.dwButtons >> i) & 1;
          AG4_Anim.JButClick[i] = AG4_Anim.JBut[i] && !AG4_Anim.JButOld[i];
          memcpy(AG4_Anim.JButOld, AG4_Anim.JBut, 32);
        }

        /* Point of view */
        AG4_Anim.JPov = ji.dwPOV == 0xFFFF ? 0 : ji.dwPOV / 4500 + 1;
      }
    }
  }

  for (i = 0; i < AG4_Anim.NumOfUnits; i++)
    AG4_Anim.Units[i]->Response(AG4_Anim.Units[i], &AG4_Anim);


  /*** Update shader ***/
  if (AG4_Anim.GlobalTime - ShdTime > 2)
  {
    INT i;

    for (i = 0; i < AG4_RndNumOfShaders; i++)
    {
      AG4_RndShaderFree(AG4_RndShaders[i].ProgId);
      AG4_RndShaders[i].ProgId = AG4_RndShaderLoad(AG4_RndShaders[i].Name);
    }
    ShdTime = AG4_Anim.GlobalTime;
  }

  /*** Clear frame ***/
  /* Clear background */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (i = 0; i < AG4_Anim.NumOfUnits; i++)
  {
    AG4_Anim.Units[i]->Render(AG4_Anim.Units[i], &AG4_Anim);
  }

  glFinish();
}

/* close animation */
VOID AG4_AnimDoExit( VOID )
{
  static BOOL IsExist = FALSE;

  if(IsExist)
    return;
  IsExist = TRUE;
  PostMessage(AG4_Anim.hWnd, WM_CLOSE, 0, 0);
}

/* add new animation object*/
VOID AG4_AnimAddUnit( ag4UNIT *Uni )
{
  if(AG4_Anim.NumOfUnits < AG4_MAX_UNITS)
  {
    AG4_Anim.Units[AG4_Anim.NumOfUnits] = Uni;
    AG4_Anim.Units[AG4_Anim.NumOfUnits]->Init(AG4_Anim.Units[AG4_Anim.NumOfUnits], &AG4_Anim);
    AG4_Anim.NumOfUnits++;
  }
}

/* Set/reset full screen mode function */
VOID AG4_FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;

  if (IsFullScreen)
  {
    /* restore window size */
    SetWindowPos(hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
  }
  else
  {
    /* Set full screen size to window */
    HMONITOR hmon;
    MONITORINFOEX moninfo;
    RECT rc;

    /* Store window old size */
    GetWindowRect(hWnd, &SaveRect);

    /* Get nearest monitor */
    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

    /* Obtain monitor info */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* Set window new size */
    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    SetWindowPos(hWnd, HWND_TOPMOST,
      rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOOWNERZORDER);
  }
  IsFullScreen = !IsFullScreen;
} /* End of 'FlipFullScreen' function */

/* end of 'Anim.C' file '*/