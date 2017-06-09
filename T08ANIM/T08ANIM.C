/* FILE NAME: T08ANIM.C
 * PROGRAMMER: AG4
 * DATE: 09.06.2017
 * PURPOSE: Creating animation library.
 */

#include <windows.h>
#include <math.h>
#include <stdio.h>
#include "VEC.h"

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd ) 
{
  MATR m = UnitMatrix;
  MATR M = {{{0, 0, 0, 1},{0, 0, 1, 0},{0, 1, 0, 0},{1, 0, 0, 0}}};
  m = MatrMulMatr(m, M);
  return 0;
}
