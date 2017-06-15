/* FILE NAME: UNITS.C
 * PROGRAMMER: AG4
 * DATE: 09.06.2017
 * PURPOSE: Common definitions.
 */

#include "anim.h"

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID AG4_UnitInit( ag4UNIT *Uni, ag4ANIM *Ani )
{
} /* End of 'VG4_UnitInit' function */
/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */

static VOID AG4_UnitClose( ag4PRIM *Uni, ag4ANIM *Ani )
{
} /* End of 'VG4_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID AG4_UnitResponse( ag4UNIT *Uni, ag4ANIM *Ani )
{
} /* End of 'VG4_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID AG4_UnitRender( ag4UNIT *Uni, ag4ANIM *Ani )
{
} /* End of 'VG4_UnitRender' function */

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (vg4UNIT *) pointer to created unit.
 */
ag4UNIT *AG4_AnimUnitCreate( INT Size )
{
  ag4UNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(ag4UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* Setup unit methods */
  Uni->Init = AG4_UnitInit;
  Uni->Close = AG4_UnitClose;
  Uni->Response = AG4_UnitResponse;
  Uni->Render = AG4_UnitRender;
  return Uni;
} /* End of 'AG4_AnimUnitCreate' function */

/* end of 'UNITS.C' file '*/