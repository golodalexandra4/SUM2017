/* FILE NAME: T01EYES.C
 * PROGRAMMER: AG4
 * DATE: 01.06.2017
 * PURPOSE: Eyes drowing problem.
 */

#include <math.h> 
#include <stdlib.h> 
#include <time.h> 

#include <windows.h>

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
  INT X, Y, Xc, Yc, X1, Y1, X2, Y2, len, R, l1, l2, i;
  POINT pt;
  FLOAT t;
  static INT w, h;
  static HDC hMemDC;
  static HBITMAP hBm;

  switch (Msg)
  {
  case WM_CREATE: /* windows contecst */
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    SetTimer(hWnd, 47, 30, NULL);
    return 0;
  
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC( hWnd, hDC);

    SelectObject( hMemDC, hBm );
    SendMessage(hWnd, WM_TIMER, w, h);

    return 0;

  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      DestroyWindow(hWnd);
    return 0;
  
  case WM_TIMER:
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    srand(30);

    Rectangle(hMemDC, 0, 0, w, h);

    t = clock();
    
    for (i = 0; i < 39; i++)
    {
      /* eye drawing */
      X1 = rand() % 1000;
      X2 = X1 + 200 - i * 5;
      Y1 = rand() % 1000;
      Y2 = Y1 + 200 - i * 5;
      Ellipse(hMemDC, X1, Y1, X2, Y2);

      SelectObject(hMemDC, GetStockObject(BLACK_BRUSH));

      Xc = (X1 + X2) / 2;
      Yc = (Y1 + Y2) / 2;
      R = (X2 - X1) / 2;
      l1 = pt.x - Xc;
      l2 = pt.y - Yc;
      len = sqrt(l1 * l1 + l2 * l2);
      X = 2 * l1 * R / (3 * len);
      Y = 2 * l2 * R / (3 * len);
      Ellipse(hMemDC, Xc + X - (X2 - X1) / 6, Yc + Y - (X2 - X1) / 6, Xc + X + (X2 - X1) / 6, Yc + Y + (X2 - X1) / 6);
      
      SelectObject(hMemDC, GetStockObject(WHITE_BRUSH));
    }
    
    InvalidateRect( hWnd, NULL, FALSE );
    return 0;

  case WM_PAINT:
    hDC = BeginPaint( hWnd, &ps );
    BitBlt( hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY );
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

