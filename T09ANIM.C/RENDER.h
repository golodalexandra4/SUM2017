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

#define AG4_RES_TABLE_SIZE 1000

#define AG4_MATERIAL_NAME_SIZE 300

/* Project parameters */
extern FLT
  AG4_RndProjDist, /* Distance from viewer to project plane */
  AG4_RndProjSize, /* Prohect plane inner size */
  AG4_RndProjFarClip; /*  */

extern MATR
  AG4_RndMatrView, /* Viewer matrix */
  AG4_RndMatrProj; /* Projection matrix */

/* Light position and color */
extern VEC AG4_RndLightPos;
extern VEC AG4_RndLightColor;

/* Rendering system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AG4_RndInit( VOID );

/* Rendering system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AG4_RndClose( VOID );

/* Project parameters adjust function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AG4_RndSetProj( VOID );

/***
 * Object handle
 ***/

/* Materials table */
typedef struct tagag4MATERIAL
{
  CHAR Name[AG4_MATERIAL_NAME_SIZE]; /* Material name */

  /* Illumination coefficients */ 
  VEC Ka, Kd, Ks;                    /* Ambient, diffuse, specular coefficients */
  FLT Ph;                            /* Phong power coefficient */
  FLT Trans;                         /* Transparency factor */
  INT Tex[8];                        /* Texture references */

  /* Shader string */
  CHAR ShaderStr[AG4_MATERIAL_NAME_SIZE];
  INT Shader;                        /* Texture references */
} ag4MATERIAL;
extern ag4MATERIAL AG4_RndMaterials[AG4_RES_TABLE_SIZE];
extern INT AG4_RndNumOfMaterials;

/* Textures table */
typedef struct tagag4TEXTURE
{
  CHAR Name[AG4_MATERIAL_NAME_SIZE]; /* Texture file name */
  INT W, H;                          /* Texture image size */
  UINT TexId;                        /* Texture ID */
} ag4TEXTURE;
extern ag4TEXTURE AG4_RndTextures[AG4_RES_TABLE_SIZE];
extern INT AG4_RndNumOfTextures;

/* Shaders table */
/* Shader store representation type */
typedef struct tagag4SHADER
{
  CHAR Name[AG4_MATERIAL_NAME_SIZE]; /* Shader file name prefix */
  UINT ProgId;                       /* Shader program ID */
} ag4SHADER;
extern ag4SHADER AG4_RndShaders[AG4_RES_TABLE_SIZE];
extern INT AG4_RndNumOfShaders;

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
  INT MtlNo;
} ag4PRIM;

typedef struct tagag4OBJ
{
  ag4PRIM *P;
  INT NumOfP;
  /* object dimensions */
  VEC MinV, MaxV;
} ag4OBJ;

/* Object free memory function.
 * ARGUMENTS:
 *   - object pointer:
 *       ag4OBJ3D *Obj;
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
VOID AG4_RndPrimCreatePlane( ag4OBJ *Pr, VEC C, VEC Du, VEC Dv, INT N, INT M );

/* Text file load to memory function.
 * ARGUMENTS:
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (CHAR *) load text or NULL if error is occured.
 */
UINT AG4_RndShaderLoad( CHAR *FileNamePrefix );
VOID AG4_RndShaderFree( UINT Prg );

/* Resourcse initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AG4_RndResInit( VOID );

/* Resourcse deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AG4_RndResClose( VOID );

/* Add shader function.
 * ARGUMENTS:
 *   - shader file name prefix:
 *       CHAR *FileNamePrefix;
 * RETURNS:
 *   (INT) shader table number.
 */
INT AG4_RndShaderAdd( CHAR *FileNamePrefix );

/* Add material function.
 * ARGUMENTS:
 *   - material data:
 *       ag4MATERIAL *Mtl;
 * RETURNS:
 *   (INT) material table number.
 */
INT AG4_RndMaterialAdd( ag4MATERIAL *Mtl );

/* Add texture function.
 * ARGUMENTS:
 *   - texture name:
 *       CHAR *Name;
 *   - texture image size:
 *       INT W, H;
 *   - texture image pixel number of component:
 *       INT Components;
 *   - texture image pixel color data:
 *       VOID *Data;
 * RETURNS:
 *   (INT) texture table number.
 */
INT AG4_RndTextureAdd( CHAR *Name, INT W, INT H, INT Components, VOID *Data );

/* Material apply function.
 * ARGUMENTS:
 *   - material table number:
 *       INT MtlNo;
 * RETURNS:
 *   (UINT) shader program Id.
 */
UINT AG4_RndMaterialApply( INT MtlNo );

VOID AG4_RndObjCreate( ag4OBJ *Obj, INT NumOfP );
VOID AG4_RndObjFree( ag4OBJ *Obj );
VOID AG4_RndObjDraw( ag4OBJ *Obj, MATR M );
BOOL AG4_RndObjLoad( ag4OBJ *Obj, CHAR *FileName );

/* Collision functions */
VOID AG4_Reset( INT N );
INT AG4_Register( ag4OBJ *Obj );
BOOL AG4_IsColide( ag4OBJ *Obj );
VOID Swap( ag4OBJ *Obj1, ag4OBJ *Obj2 );
VOID AG4_CowNumber( INT N );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */