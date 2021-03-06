/* FILE NAME: T02CLOCK.C
 * PROGRAMMER: AG4
 * DATE: 02.06.2017
 * PURPOSE: Clock drowing problem.
 */

#include <math.h> 
#include <stdlib.h> 
#include <time.h> 
#include <string.h>

#include <windows.h>

#define MY_WND_CLASS_NAME "My first window"

INT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, 
                           WPARAM wParam, LPARAM lParam );
VOID ClockHandDrowing (POINT p[4], FLOAT a, INT sh, INT sw, INT lh, INT X0, INT Y0, BITMAP bm); /* Clock-hand drowing */

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
  BITMAP bm;
  INT Y0, X0, sh, sw, lh, i, day, month, year;
  PAINTSTRUCT ps;
  POINT pt, p[4];
  FLOAT sec, min, hour;
  SYSTEMTIME st;
  CHAR Str[100] = "Hello world";
  static INT w, h;
  static HDC hMemDC, hMemDCLogo;
  static HBITMAP hBm, hBmLogo, hBmXOR, hBmAND;
  static HFONT hFnt;

  switch (Msg)
  {
  case WM_CREATE: /* windows contecst */
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCLogo = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    hBm = NULL;

    SetTimer(hWnd, 47, 30, NULL);
    hBmLogo = LoadImage(NULL, "WALL.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmAND = LoadImage(NULL, "CLOCK.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmXOR = LoadImage(NULL, "XOR.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SelectObject(hMemDCLogo, hBmLogo);

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
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    srand(30);

    Rectangle(hMemDC, 0, 0, w, h);

    /* Background drowing */
    SelectObject(hMemDCLogo, hBmLogo);
    GetObject(hBmLogo, sizeof(BITMAP), &bm);
    StretchBlt(hMemDC, 0, 0, w, h, hMemDCLogo, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
    SetStretchBltMode(hMemDC, COLORONCOLOR);

    GetLocalTime(&st);

    day = st.wDay;
    month = st.wMonth;
    year = st.wYear;

    /* Text drowing */
    hFnt = CreateFont(75, 0, 0, 0, FW_BOLD, TRUE, FALSE, FALSE, 
                      RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
                      PROOF_QUALITY, VARIABLE_PITCH | FF_ROMAN, "");
    SelectObject(hMemDC, hFnt);
    SetTextColor(hMemDC, RGB(0, 0, 0));
    SetBkMode(hMemDC, TRANSPARENT);
    TextOut(hMemDC, (w - bm.bmWidth) / 2 + 75, (h - bm.bmHeight) / 2 + 230, Str, sprintf(Str, "%i. %i. %i", day, month, year));

    sec = (st.wSecond / 30.0) * 3.1415;
    min = (st.wMinute / 30.0) * 3.1415 + sec / 60.0;
    hour = (st.wHour / 30.0) * 3.1415 + min / 60.0; 
    X0 = (w - bm.bmWidth) / 2 + 100; 
    Y0 = (h - bm.bmHeight) / 2; 

    GetObject(hBmXOR, sizeof(BITMAP), &bm);

    /* Clock drowing */
    SelectObject(hMemDCLogo, hBmAND);
    BitBlt(hMemDC, X0, Y0, bm.bmWidth, bm.bmHeight, hMemDCLogo, 0, 0, SRCAND);
    SelectObject(hMemDCLogo, hBmXOR);
    BitBlt(hMemDC, X0, Y0, bm.bmWidth, bm.bmHeight, hMemDCLogo, 0, 0, SRCINVERT);
    
    sh = 20;
    sw = 10;
    lh = 60;

    ClockHandDrowing (&p, min, sh, sw, lh, X0, Y0, bm); /* Min. clock-hand drowing */
    Polygon(hMemDC, p, 4);

    ClockHandDrowing (&p, hour, sh / 2, sw, lh / 2, X0, Y0, bm); /* Hour clock-hand drowing */
    Polygon(hMemDC, p, 4);

    ClockHandDrowing (&p, sec, sh, 0, lh, X0, Y0, bm); /* Sec. clock-hand drowing */
    Polygon(hMemDC, p, 4);

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
    DeleteObject(hBmLogo);
    DeleteObject(hBmXOR);
    DeleteObject(hBmAND);
    DeleteDC(hMemDCLogo);
    KillTimer(hWnd, 47);
    PostQuitMessage(0);
    DeleteObject(hFnt);
  
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

VOID ClockHandDrowing (POINT p[4], FLOAT a, INT sh, INT sw, INT lh, INT X0, INT Y0, BITMAP bm)
{
  p[0].x = X0 + bm.bmWidth / 2 + sh * sin(a);
  p[0].y = Y0 + bm.bmHeight / 2 - sh * cos(a);
  p[1].x = X0 + bm.bmWidth / 2 - sw * cos(a);
  p[1].y = Y0 + bm.bmHeight / 2 - sw * sin(a);
  p[2].x = X0 + bm.bmWidth / 2 - lh * sin(a);
  p[2].y = Y0 + bm.bmHeight / 2 + lh * cos(a);
  p[3].x = X0 + bm.bmWidth / 2 + sw * cos(a);
  p[3].y = Y0 + bm.bmHeight / 2 + sw * sin(a);
}