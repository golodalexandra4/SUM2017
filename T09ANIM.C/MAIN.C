/* FILE NAME: MAIN.C
 * PROGRAMMER: AG4
 * DATE: 09.06.2017
 * PURPOSE: Creating animation library.
 */

#include <stdio.h>
#include "units.h"

#define MY_WND_CLASS_NAME "My first window"

INT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, 
                           WPARAM wParam, LPARAM lParam );

/* programs main function */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd ) 
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  wc.style = CS_VREDRAW | CS_HREDRAW; /* window style */
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MyWindowFunc;
  wc.lpszClassName = MY_WND_CLASS_NAME;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
      return 0;
  }

  /* creating window */
  hWnd = CreateWindow( MY_WND_CLASS_NAME, "Something strange",
                       WS_OVERLAPPEDWINDOW,
                       0, 0,
                       CW_USEDEFAULT, CW_USEDEFAULT,
                       NULL, NULL, hInstance, NULL );

  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);


  /* Add units */
  AG4_AnimAddUnit(AG4_UnitCreateCow());
  AG4_AnimAddUnit(AG4_UnitCreateControl());


  while (TRUE)
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
    {
      AG4_AnimRender();
      AG4_AnimCopyFrame();
    }

  return msg.wParam;
}

INT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, 
                           WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  static INT w, h, Mode = 0;
  static HDC hMemDC;
  static HBITMAP hBm;
  MINMAXINFO *MinMax;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    MinMax = (MINMAXINFO *)lParam;
    MinMax->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) +
      GetSystemMetrics(SM_CYMENU) +
      GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;

  case WM_MOUSEWHEEL:
    AG4_MouseWheel += (SHORT)HIWORD(wParam);
    return 0;

  case WM_CREATE: /* windows contecst */
    AG4_AnimInit(hWnd);
    SetTimer(hWnd, 47, 30, NULL);
    return 0;
  
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    AG4_AnimResize(w, h);
    AG4_AnimRender();
    return 0;

  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      DestroyWindow(hWnd);
    if (wParam == 'F')
      AG4_FlipFullScreen(hWnd);
    return 0;
  
  case WM_TIMER:
    AG4_AnimRender();
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    AG4_AnimCopyFrame();
    EndPaint(hWnd, &ps);
    return 0;

  case WM_ERASEBKGND:
    return 1;

  case WM_DESTROY: /* delete creating objects */
    AG4_AnimClose();
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

/* end of 'MAIN.C' file '*/