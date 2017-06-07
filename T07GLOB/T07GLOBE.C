/* FILE NAME: T07GLOBE.C
 * PROGRAMMER: AG4
 * DATE: 07.06.2017
 * PURPOSE: Globus drowing problem.
 */

#include <windows.h>

#include "SPHERA.h"

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
  hWnd = CreateWindow( MY_WND_CLASS_NAME, "Hello world",
                       WS_OVERLAPPEDWINDOW,
                       0, 0,
                       CW_USEDEFAULT, CW_USEDEFAULT,
                       NULL, NULL, hInstance, NULL );

  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
      DispatchMessage(&msg);
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

  switch (Msg)
  {
  case WM_CREATE: /* windows contecst */
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    hBm = NULL;

    SetTimer(hWnd, 47, 30, NULL);

    return 0;
  
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);

    ReleaseDC(hWnd, hDC);

    SelectObject( hMemDC, hBm );
    SendMessage(hWnd, WM_TIMER, w, h);

    return 0;

  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      DestroyWindow(hWnd);
    return 0;
  
  case WM_TIMER:
    Rectangle(hMemDC, 0, 0, w, h);

    BuildSphera( hMemDC, 0.3 );
    SphereDraw( hMemDC, w, h );

    InvalidateRect(hWnd, NULL, FALSE);
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;

  case WM_ERASEBKGND:
    return 1;

  case WM_DESTROY: /* delete creating objects */
    DeleteObject(hBm);
    DeleteDC(hMemDC);
    KillTimer(hWnd, 47);
    PostQuitMessage(0);
  
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}