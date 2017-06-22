/* FILE NAME: OBJ.C
 * PROGRAMMER: AG4
 * DATE: 14.06.2017
 * PURPOSE: Object handle module.
 */

#include <stdio.h>

#include "anim.h"

VEC
  AG4_RndObjMin,
  AG4_RndObjMax;

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
  memset(Obj, 0, sizeof(ag4OBJ));
  if ((Obj->P = malloc(sizeof(ag4PRIM) * NumOfP)) == NULL)
    return;
  memset(Obj->P, 0, sizeof(ag4PRIM) * NumOfP);
  Obj->NumOfP = NumOfP;
} /* Ens of 'AG4_RndObjCreate' function */

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
  memset(Obj, 0, sizeof(ag4OBJ));
} /* Ens of 'AG4_RndObjFree' function */

/* Draw object function.
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
} /* Ens of 'AG4_RndObjDraw' function */

/* Load object from "G3DM" file function.
 * ARGUMENTS:
 *   - drawing object:
 *       ag4OBJ *Obj;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL AG4_RndObjLoad( ag4OBJ *Obj, CHAR *FileName )
{
  FILE *F;
  DWORD Sign;
  INT NoofP = 0, NoofM = 0, NoofT = 0, i;
  VEC max, min;

  memset(Obj, 0, sizeof(ag4OBJ));

  /* Open file */
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  /* Check signature */
  fread(&Sign, 4, 1, F);
  if (Sign != *(DWORD *)"G3DM")
  {
    fclose(F);
    return FALSE;
  }

  /* Read quantities */
  fread(&NoofP, 4, 1, F);
  fread(&NoofM, 4, 1, F);
  fread(&NoofT, 4, 1, F);

  if (AG4_RndNumOfMaterials + NoofM >= AG4_RES_TABLE_SIZE ||
      AG4_RndNumOfTextures + NoofT >= AG4_RES_TABLE_SIZE)
  {
    fclose(F);
    return FALSE;
  }

  /* Create object */
  AG4_RndObjCreate(Obj, NoofP);

  /* Load primitives */
  for (i = 0; i < NoofP; i++)
  {
    INT nv = 0, ni = 0, mtl_no = -1, size;
    ag4VERTEX *V;
    INT *I;

    fread(&nv, 4, 1, F);
    fread(&ni, 4, 1, F);
    fread(&mtl_no, 4, 1, F);
    size = sizeof(ag4VERTEX) * nv + sizeof(INT) * ni;
    if ((V = malloc(size)) == NULL)
    {
      fclose(F);
      AG4_RndObjFree(Obj);
      return FALSE;
    }
    fread(V, 1, size, F);
    I = (INT *)(V + nv);
    AG4_RndPrimCreate(&Obj->P[i], TRUE, V, nv, I, ni);
    free(V);
    if (mtl_no != -1)
      mtl_no += AG4_RndNumOfMaterials;
    Obj->P[i].MtlNo = mtl_no;
  }

  /* Found dimensions of primitive */
  /* min = max = */

  /* Load materials */
  fread(&AG4_RndMaterials[AG4_RndNumOfMaterials], sizeof(ag4MATERIAL), NoofM, F);
  for (i = 0; i < NoofM; i++)
  {
    INT j;
    ag4MATERIAL *mtl = &AG4_RndMaterials[AG4_RndNumOfMaterials + i];

    if (mtl->ShaderStr[0] != 0)
      mtl->Shader = AG4_RndShaderAdd(mtl->ShaderStr);

    for (j = 0; j < 8; j++)
      if (mtl->Tex[j] != -1)
        mtl->Tex[j] += AG4_RndNumOfTextures;
  }
  AG4_RndNumOfMaterials += NoofM;

  /* Load textures */
  for (i = 0; i < NoofT; i++)
  {
    CHAR Name[300];
    INT W, H, C;
    VOID *mem;

    fread(Name, 1, 300, F);
    fread(&W, 4, 1, F);
    fread(&H, 4, 1, F);
    fread(&C, 4, 1, F);
    if ((mem = malloc(W * H * C)) == NULL)
    {
      fclose(F);
      AG4_RndObjFree(Obj);
      return FALSE;
    }
    fread(mem, C, W * H, F);
    AG4_RndTextureAdd(Name, W, H, C, mem);
    free(mem);
  }
  AG4_RndNumOfTextures += NoofT;

  fclose(F);
  return TRUE;
} /* Ens of 'AG4_RndObjLoad' function */

/* END OF 'OBJ.C' FILE */
