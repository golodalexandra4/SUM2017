/* FILE NAME: T07GLOBE.C
 * PROGRAMMER: AG4
 * DATE: 07.06.2017
 * PURPOSE: Globus drowing problem.
 */

#include <math.h>
#include <time.h>
#include <windows.h>

#include "SPHERA.h"

#pragma warning (disable: 4244)

#define N 30
#define M 30
#define PI 3.14159265358979323846

static VEC G[N][M];

/* Sphere building function */
VOID BuildSphera( HDC hDC, DBL R )
{
  DBL theta, phi;
  INT i, j;
  
  for (theta = 0, i = 0; i < N; i++, theta += PI / (N - 1))
    for (phi = 0, j = 0; j < M; j++, phi += 2 * PI / (M - 1))
    {
      G[i][j].X = (DBL)R * sin(theta) * sin(phi);
      G[i][j].Y = (DBL)R * cos(theta);
      G[i][j].Z = (DBL)R * sin(theta) * cos(phi); 
    }
}

/* Sphere drawing function */
VOID SphereDraw( HDC hDC, DBL W, DBL H )
{
  DBL size = 1, rx = size, ry = size, Wp, Hp, Xp, Yp, t;
  INT i, j;
  POINT pts[N][M], polegon[4];
  VEC P = {1, 1, 1};
  HBRUSH hBr, hBrOld;

  if (W > H)
    rx *= (DBL)W / H;
  else
    ry *= (DBL)H / W;
  Wp = rx;
  Hp = ry;
  
  t = clock();

  for (i = 0; i < N; i++)
    for (j = 0; j < M; j++)
    {                           
      MATR m = MatrRotate(t / 30, P);
      VEC V = VecTransForm(G[i][j], m);;
      
      Xp = V.X * cos(t / 1000) - V.Y * sin(t / 1000);
      Yp = V.Y * cos(t / 1000) + V.X * sin(t / 1000);
      pts[i][j].x = W / 2 + Xp * W / Wp;
      pts[i][j].y = H / 2 - Yp * H / Hp;
    }

  for (i = 1; i < N; i++)
    for (j = 1; j < M; j++)
    {   
      /* Ellipse(hDC, pts[i][j].x - 3, pts[i][j].y - 3, pts[i][j].x + 3, pts[i][j].y + 3); */
     
      polegon[0] = pts[i][j];
      polegon[1] = pts[i - 1][j];
      polegon[2] = pts[i - 1][j - 1];
      polegon[3] = pts[i][j - 1];
  
      if ((polegon[0].x - polegon[1].x) * (polegon[0].y + polegon[1].y) + 
          (polegon[1].x - polegon[2].x) * (polegon[1].y + polegon[2].y) + 
          (polegon[2].x - polegon[3].x) * (polegon[2].y + polegon[3].y) + 
          (polegon[3].x - polegon[0].x) * (polegon[3].y + polegon[0].y) < 0)
        continue;

      hBr = CreateSolidBrush(RGB(rand() & 255, rand() & 255, rand() & 255));
      hBrOld = SelectObject(hDC, hBr);
      
      Polygon(hDC, polegon, 4);
      
      SelectObject(hDC, hBrOld);
      DeleteObject(hBr);
    }
}

MATR MatrRotate( DBL AngleInDegree, VEC R )
{
  DBL a = AngleInDegree * PI / 180, sine = sin(a), cosine = cos(a);
  MATR m;

  a = sqrt(R.X * R.X + R.Y * R.Y + R.Z * R.Z);
  R.X /= a;
  R.Y /= a;
  R.Z /= a;

  m.A[0][0] = cosine + R.X * R.X * (1 - cosine);
  m.A[0][1] = R.X * R.Y * (1 - cosine) - R.Z * sine;
  m.A[0][2] = R.X * R.Z * (1 - cosine) + R.Y * sine;

  m.A[1][0] = R.Y * R.X * (1 - cosine) + R.Z * sine;
  m.A[1][1] = cosine + R.Y * R.Y * (1 - cosine);
  m.A[1][2] = R.Y * R.Z * (1 - cosine) - R.X * sine;

  m.A[2][0] = R.Z * R.X * (1 - cosine) - R.Y * sine;
  m.A[2][1] = R.Z * R.Y * (1 - cosine) + R.X * sine;
  m.A[2][2] = cosine + R.Z * R.Z * (1 - cosine);
  
  return m;
} /* End of 'MatrRotate' function */

/* Vector transform function */
VEC VecTransForm( VEC V, MATR m )
{
  VEC P;

  P.X = V.X * m.A[0][0] + V.Y * m.A[0][1] + V.Z * m.A[0][2];
  P.Y = V.X * m.A[1][0] + V.Y * m.A[1][1] + V.Z * m.A[1][2];
  P.Z = V.X * m.A[2][0] + V.Y * m.A[2][1] + V.Z * m.A[2][2];

  return P;
} /* End of 'VecTransform' function */

