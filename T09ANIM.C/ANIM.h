/* FILE NAME: ANIM.H
 * PROGRAMMER: AG4
 * DATE: 09.06.2017
 * PURPOSE: Common definitions.
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "def.h"
#include "render.h"

#define AG4_MAX_UNITS 1000
#define AG4_UNIT_BASE_FIELDS \
  /* Unit initialization function.                \
   * ARGUMENTS:                                   \
   *   - self-pointer to unit object:             \
   *       ag4UNIT *Uni;                          \
   *   - animation context:                       \
   *       ag4ANIM *Ani;                          \
   * RETURNS: None.                               \
   */                                             \
  VOID (*Init)( ag4UNIT *Uni, ag4ANIM *Ani );     \
                                                  \
  /* Unit deinitialization function.              \
   * ARGUMENTS:                                   \
   *   - self-pointer to unit object:             \
   *       ag4UNIT *Uni;                          \
   *   - animation context:                       \
   *       ag4ANIM *Ani;                          \
   * RETURNS: None.                               \
   */                                             \
  VOID (*Close)( ag4UNIT *Uni, ag4ANIM *Ani );    \
                                                  \
  /* Unit inter frame events handle function.     \
   * ARGUMENTS:                                   \
   *   - self-pointer to unit object:             \
   *       ag4UNIT *Uni;                          \
   *   - animation context:                       \
   *       ag4ANIM *Ani;                          \
   * RETURNS: None.                               \
   */                                             \
  VOID (*Response)( ag4UNIT *Uni, ag4ANIM *Ani ); \
                                                  \
  /* Unit render function.                        \
   * ARGUMENTS:                                   \
   *   - self-pointer to unit object:             \
   *       ag4UNIT *Uni;                          \
   *   - animation context:                       \
   *       ag4ANIM *Ani;                          \
   * RETURNS: None.                               \
   */                                             \
  VOID (*Render)( ag4UNIT *Uni, ag4ANIM *Ani )

typedef struct tagag4ANIM ag4ANIM;
typedef struct tagag4UNIT ag4UNIT;

struct tagag4UNIT
{
  AG4_UNIT_BASE_FIELDS;
};

struct tagag4ANIM
{
  HWND hWnd;
  HDC hDC;      /* Drawing window context */
  HGLRC hGLRC;  /* Rendering context */
  INT W, H;
  ag4UNIT *Units[AG4_MAX_UNITS];
  INT NumOfUnits;
  
  DBL
    GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
    Time, DeltaTime,             /* Time with pause and interframe interval */
    FPS;                         /* Frames per seond value */
  BOOL
    IsPause;                     /* Pause flag */
  
  BYTE Keys[256];       /* keyboard */
  BYTE KeysOld[256];
  BYTE KeysClick[256];
 
  INT
    Mx, My, Mz,     /* Absolute coordinates */
    Mdx, Mdy, Mdz;  /* Relative coordinates shift */

  BYTE
    JBut[32],                    /* Joystick button state */
    JButOld[32],                 /* Joystick button old state */
    JButClick[32];               /* Joystick button click info */
  INT JPov;                      /* Joystick point-of-view control [0,1..8] */
  DBL
    Jx, Jy, Jz, Jr;              /* Joystick axes */
};

extern ag4ANIM AG4_Anim;
extern INT AG4_MouseWheel;

ag4UNIT *AG4_AnimUnitCreate( INT Size );

BOOL AG4_AnimInit( HWND hWnd );
VOID AG4_AnimClose( VOID );
VOID AG4_AnimResize( INT W, INT H );
VOID AG4_AnimCopyFrame( VOID );
VOID AG4_AnimRender( VOID );
VOID AG4_AnimDoExit( VOID );
VOID AG4_AnimAddUnit( ag4UNIT *Uni );
VOID AG4_FlipFullScreen( HWND hWnd );

#endif 
/* end of "ANIM.H" file*/