/* FILE NAME: U_CONTR.C
 * PROGRAMMER: AG4
 * DATE: 10.06.2017
 * PURPOSE: Units base functions.
 */

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "anim.h"
#include "units.h"

/* Animation unit representation type */
typedef struct tagag4UNIT_CONTROL
{
  AG4_UNIT_BASE_FIELDS;
  ag4OBJ Cactus;
  VEC Pos;
  FLT Rotate;
  DBL TimeShift;
  INT Number; /* Number of unit in array of units */
} ag4UNIT_CONTROL;

/* Control unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ag4UNIT_CONTROL *Uni;
 *   - animation context:
 *       ag4ANIM *Ani;
 * RETURNS: None.
 */
static VOID AG4_UnitInit( ag4UNIT_CONTROL *Uni, ag4ANIM *Ani )
{
  VEC place[3];
  INT r = (INT)rand() % 3;

  Uni->TimeShift = 1;

  place[0] = VecSet(-10, 0, -30);
  place[1] = VecSet(-20, 0, -20);
  place[2] = VecSet(-30, 0, -10);
  AG4_RndObjLoad( &Uni->Cactus, "models\\cactus.g3dm" );
  Uni->Number = AG4_Register(&Uni->Cactus);
  Uni->Pos = place[r];

} /* End of 'AG4_UnitInit' function */

/* Control unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ag4UNIT_CONTROL *Uni;
 *   - animation context:
 *       ag4ANIM *Ani;
 * RETURNS: None.
 */
static VOID AG4_UnitClose( ag4UNIT_CONTROL *Uni, ag4ANIM *Ani )
{
  AG4_RndObjFree(&Uni->Cactus);
} /* End of 'AG4_UnitClose' function */

/* Control unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ag4UNIT_CONTROL *Uni;
 *   - animation context:
 *       ag4ANIM *Ani;
 * RETURNS: None.
 */
static VOID AG4_UnitResponse( ag4UNIT_CONTROL *Uni, ag4ANIM *Ani )
{
  static BOOL counter = FALSE;
  static DBL OldCactusTime = 0;
  static BOOL cow_counter = FALSE;
  
  if (!counter)
  {  
    AG4_AnimAddUnit(AG4_UnitCreateGround());
    counter = !counter;
  }

  if(!cow_counter)
    AG4_AnimAddUnit(AG4_UnitCreateCow()), cow_counter = !cow_counter;

  if (Ani->KeysClick[VK_ESCAPE])
    AG4_AnimDoExit();
  else if (Ani->KeysClick['F'])
    AG4_FlipFullScreen( Ani->hWnd );
  else if (Ani->JButClick[5])
    AG4_FlipFullScreen( Ani->hWnd );
  else if (Ani->KeysClick['P'])
    AG4_Anim.IsPause = !AG4_Anim.IsPause;
  else if (Ani->JButClick[4])
    AG4_Anim.IsPause = !AG4_Anim.IsPause;
  else if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else if (Ani->JButClick[7])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else if (Ani->KeysClick['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  else if (Ani->JButClick[0])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  if (Ani->Time - OldCactusTime > Uni->TimeShift)
  {
    OldCactusTime = Ani->Time;
    Uni->TimeShift = rand() % 5 + 10;
    AG4_AnimAddUnit(AG4_UnitCreateControl());
  }

  Uni->Pos.X += Ani->Time / 400;
  Uni->Pos.Z += Ani->Time / 400;

  /* Checking collision
  if (AG4_IsColide(&Uni->Cactus))
  {
    AG4_Reset(Uni->Number - 1);
    //AG4_RndObjFree(&Uni->Cactus);
  }*/

  if (Ani->JButClick[6])
  {
    Uni->Pos.X = 0;
    Uni->Pos.Y = 0;
    Uni->Pos.Z = 0;
  }

} /* End of 'AG4_UnitResponse' function */

/* Control unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ag4UNIT_CONTROL *Uni;
 *   - animation context:
 *       ag4ANIM *Ani;
 * RETURNS: None.
 */
static VOID AG4_UnitRender( ag4UNIT_CONTROL *Uni, ag4ANIM *Ani )
{
  INT len;
  static CHAR Buf[100];
  VEC tmp = Uni->Pos;

  len = sprintf(Buf, "FPS: %.5f, Units: %d, M: (%d,%d) [%d,%d] %d "
    "J: %.3f %.3f %.3f %.3f",
    Ani->FPS, Ani->NumOfUnits,
    Ani->Mdx, Ani->Mdy, Ani->Mx, Ani->My, Ani->Mz,
    Ani->Jx, Ani->Jy, Ani->Jz, Ani->Jr);

  SetWindowText(Ani->hWnd, Buf);

  tmp.X += Ani->Mz / 60;
  tmp.Y += Ani->Mz / 60;
  tmp.Z += Ani->Mz / 60;
  //Uni->Cactus.MaxV = VecMulMatr3(Uni->Cactus.MaxV, MatrMulMatr(MatrRotate(Uni->Rotate, VecSet(0, -1, 0)), MatrTranslate(tmp)));
  AG4_RndObjDraw(&Uni->Cactus, MatrMulMatr(MatrRotate(Uni->Rotate, VecSet(0, -1, 0)), MatrTranslate(tmp)));

  /* Checking collision */
  if (AG4_IsColide(&Uni->Cactus))
  {
    AG4_Reset(Uni->Number - 1);
    AG4_RndObjFree(&Uni->Cactus);
  }

  /*if (Uni->Pos.Z > 100)
  {
    AG4_RndObjFree(&Uni->Cactus);
    Ani->NumOfUnits--;
  } */
} /* End of 'AG4_UnitRender' function */

/* Control unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (ag4UNIT *) pointer to created unit.
 */
ag4UNIT * AG4_UnitCreateControl( VOID )
{
  ag4UNIT_CONTROL *Uni;

  /* Memory allocation */
  if ((Uni = (ag4UNIT_CONTROL *)AG4_AnimUnitCreate(sizeof(ag4UNIT_CONTROL))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)AG4_UnitInit;
  Uni->Close = (VOID *)AG4_UnitClose;
  Uni->Response = (VOID *)AG4_UnitResponse;
  Uni->Render = (VOID *)AG4_UnitRender;
  return (ag4UNIT *)Uni;
} /* End of 'AG4_UnitCreateControl' function */

/* END OF 'U_CONTROL.C' FILE */
