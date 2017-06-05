/* FILE NAME: T03POLE.C
 * PROGRAMMER: AG4
 * DATE: 05.06.2017
 * PURPOSE: Pole drowing problem.
 */

#include <math.h> 
#include <stdlib.h> 
#include <time.h> 
#include <string.h>

#include <windows.h>

VOID ClockHandDrowing (HDC hMemDC, INT X0, INT Y0, INT x, INT y)
{
  POINT p[3];
  INT sw, lh, l1, l2;
  FLOAT len, cosinus, sinus;
  HBRUSH hBr, hBrOld;
  sw = 20;
  lh = 80;
  l1 = x - X0;
  l2 = - y + Y0;
  len = sqrt(l1 * l1 + l2 * l2);
  cosinus = l1 / len;
  sinus = l2 / len;

  hBr = CreateSolidBrush(RGB(255, 0, 0));
  hBrOld = SelectObject(hMemDC, hBr);

  p[0].x = X0 + sw * sinus;
  p[0].y = Y0 + sw * cosinus;
  p[1].x = X0 + lh * cosinus;
  p[1].y = Y0 - lh * sinus;
  p[2].x = X0 - sw * sinus;
  p[2].y = Y0 - sw * cosinus;
  Polygon(hMemDC, p, 3);
  
  SelectObject(hMemDC, hBrOld);
  DeleteObject(hBr);

  hBr = CreateSolidBrush(RGB(0, 0, 255));
  hBrOld = SelectObject(hMemDC, hBr);

  p[0].x = X0 - sw * sinus;
  p[0].y = Y0 - sw * cosinus;
  p[1].x = X0 - lh * cosinus;
  p[1].y = Y0 + lh * sinus;
  p[2].x = X0 + sw * sinus;
  p[2].y = Y0 + sw * cosinus;
  Polygon(hMemDC, p, 3);

  SelectObject(hMemDC, hBrOld);
  DeleteObject(hBr);
}

