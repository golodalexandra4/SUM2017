/* FILE NAME: ANIM.C
 * PROGRAMMER: AG4
 * DATE: 09.06.2017
 * PURPOSE: Creating animation library.
 */

#include "units.h"
#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>

#pragma comment(lib, "winmm")

#define AG4_MAX_UNITS 4

#define AG4_GET_JOYSTICK_AXIS(A) \
  (2.0 * (ji.dw ## A ## pos = jc.w ## A ## min) / (jc.w ## A ## max - jc.w ## A ## min - 1) - 1)

ag4ANIM AG4_Anim;
INT AG4_MouseWheel;

static UINT64
  AG4_StartTime,  /* Start program time */
  AG4_OldTime,    /* Time from program start to previous frame */
  AG4_OldTimeFPS, /* Old time FPS measurement */
  AG4_PauseTime,  /* Time during pause period */
  AG4_TimePerSec, /* Timer resolution */
  AG4_FrameCounter; /* Frames counter */

/* animation's initialisation */
VOID AG4_AnimInit( HWND hWnd )
{
  HDC hDC;
  LARGE_INTEGER t;

  memset(&AG4_Anim, 0, sizeof(ag4ANIM));
  hDC = GetDC(hWnd);
  AG4_Anim.hDC = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);

  /* Timer initialization */
  QueryPerformanceFrequency(&t);
  AG4_TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  AG4_StartTime = AG4_OldTime = AG4_OldTimeFPS = t.QuadPart;
  AG4_PauseTime = 0;
  AG4_FrameCounter = 0;

  AG4_Anim.IsPause = FALSE;
  AG4_Anim.FPS = 50;

  AG4_RndInit();
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

  AG4_Anim.NumOfUnits = 0;
  DeleteDC(AG4_Anim.hDC);
  DestroyWindow(AG4_Anim.hWnd);
}

/* change frame's size */
VOID AG4_AnimResize( INT W, INT H )
{
  HDC hDC;

  AG4_Anim.W = W;
  AG4_Anim.H = H;

  if (AG4_Anim.hFrame != NULL)
    DeleteObject(AG4_Anim.hFrame);
  hDC = GetDC(AG4_Anim.hWnd);
  AG4_Anim.hFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(AG4_Anim.hWnd, hDC);

  SelectObject(AG4_Anim.hDC, AG4_Anim.hFrame);

  AG4_RndSetProj();
}

/* copy frame */
VOID AG4_AnimCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, AG4_Anim.W, AG4_Anim.H, AG4_Anim.hDC, 0, 0, SRCCOPY);
}

/* build frame*/
VOID AG4_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER t;
  POINT pt;
  
  for (i = 0; i < AG4_Anim.NumOfUnits; i++)
    AG4_Anim.Units[i]->Response(AG4_Anim.Units[i], &AG4_Anim);

  /* clear background */
  SelectObject(AG4_Anim.hDC, GetStockObject(NULL_PEN));
  SelectObject(AG4_Anim.hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(AG4_Anim.hDC, RGB(0, 55, 0));
  Rectangle(AG4_Anim.hDC, 0, 0, AG4_Anim.W + 1, AG4_Anim.H + 1); 

  for (i = 0; i < AG4_Anim.NumOfUnits; i++)
  {
    SelectObject(AG4_Anim.hDC, GetStockObject(DC_PEN));
    SelectObject(AG4_Anim.hDC, GetStockObject(NULL_BRUSH));
    SetDCBrushColor(AG4_Anim.hDC, RGB(0, 150, 150));
    SetDCPenColor(AG4_Anim.hDC, RGB(50, 250, 150));

    AG4_Anim.Units[i]->Render(AG4_Anim.Units[i], &AG4_Anim);
  }

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

  GetKeyboardState(AG4_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    AG4_Anim.Keys[i] >>= 7;
    AG4_Anim.KeysClick[i] = AG4_Anim.Keys[i] && !AG4_Anim.KeysOld[i];
  }
  memcpy(AG4_Anim.KeysOld, AG4_Anim.Keys, 256);

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
      rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  IsFullScreen = !IsFullScreen;
} /* End of 'FlipFullScreen' function */

/* end of 'Anim.C' file '*/