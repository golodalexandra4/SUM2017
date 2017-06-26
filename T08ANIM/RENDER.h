/* FILE NAME: RENDER.H
 * PROGRAMMER: AG4
 * DATE: 13.06.2017
 * PURPOSE: Base rendering system module.
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "def.h"

/* Project parameters */
extern DBL
  AG4_RndProjDist, /* Distance from viewer to project plane */
  AG4_RndProjSize, /* Prohect plane inner size */
  AG4_RndProjFarClip; /*  */

extern MATR
  AG4_RndMatrView, /* Viewer matrix */
  AG4_RndMatrProj; /* Projection matrix */

/* Rendering system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AG4_RndInit( VOID );

/* Project parameters adjust function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AG4_RndSetProj( VOID );

/***
 * Object handle
 ***/

/* Object description type */
typedef struct tagag4OBJ3D
{
  VEC *V;      /* Vertex array */
  INT NumOfV;  /* Vertex array size */
  INT (*F)[3]; /* Facets array (point indices) */
  INT NumOfF;  /* Facets array size */
} ag4OBJ3D;

/* Object free memory function.
 * ARGUMENTS:
 *   - object pointer:
 *       ag4OBJ3D *Obj;
 *   - model *.OBJ file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL AG4_RndObjLoad( ag4OBJ3D *Obj, CHAR *FileName );

/* Object free memory function.
 * ARGUMENTS:
 *   - object pointer:
 *       vg4OBJ3D *Obj;
 * RETURNS: None.
 */
VOID AG4_RndObjFree( ag4OBJ3D *Obj );

/* Object drawing function.
 * ARGUMENTS:
 *   - object pointer:
 *       ag4OBJ3D *Obj;
 *   - world coordinate system transform matrix:
 *       MATR M;
 * RETURNS: None.
 */
VOID AG4_RndObjDraw( ag4OBJ3D *Obj, MATR M );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */