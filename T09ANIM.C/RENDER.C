/* FILE NAME: RENDER.C
 * PROGRAMMER: AG4
 * DATE: 13.06.2017
 * PURPOSE: Base rendering system module.
 */

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
#pragma comment(lib, "glew32s")

#include "anim.h"

/* Project parameters */
FLT
  AG4_RndProjSize = 1, /* Prohect plane inner size */
  AG4_RndProjDist = 1, /* Distance from viewer to project plane */
  AG4_RndProjFarClip = 1000; /*  */

MATR
  AG4_RndMatrView, /* Viewer matrix */
  AG4_RndMatrProj; /* Projection matrix */

VEC AG4_RndLightPos;
VEC AG4_RndLightColor;

/* Rendering system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AG4_RndInit( VOID )
{
  AG4_RndMatrView = MatrView(VecSet1(20), VecSet1(0), VecSet(0, 1, 0));
  AG4_RndLightPos = VecSet(-100, 20, 0);
  AG4_RndLightColor = VecSet(1, 1, 1);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.5, 0.5, 0.5, 1);

  AG4_RndResInit();
} /* End of 'AG4_RndInit' function */

/* Rendering system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AG4_RndClose( VOID )
{
  AG4_RndResClose();
} /* End of 'AG4_RndClose' function */

/* Project parameters adjust function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AG4_RndSetProj( VOID )
{
  FLT rx = AG4_RndProjSize / 2, ry = AG4_RndProjSize / 2;

  if (AG4_Anim.W > AG4_Anim.H)
    rx *= (FLT)AG4_Anim.W / AG4_Anim.H;
  else
    ry *= (FLT)AG4_Anim.H / AG4_Anim.W;

  AG4_RndMatrProj = MatrFrustum(-rx, rx, -ry, ry, AG4_RndProjDist, AG4_RndProjFarClip);
} /* End of 'AG4_RndSetProj' function */

/* Current shader */
UINT
  AG4_RndProgId;    /* Shader program identifier */

/* END OF 'RENDER.C' FILE */