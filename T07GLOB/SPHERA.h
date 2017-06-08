/* FILE NAME: T07GLOBE.C
 * PROGRAMMER: AG4
 * DATE: 07.06.2017
 * PURPOSE: Globus drowing problem.
 */

typedef double DBL;

typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;

/* Matrix representation type */
typedef struct tagMATR
{
  DBL A[3][3];
} MATR;


VOID BuildSphera( HDC hDC, DBL R );
VOID SphereDraw( HDC hDC, DBL W, DBL H );
MATR MatrRotate( DBL AngleInDegree, VEC R );
VEC VecTransForm( VEC V, MATR m );

