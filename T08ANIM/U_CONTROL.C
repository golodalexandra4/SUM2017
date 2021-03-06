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
  HFONT hFont;           /* Using font */
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
  Uni->hFont = CreateFont(30, 0, 0, 0, FW_BOLD, TRUE, FALSE, FALSE, RUSSIAN_CHARSET,
    OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
    DEFAULT_PITCH | FF_DONTCARE, "");
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
  DeleteObject(Uni->hFont);
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
  if (Ani->KeysClick[VK_ESCAPE])
    AG4_AnimDoExit();
  else if (Ani->KeysClick['F'])
    AG4_FlipFullScreen( Ani->hWnd );
  else if (Ani->KeysClick['P'])
    AG4_Anim.IsPause = !AG4_Anim.IsPause;
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
  HFONT hFontOld = SelectObject(Ani->hDC, Uni->hFont);
  INT len;
  static CHAR Buf[100];

  len = sprintf(Buf, "FPS: %.5f, Units: %d, Wheel: %d "
    "J: %.3f %.3f %.3f %.3f",
    Ani->FPS, Ani->NumOfUnits, Ani->Mz,
    Ani->Jx, Ani->Jy, Ani->Jy, Ani->Jr);

  SetBkMode(Ani->hDC, TRANSPARENT);
  SetTextColor(Ani->hDC, RGB(0, 0, 0));
  TextOut(Ani->hDC, 2, 2, Buf, len);
  SetTextColor(Ani->hDC, RGB(0, 150, 0));
  TextOut(Ani->hDC, 0, 0, Buf, len);

  SelectObject(Ani->hDC, hFontOld);
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

/* END OF 'U_CONTR.C' FILE */
