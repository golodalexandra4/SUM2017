/* FILE NAME: RENDER.H
 * PROGRAMMER: AG4
 * DATE: 13.06.2017
 * PURPOSE: Base rendering system module.
 */

#ifndef __RENDER_H_
#define __RENDER_H_

/* OpenGL specific includes */
#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "def.h"

/* Project parameters */
extern FLT
  AG4_RndProjDist, /* Distance from viewer to project plane */
  AG4_RndProjSize, /* Prohect plane inner size */
  AG4_RndProjFarClip; /*  */

extern MATR
  AG4_RndMatrView, /* Viewer matrix */
  AG4_RndMatrProj; /* Projection matrix */

/* Current shader */
extern UINT
  AG4_RndProgId;    /* Shader program identifier */

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


typedef struct tagag4VERTEX
{
  VEC P;
  VEC2 T;
  VEC N;
  VEC4 C;
}ag4VERTEX;

typedef struct tagag4PRIM
{
  BOOL IsTrimesh;
  INT NumOfI;
  MATR M;

  /* VBO Data */
  INT VA, VBuf;
  /* Index Buffer */
  INT IBuf;
} ag4PRIM;

typedef struct tagag4OBJ
{
  ag4PRIM *P;
  INT NumOfP;
} ag4OBJ;

/* Object free memory function.
 * ARGUMENTS:
 *   - object pointer:
 *       vg4OBJ3D *Obj;
 * RETURNS: None.
 */
VOID AG4_RndObjFree( ag4OBJ *Obj );

/* Object drawing function.
 * ARGUMENTS:
 *   - object pointer:
 *       ag4OBJ3D *Obj;
 *   - world coordinate system transform matrix:
 *       MATR M;
 * RETURNS: None.
 */
VOID AG4_RndObjDraw( ag4OBJ *Obj, MATR M );

/* Create primitive function.
 * ARGUMENTS:
 *   - created primitive:
 *       ag4PRIM *Pr;
 *   - trimesh flag:
 *       BOOL IsTrimesh;
 *   - vertex array:
 *       ag4VERTEX *V;
 *   - vertex array size:
 *       INT NumOfV
 *   - index array:
 *       INT *I;
 *   - index array size:
 *       INT NumOfI;
 * RETURNS: None.
 */
VOID AG4_RndPrimCreate( ag4PRIM *Pr, BOOL IsTrimesh,
                        ag4VERTEX *V, INT NumOfV,
                        INT *I, INT NumOfI );
/* Free primitive function.
 * ARGUMENTS:
 *   - deleted primitive:
 *       ag4PRIM *Pr;
 * RETURNS: None.
 */
VOID AG4_RndPrimFree( ag4PRIM *Pr );

/* Draw primitive function.
 * ARGUMENTS:
 *   - drawing primitive:
 *       ag4PRIM *Pr;
 *   - transformation matrix:
 *       MATR M;
 * RETURNS: None.
 */
VOID AG4_RndPrimDraw( ag4PRIM *Pr, MATR M );

/* Primitive load function.
 * ARGUMENTS:
 *   - primitive pointer:
 *       ag4OBJ3D *Obj;
 *   - model *.OBJ file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL AG4_RndPrimLoad( ag4PRIM *Obj, CHAR *FileName );

/* Evaluate trimesh vertex normals function.
 * ARGUMENTS:
 *   - vertex array:
 *       ag4VERTEX *V;
 *   - vertex array size:
 *       INT NumOfV;
 *   - index array:
 *       INT *I;
 *   - index array size:
 *       INT NumOfI;
 * RETURNS: None.
 */
VOID AG4_RndTriMeshEvalNormals( ag4VERTEX *V, INT NumOfV, INT *I, INT NumOfI );

/* Evaluate grid vertex normals function.
 * ARGUMENTS:
 *   - vertex array:
 *       ag4VERTEX *V;
 *   - grid split numbers:
 *       INT N, M;
 * RETURNS: None.
 */
VOID AG4_RndGridEvalNormals( ag4VERTEX *V, INT N, INT M );

/* Sphere primitive creation function.
 * ARGUMENTS:
 *   - primitive to be create:
 *       ag4PRIM *Pr;
 *   - sphere center position:
 *       VEC C;
 *   - sphere radius:
 *       FLT R;
 *   - sphere split numbers:
 *       INT N, M;
 * RETURNS: None.
 */
VOID AG4_RndPrimCreateSphere( ag4PRIM *Pr, VEC C, FLT R, INT N, INT M );

/* Plane primitive creation function.
 * ARGUMENTS:
 *   - primitive to be create:
 *       ag4PRIM *Pr;
 *   - plane corner position:
 *       VEC C;
 *   - plane directions:
 *       VEC Du, Dv;
 *   - plane split numbers:
 *       INT N, M;
 * RETURNS: None.
 */
VOID AG4_RndPrimCreatePlane( ag4PRIM *Pr, VEC C, VEC Du, VEC Dv, INT N, INT M );

/* Text file load to memory function.
 * ARGUMENTS:
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (CHAR *) load text or NULL if error is occured.
 */
UINT AG4_RndShaderLoad( CHAR *FileNamePrefix );
VOID AG4_RndShaderFree( UINT Prg );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */