/* FILE NAME: VEC.H
 * PROGRAMMER: AG4
 * DATE: 09.06.2017
 * PURPOSE: Creating animation library.
 */

#ifndef __VEC_H_
#define __VEC_H_

#include <math.h>
#include <windows.h>

#define PI 3.14159265358979323946
#define D2R(A) ((A) * (PI / 180.0))
#define Degree2Radian(a) D2R(a)

/* base float type */
typedef double DBL;
/* type for vector in space */
typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;
/* type for matrix */
typedef struct tagMATR
{
  DBL M[4][4];
} MATR;

static MATR UnitMatrix = 
{
  {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  }
};

__inline VEC VecSet( DBL X, DBL Y, DBL Z)
{
  VEC v;

  v.X = X;
  v.Y = Y;
  v.Z = Z;

  return v;
}

__inline VEC VecSet1( DBL C )
{
  VEC v;

  v.X = C;
  v.Y = C;
  v.Z = C;

  return v;
}

__inline MATR MatrIdentity( VOID )
{
  return UnitMatrix;
}

__inline MATR MatrTranslate( VEC T )
{
  MATR m = UnitMatrix;

  m.M[3][0] = T.X;
  m.M[3][1] = T.Y;
  m.M[3][2] = T.Z;
  return m;
}


__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet( V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
}

__inline VEC VecSubVec( VEC V1, VEC V2 )
{
  return VecSet( V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z);
}

__inline VEC VecMulNum( VEC V1, DBL N )
{
  return VecSet( V1.X * N, V1.Y * N, V1.Z * N);
}

__inline VEC VecDivNum( VEC V1, DBL N )
{
  return VecSet( V1.X / N, V1.Y / N, V1.Z / N);
}

__inline VEC VecNeg( VEC V )
{
  return VecSet( - V.X , - V.Y, - V.Z);
}

/* dot prodaction */
__inline DBL VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
}

/* croos prodaction */
__inline VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.Y * V2.Z - V2.Y * V1.Z, V1.Z * V2.X - V1.X * V2.Z, V1.X * V2.Y - V2.X * V1.Y);
}

__inline DBL VecLen2( VEC V )
{
  return V.X * V.X + V.Y * V.Y + V.Z * V.Z;
}

__inline DBL VecLen( VEC V )
{
  return sqrt(VecLen2(V));
}

__inline VEC VecNormalize( VEC V )
{
  DBL len = VecLen(V);

  return VecSet( V.X / len, V.Y / len, V.Z / len);
}

/* Vector by matrix multiplication (without projection) function.
 * ARGUMENTS:
 *   - source vector:
 *       VEC V;
 *   - multiplied matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecMulMatr43( VEC V, MATR M )
{
  return VecSet(V.X * M.M[0][0] + V.Y * M.M[1][0] + V.Z * M.M[2][0] + M.M[3][0],
                V.X * M.M[0][1] + V.Y * M.M[1][1] + V.Z * M.M[2][1] + M.M[3][1],
                V.X * M.M[0][2] + V.Y * M.M[1][2] + V.Z * M.M[2][2] + M.M[3][2]);
} /* End of 'VecMulMatr43' function */

__inline VEC VecAddVecEq( VEC *VRes, VEC V2 )
{
  VRes->X += V2.X;
  VRes->Y += V2.Y;
  VRes->Z += V2.Z;

  return *VRes;
}

/* Vector by matrix multiplication (only orientation) function.
 * ARGUMENTS:
 *   - source vector:
 *       VEC V;
 *   - multiplied matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecMulMatr3( VEC V, MATR M )
{
  return VecSet(V.X * M.M[0][0] + V.Y * M.M[1][0] + V.Z * M.M[2][0],
                V.X * M.M[0][1] + V.Y * M.M[1][1] + V.Z * M.M[2][1],
                V.X * M.M[0][2] + V.Y * M.M[1][2] + V.Z * M.M[2][2]);
} /* End of 'VecMulMatr3' function */

/* Vector by matrix multiplication (with homogenious devide) function.
 * ARGUMENTS:
 *   - source vector:
 *       VEC V;
 *   - multiplied matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC PointTransform( VEC V, MATR M )
{
  DBL w = V.X * M.M[0][3] + V.Y * M.M[1][3] + V.Z * M.M[2][3] + M.M[3][3];

  return VecSet((V.X * M.M[0][0] + V.Y * M.M[1][0] + V.Z * M.M[2][0] + M.M[3][0]) / w,
                (V.X * M.M[0][1] + V.Y * M.M[1][1] + V.Z * M.M[2][1] + M.M[3][1]) / w,
                (V.X * M.M[0][2] + V.Y * M.M[1][2] + V.Z * M.M[2][2] + M.M[3][2]) / w);
} /* End of 'PointTransform' function */


__inline MATR MatrRotateX( DBL AngleInDegree )
{
  DBL a = D2R(AngleInDegree);
  MATR m = UnitMatrix;

  m.M[1][1] = cos(a);
  m.M[1][2] = sin(a);
  m.M[2][1] = - sin(a);
  m.M[2][2] = cos(a);

  return m;
}

__inline MATR MatrRotateY( DBL AngleInDegree )
{
  DBL a = D2R(AngleInDegree);
  MATR m = UnitMatrix;
  DBL c = cos(a), s = sin(a);

  m.M[0][0] = c;
  m.M[2][0] = s;
  m.M[0][2] = -s;
  m.M[2][2] = c;

  return m;
}

