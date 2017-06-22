/* FILE NAME: MATERIAL.C
 * PROGRAMMER: AG4
 * DATE: 17.06.2017
 * PURPOSE: Material (texture, shader store) handle module.
 */

#include <string.h>

#include "anim.h"

/***
 * Resource storages
 ***/

/* Materials table */
ag4MATERIAL AG4_RndMaterials[AG4_RES_TABLE_SIZE];
INT AG4_RndNumOfMaterials;

/* Textures table */
ag4TEXTURE AG4_RndTextures[AG4_RES_TABLE_SIZE];
INT AG4_RndNumOfTextures;

/* Shaders table */
ag4SHADER AG4_RndShaders[AG4_RES_TABLE_SIZE];
INT AG4_RndNumOfShaders;

/* Resourcse initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AG4_RndResInit( VOID )
{
  ag4MATERIAL def_mtl =
  {
    "Default material",
    {0.1, 0.1, 0.1}, {0.9, 0.9, 0.9}, {0.3, 0.3, 0.3}, 30,
    1,
    {-1, -1, -1, -1, -1, -1, -1, -1},
    "", 0
  };

  memset(AG4_RndMaterials, 0, sizeof(AG4_RndMaterials));
  memset(AG4_RndTextures, 0, sizeof(AG4_RndTextures));
  memset(AG4_RndShaders, 0, sizeof(AG4_RndShaders));
  AG4_RndNumOfMaterials = 0;
  AG4_RndNumOfTextures = 0;
  AG4_RndNumOfShaders = 0;

  AG4_RndShaderAdd("SHADERS\\DEFAULT");
  AG4_RndMaterialAdd(&def_mtl);
} /* End of 'AG4_RndResInit' function */

/* Resourcse deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AG4_RndResClose( VOID )
{
  INT i;

  /* Delete shaders */
  for (i = 0; i < AG4_RndNumOfShaders; i++)
    AG4_RndShaderFree(AG4_RndShaders[i].ProgId);
  memset(AG4_RndShaders, 0, sizeof(AG4_RndShaders));
  AG4_RndNumOfShaders = 0;

  /* Delete textures */
  for (i = 0; i < AG4_RndNumOfTextures; i++)
    glDeleteTextures(1, &AG4_RndTextures[i].TexId);
  memset(AG4_RndTextures, 0, sizeof(AG4_RndTextures));
  AG4_RndNumOfTextures = 0;

  /* Remove materials */
  memset(AG4_RndMaterials, 0, sizeof(AG4_RndMaterials));
  AG4_RndNumOfMaterials = 0;
} /* End of 'AG4_RndResClose' function */

/* Add shader function.
 * ARGUMENTS:
 *   - shader file name prefix:
 *       CHAR *FileNamePrefix;
 * RETURNS:
 *   (INT) shader table number.
 */
INT AG4_RndShaderAdd( CHAR *FileNamePrefix )
{
  if (AG4_RndNumOfShaders >= AG4_RES_TABLE_SIZE)
    return 0;
  strncpy(AG4_RndShaders[AG4_RndNumOfShaders].Name, FileNamePrefix,
    AG4_MATERIAL_NAME_SIZE - 1);
  AG4_RndShaders[AG4_RndNumOfShaders].ProgId =
    AG4_RndShaderLoad(FileNamePrefix);
  return AG4_RndNumOfShaders++;
} /* End of 'AG4_RndShaderAdd' function */

/* Add material function.
 * ARGUMENTS:
 *   - material data:
 *       ag4MATERIAL *Mtl;
 * RETURNS:
 *   (INT) material table number.
 */
INT AG4_RndMaterialAdd( ag4MATERIAL *Mtl )
{
  if (AG4_RndNumOfMaterials >= AG4_RES_TABLE_SIZE)
    return 0;
  AG4_RndMaterials[AG4_RndNumOfMaterials] = *Mtl;
  return AG4_RndNumOfMaterials++;
} /* End of 'AG4_RndMaterialAdd' function */

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
INT AG4_RndTextureAdd( CHAR *Name, INT W, INT H, INT Components, VOID *Data )
{
  if (AG4_RndNumOfTextures >= AG4_RES_TABLE_SIZE)
    return -1;
  strncpy(AG4_RndTextures[AG4_RndNumOfTextures].Name, Name,
    AG4_MATERIAL_NAME_SIZE - 1);
  AG4_RndTextures[AG4_RndNumOfTextures].W = W;
  AG4_RndTextures[AG4_RndNumOfTextures].H = H;

  glGenTextures(1, &AG4_RndTextures[AG4_RndNumOfTextures].TexId);
  glBindTexture(GL_TEXTURE_2D, AG4_RndTextures[AG4_RndNumOfTextures].TexId);
  gluBuild2DMipmaps(GL_TEXTURE_2D, Components, W, H,
    Components == 4 ? GL_BGRA_EXT : Components == 3 ? GL_BGR_EXT : GL_LUMINANCE,
    GL_UNSIGNED_BYTE, Data);
  glBindTexture(GL_TEXTURE_2D, 0);

  return AG4_RndNumOfTextures++;
} /* End of 'AG4_RndTextureAdd' function */

/* Material apply function.
 * ARGUMENTS:
 *   - material table number:
 *       INT MtlNo;
 * RETURNS:
 *   (UINT) shader program Id.
 */
UINT AG4_RndMaterialApply( INT MtlNo )
{
  ag4MATERIAL *mtl;
  INT prg, loc, i;

  if (MtlNo < 0 || MtlNo >= AG4_RndNumOfMaterials)
    MtlNo = 0;
  mtl = &AG4_RndMaterials[MtlNo];

  /* Enable shader */
  prg = mtl->Shader;
  if (prg < 0 || prg >= AG4_RndNumOfShaders)
    prg = 0;
  prg = AG4_RndShaders[prg].ProgId;

  glUseProgram(prg);

  /* Illumination coefficients */
  loc = glGetUniformLocation(prg, "Ka");
  if (loc != -1)
    glUniform3fv(loc, 1, &mtl->Ka.X);
  loc = glGetUniformLocation(prg, "Kd");
  if (loc != -1)
    glUniform3fv(loc, 1, &mtl->Kd.X);
  loc = glGetUniformLocation(prg, "Ks");
  if (loc != -1)
    glUniform3fv(loc, 1, &mtl->Ks.X);
  loc = glGetUniformLocation(prg, "Ph");
  if (loc != -1)
    glUniform1f(loc, mtl->Ph);
  loc = glGetUniformLocation(prg, "Trans");
  if (loc != -1)
    glUniform1f(loc, mtl->Trans);

  /* Textures */
  for (i = 0; i < 8; i++)
  {
    CHAR ist[] = "IsTexture0";
    INT t = mtl->Tex[i];

    /* Set texture usage flag */
    ist[9] = '0' + i;
    loc = glGetUniformLocation(prg, ist);
    if (loc != -1)
      glUniform1i(loc, t != -1);

    /* Select texture to sampler */
    if (t != -1)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, AG4_RndTextures[t].TexId);
    }
  }
  return prg;
} /* End of 'AG4_RndMaterialApply' function */

/* END OF 'MATERIAL.C' FILE */
