/* FILE NAME: U_COW.C
 * PROGRAMMER: AG4
 * DATE: 10.06.2017
 * PURPOSE: Units base functions.
 */

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "units.h"

/* Animation unit representation type */
typedef struct tagag4UNIT_COW
{
  AG4_UNIT_BASE_FIELDS;
  ag4OBJ3D Cow; /* Cow model1*/
  DBL Rotate;   /* Cow rotate */
  VEC Pos;
} ag4UNIT_COW;

/* Cow drawing unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ag4UNIT_COW *Uni;
 *   - animation context:
 *       ag4ANIM *Ani;
 * RETURNS: None.
 */
static VOID AG4_UnitInit( ag4UNIT_COW *Uni, ag4ANIM *Ani )
{
  AG4_RndObjLoad(&Uni->Cow, "cow.object");
  Uni->Pos = VecSet(rand() % 20 - 10, rand() % 20 - 10, rand() % 20 - 10);
} /* End of 'VG4_UnitInit' function */

/* Cow drawing unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ag4UNIT_COW *Uni;
 *   - animation context:
 *       ag4ANIM *Ani;
 * RETURNS: None.
 */
static VOID AG4_UnitClose( ag4UNIT_COW *Uni, ag4ANIM *Ani )
{
  AG4_RndObjFree(&Uni->Cow);
} /* End of 'AG4_UnitClose' function */

/* Cow drawing unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ag4UNIT_COW *Uni;
 *   - animation context:
 *       ag4ANIM *Ani;
 * RETURNS: None.
 */
static VOID AG4_UnitResponse( ag4UNIT_COW *Uni, ag4ANIM *Ani )
{
  Uni->Rotate += Ani->GlobalDeltaTime * Ani->Keys[VK_LBUTTON] * Ani->Mdx * 102;
  Uni->Rotate += Ani->DeltaTime * Ani->Keys['A'] * 102;
  Uni->Rotate += -Ani->DeltaTime * Ani->Keys['D'] * 102;
  Uni->Pos.X += - Ani->DeltaTime * Ani->Keys[VK_LEFT] * 10 + Ani->DeltaTime * Ani->Keys[VK_RIGHT] * 10;
  Uni->Pos.Z += - Ani->DeltaTime * Ani->Keys[VK_UP] * 10 + Ani->DeltaTime * Ani->Keys[VK_DOWN] * 10;

  Uni->Pos.X += Ani->DeltaTime * Ani->Jx * 100;
  Uni->Pos.Z += Ani->DeltaTime * Ani->Jy * 100;
  Uni->Rotate += -Ani->DeltaTime * Ani->Jr * 100;
  Uni->Rotate += -Ani->DeltaTime * Ani->Jz * 100;
  
  if (Ani->JButClick[6])
  {  
    Uni->Pos.X = 0;
    Uni->Pos.Y = 0;
    Uni->Pos.Z = 0;
  }
}
/* Cow drawing unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ag4UNIT_COW *Uni;
 *   - animation context:
 *       ag4ANIM *Ani;
 * RETURNS: None.
 */
static VOID AG4_UnitRender( ag4UNIT_COW *Uni, ag4ANIM *Ani )
{
  VEC tmp = Uni->Pos;
  tmp.X += Ani->Mz / 60;
  tmp.Y += Ani->Mz / 60;
  tmp.Z += Ani->Mz / 60;
  AG4_RndObjDraw(&Uni->Cow, MatrMulMatr(MatrRotate(Uni->Rotate, VecSet(0, -1, 0)), MatrTranslate(tmp)));
} /* End of 'AG4_UnitRender' function */

/* Cow drawing unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (ag4UNIT *) pointer to created unit.
 */
ag4UNIT *AG4_UnitCreateCow( VOID )
{
  ag4UNIT_COW *Uni;

  /* Memory allocation */
  if ((Uni = (ag4UNIT_COW *)AG4_AnimUnitCreate(sizeof(ag4UNIT_COW))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)AG4_UnitInit;
  Uni->Close = (VOID *)AG4_UnitClose;
  Uni->Response = (VOID *)AG4_UnitResponse;
  Uni->Render = (VOID *)AG4_UnitRender;
  return (ag4UNIT *)Uni;
} /* End of 'AG4_UnitCreateCow' function */

/* END OF 'U_COW.C' FILE */