__inline MATR MatrRotateZ( DBL AngleInDegree )
{
  DBL a = D2R(AngleInDegree);
  MATR m = UnitMatrix;

  m.M[0][0] = cos(a);
  m.M[0][1] = sin(a);
  m.M[1][0] = - sin(a);
  m.M[1][1] = cos(a);

  return m;
}

__inline MATR MatrRotate( DBL AngleInDegree, VEC R )
{
  DBL a = D2R(AngleInDegree), sine = sin(a), cosine = cos(a);
  VEC V = VecNormalize(R);
  MATR M = 
  {
    {
      {cosine + V.X * V.X * (1 - cosine),
         V.X * V.Y * (1 - cosine) - V.Z * sine,
           V.X * V.Z * (1 - cosine) + V.Y * sine, 0},
      {V.Y * V.X * (1 - cosine) + V.Z * sine,
         cosine + V.Y * V.Y * (1 - cosine),
           V.Y * V.Z * (1 - cosine) - V.X * sine, 0},
      {V.Z * V.X * (1 - cosine) - V.Y * sine,
         V.Z * V.Y * (1 - cosine) + V.X * sine,
           cosine + V.Z * V.Z * (1 - cosine), 0},
      {0, 0, 0, 1}
    }
  };

  return M;
} /* End of 'MatrRotate' function */

__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  MATR r;
  INT i, j, k;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (r.M[i][j] = 0, k = 0; k < 4; k++)
        r.M[i][j] += M1.M[i][k] * M2.M[k][j];

  return r;
}

__inline MATR MatrTranspose( MATR M )
{
  INT i, j;
  MATR r;

  for(i = 0; i < 4; i++)
    for(j = 0; j < 4; j++)
      r.M[i][j] = M.M[j][i];

  return r;
}


__inline DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                   DBL A21, DBL A22, DBL A23,
                   DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 - A11 * A23 * A32 - A12 * A21 * A33 +
         A12 * A23 * A31 + A31 * A21 * A32 - A31 * A22 * A31;
}

__inline DBL MatrDeterm( MATR M )
{
  return
    M.M[0][0] * MatrDeterm3x3(M.M[1][1], M.M[1][2], M.M[1][3],
                              M.M[2][1], M.M[2][2], M.M[2][3],
                              M.M[3][1], M.M[3][2], M.M[3][3]) - 
    M.M[0][1] * MatrDeterm3x3(M.M[1][0], M.M[1][2], M.M[1][3],
                              M.M[2][0], M.M[2][2], M.M[2][3],
                              M.M[3][0], M.M[3][2], M.M[3][3]) +
    M.M[0][2] * MatrDeterm3x3(M.M[1][0], M.M[1][1], M.M[1][3],
                              M.M[2][0], M.M[2][1], M.M[2][3],
                              M.M[3][0], M.M[3][1], M.M[3][3]) - 
    M.M[0][3] * MatrDeterm3x3(M.M[1][0], M.M[1][1], M.M[1][2],
                              M.M[2][0], M.M[2][1], M.M[2][2],
                              M.M[3][0], M.M[3][1], M.M[3][2]);
}

__inline MATR MatrInverse( MATR M )
{
  MATR r;
  DBL det = MatrDeterm(M);
  INT p[4][3] = {{1, 2, 3},{0, 2, 3},{0, 1, 3},{0, 1, 2}};
  INT i, j;

  if (det == 0)
    return UnitMatrix;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.M[j][i] = 
        MatrDeterm3x3(
          M.M[p[i][0]][p[j][0]], M.M[p[i][0]][p[j][1]], M.M[p[i][0]][p[j][2]],
          M.M[p[i][1]][p[j][0]], M.M[p[i][1]][p[j][1]], M.M[p[i][1]][p[j][2]],
          M.M[p[i][2]][p[j][0]], M.M[p[i][2]][p[j][1]], M.M[p[i][2]][p[j][2]]) / det;
  return r;
}

__inline MATR MatrScale( VEC S )
{
  MATR m = UnitMatrix;

  m.M[0][0] = S.X;
  m.M[1][1] = S.Y;
  m.M[2][2] = S.Z;

  return m;
}

/* Matrix look-at viewer setup function.
 * ARGUMENTS:
 *   - viewer position, look-at point, approximate up direction:
 *       VEC Loc, At, Up1;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrView( VEC Loc, VEC At, VEC Up1 )
{
  VEC
    Dir = VecNormalize(VecSubVec(At, Loc)),
    Right = VecNormalize(VecCrossVec(Dir, Up1)),
    Up = VecNormalize(VecCrossVec(Right, Dir));
  MATR m =
  {
    {
      {               Right.X,                Up.X,              -Dir.X, 0},
      {               Right.Y,                Up.Y,              -Dir.Y, 0},
      {               Right.Z,                Up.Z,              -Dir.Z, 0},
      {-VecDotVec(Loc, Right), -VecDotVec(Loc, Up), VecDotVec(Loc, Dir), 1}
    }
  };

  return m;
} /* End of 'MatrView' function */

__inline MATR MatrFrustum( DBL left, DBL right, DBL bottom, DBL top, DBL ProgectDist, DBL FarClip)
{
  MATR M = UnitMatrix;

  M.M[0][0] = 2 * ProgectDist / (right - left);
  M.M[2][0] = (right + left) / (right - left);
  M.M[1][1] = 2 * ProgectDist / (top - bottom);
  M.M[2][1] = (top + bottom) / (top - bottom);
  M.M[2][2] = (ProgectDist + FarClip) / (ProgectDist - FarClip);
  M.M[3][2] = 2 * ProgectDist * FarClip / (ProgectDist - FarClip);
  M.M[2][3] = -1;
  M.M[3][3] = 0;

  return M;
}

#endif 

/* end of 'VEC.H' file '*/