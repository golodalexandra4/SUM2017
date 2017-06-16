/* FILE NAME: U_GROUND.C
 * PROGRAMMER: AG4
 * DATE: 16.06.2017
 * PURPOSE: Draw ground.
 */

#include <stdio.h>
#include <math.h>

#include "anim.h"
#include "units.h"

/* Animation unit representation type */
typedef struct tagag4UNIT_GROUND
{
  AG4_UNIT_BASE_FIELDS;
  ag4PRIM Ground; /* Ground model1*/
  FLT Rotate;
  VEC Pos;
} ag4UNIT_GROUND;

/* Cow drawing unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ag4UNIT_GROUND *Uni;
 *   - animation context:
 *       ag4ANIM *Ani;
 * RETURNS: None.
 */
static VOID AG4_UnitInit( ag4UNIT_GROUND *Uni, ag4ANIM *Ani )
{
  AG4_RndPrimCreatePlane( &Uni->Ground, VecSet(-50, 0, -50), VecSet(100, 0, 0), VecSet(0, 0, 100), 50, 50);
} /* End of 'AG4_UnitInit' function */

/* Cow drawing unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ag4UNIT_GROUND *Uni;
 *   - animation context:
 *       ag4ANIM *Ani;
 * RETURNS: None.
 */
static VOID AG4_UnitClose( ag4UNIT_GROUND *Uni, ag4ANIM *Ani )
{
  AG4_RndPrimFree(&Uni->Ground);
} /* End of 'AG4_UnitClose' function */


static VOID AG4_UnitResponse( ag4UNIT_GROUND *Uni, ag4ANIM *Ani )
{
  Uni->Rotate += Ani->GlobalDeltaTime * Ani->Keys[VK_LBUTTON] * Ani->Mdx * 102;
}

/* Cow drawing unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ag4UNIT_GROUND *Uni;
 *   - animation context:
 *       ag4ANIM *Ani;
 * RETURNS: None.
 */
static VOID AG4_UnitRender( ag4UNIT_GROUND *Uni, ag4ANIM *Ani )
{
  VEC tmp = Uni->Pos;
  tmp.X += Ani->Mz / 60;
  tmp.Y += Ani->Mz / 60;
  tmp.Z += Ani->Mz / 60;
  AG4_RndPrimDraw(&Uni->Ground, MatrMulMatr(MatrRotate(Uni->Rotate, VecSet(0, -1, 0)), MatrTranslate(tmp)));
}

/* Ground drawing unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (ag4UNIT *) pointer to created unit.
 */
ag4UNIT *AG4_UnitCreateGround( VOID )
{
  ag4UNIT_GROUND *Uni;

  /* Memory allocation */
  if ((Uni = (ag4UNIT_GROUND *)AG4_AnimUnitCreate(sizeof(ag4UNIT_GROUND))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)AG4_UnitInit;
  Uni->Close = (VOID *)AG4_UnitClose;
  Uni->Response = (VOID *)AG4_UnitResponse;
  Uni->Render = (VOID *)AG4_UnitRender;
  return (ag4UNIT *)Uni;
} /* End of 'AG4_UnitCreateGround' function */

/* END OF 'U_COW.C' FILE */

