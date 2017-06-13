/* FILE NAME: RENDER.C
 * PROGRAMMER: AG4
 * DATE: 13.06.2017
 * PURPOSE: Base rendering system module.
 */

#include "anim.h"

/* Project parameters */
DBL
  AG4_RndWp,       /* Project plane width */
  AG4_RndHp,       /* Project plane height */
  AG4_RndProjDist, /* Distance from viewer to project plane */
  AG4_RndProjSize; /* Prohect plane inner size */

MATR
  AG4_RndMatrView; /* Viewer matrix */

/* Rendering system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AG4_RndInit( VOID )
{
  AG4_RndWp = 1;
  AG4_RndHp = 1;
  AG4_RndProjDist = 1;
  AG4_RndProjSize = 1;

  AG4_RndMatrView = MatrView(VecSet1(23), VecSet1(0), VecSet(0, 1, 0));
} /* End of 'VG4_RndInit' function */

/* Project parameters adjust function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AG4_RndSetProj( VOID )
{
  AG4_RndWp = AG4_RndProjSize;
  AG4_RndHp = AG4_RndProjSize;
  if (AG4_Anim.W > AG4_Anim.H)
    AG4_RndWp *= (DBL)AG4_Anim.W / AG4_Anim.H;
  else
    AG4_RndHp *= (DBL)AG4_Anim.H / AG4_Anim.W;
} /* End of 'VG4_RndSetProj' function */

/* END OF 'RENDER.C' FILE */