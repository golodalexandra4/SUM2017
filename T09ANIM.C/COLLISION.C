/* FILE NAME: MATERIAL.C
 * PROGRAMMER: AG4
 * DATE: 22.06.2017
 * PURPOSE: module, which checking object collision.
 */

#include "anim.h"
#include "units.h"

ag4OBJ *ColObj[AG4_MAX_UNITS];
INT NumOfObj = 0;
INT NumOfCow = 0;

VOID AG4_Reset( INT N )
{
  if (NumOfObj == 0)
    return;
  Swap(ColObj[N], ColObj[NumOfObj - 1]);
  AG4_RndObjFree(ColObj[NumOfObj - 1]);
  NumOfObj--;
}

INT AG4_Register( ag4OBJ *Obj )
{
  ColObj[NumOfObj] = Obj;
  NumOfObj++;

  return NumOfObj;
}

BOOL AG4_IsColide( ag4OBJ *Obj )
{
  INT numer = AG4_Register(Obj);
  
  if ((ColObj[NumOfCow - 1]->MaxV.X > ColObj[numer - 1]->MinV.X || 
       ColObj[NumOfCow - 1]->MaxV.Z > ColObj[numer - 1]->MinV.Z) || 
      (ColObj[NumOfCow - 1]->MinV.X < ColObj[numer - 1]->MaxV.X || 
       ColObj[NumOfCow - 1]->MinV.Z < ColObj[numer - 1]->MaxV.Z))
    return FALSE;
  else
    return TRUE;
}

VOID AG4_CowNumber( INT N )
{
  NumOfCow = N;
}
VOID Swap( ag4OBJ *Obj1, ag4OBJ *Obj2 )
{
  ag4OBJ tmp;

  tmp = *Obj1;
  *Obj1 = *Obj2;
  *Obj2 = tmp;
}
/* END OF 'COLLISION.C' FILE */