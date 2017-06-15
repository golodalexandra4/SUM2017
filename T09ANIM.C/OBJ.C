/* FILE NAME: OBJ.C
 * PROGRAMMER: AG4
 * DATE: 14.06.2017
 * PURPOSE: Object handle module.
 */

#include <stdio.h>

#include "anim.h"

/* Create object function.
 * ARGUMENTS:
 *   - created object:
 *       ag4OBJ *Obj;
 *   - number of primitives:
 *       INT NumOfP;
* RETURNS: None.
 */
VOID AG4_RndObjCreate( ag4OBJ *Obj, INT NumOfP )
{
  memset(Obj, 0, sizeof(ag4PRIM));
  if ((Obj->P = malloc(sizeof(ag4PRIM) * NumOfP)) == NULL)
    return;
  memset(Obj->P, 0, sizeof(ag4PRIM) * NumOfP);
  Obj->NumOfP = NumOfP;
} /* End of 'AG4_RndObjCreate' function */

/* Free object function.
 * ARGUMENTS:
 *   - deleted object:
 *       ag4OBJ *Obj;
 * RETURNS: None.
 */
VOID AG4_RndObjFree( ag4OBJ *Obj )
{
  INT i;

  for (i = 0; i < Obj->NumOfP; i++)
    AG4_RndPrimFree(&Obj->P[i]);
  free(Obj->P);
  memset(Obj, 0, sizeof(ag4PRIM));
} /* End of 'AG4_RndObjFree' function */

/* Draw primitive function.
 * ARGUMENTS:
 *   - drawing object:
 *       ag4OBJ *Obj;
 *   - transformation matrix:
 *       MATR M;
 * RETURNS: None.
 */
VOID AG4_RndObjDraw( ag4OBJ *Obj, MATR M )
{
  INT i;

  for (i = 0; i < Obj->NumOfP; i++)
    AG4_RndPrimDraw(&Obj->P[i], M);
} /* End of 'AG4_RndObjDraw' function */

/* END OF 'OBJ.C' FILE */
