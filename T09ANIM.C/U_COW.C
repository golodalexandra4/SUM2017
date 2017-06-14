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
  Uni->Rotate += Ani->DeltaTime * Ani->Keys[VK_LBUTTON] * Ani->Mdx * 102;
  /*
  Uni->Rotate += Ani->DeltaTime * Ani->Keys[VK_RBUTTON] * (Ani->W / 2 - Ani->Mx) / 5.0;
  Uni->Rotate += Ani->DeltaTime * AG4_MouseWheel * 2;
  */
} /* End of 'AG4_UnitResponse' function */

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
  AG4_RndObjDraw(&Uni->Cow, MatrRotateY(Uni->Rotate));
} /* End of 'VG4_UnitRender' function */

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
