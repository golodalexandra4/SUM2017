/* FILE NAME: T05DET.C
 * PROGRAMMER: AG4
 * DATE: 05.06.2017
 * PURPOSE: Get matrix's determination.
 */

#include <stdio.h>
#include <math.h>
#define MAX 4

int N, Parity, P[MAX];
double A[MAX][MAX], Det = 1;

/* Get matrix from file */
void MatrixLoad( char *FileName )
{
  int i, j;
  FILE *F;

  if ((F = fopen(FileName, "r")) == NULL)
    return;

  fscanf(F, "%d", &N);
  Parity = 0;

  if (N > MAX)
    N = MAX;
  
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &A[i][j]);
  
  fclose(F);
}

/* get permetations */
/*void Go( int Pos ) 
{
  int i;
  
  if (Pos >= N)
    return;

  for (i = Pos; i < N; i++)
  {
    Swap(&P[Pos], &P[i]);
    if(Pos != i)
      Parity = !Parity;
    Go(Pos + 1);
    p *= A[i][P[i]];
    if (Parity == 0)
      det += p;
    else
      det -= p;
    Swap(&P[Pos], &P[i]);
  }
}  */

/* print determination to file */
void PrintDet( void ) 
{
  FILE *F;

  if ((F = fopen("MATRIX.TXT", "a")) == NULL)
    return;
  fprintf(F, "determinator: %lf\n", Det);
  fclose(F);
}

/* change two elements */
void Swap( double *A, double *B )
{
  double tmp = *A;

  *A = *B;
  *B = tmp;
}

/* change matrix*/
void Gauss( void )
{
  int i, j, ii, jj, k, max_i, max_j, sign = 1;

  for (j = 0; j < N; j++)
  {
    max_i = max_j = j;

    /* Look for max value */
    for (ii = j; ii < N; ii++)
      for (jj = j; jj < N; jj++)
        if (fabs(A[max_i][max_j]) < fabs(A[ii][jj]))
          max_i = ii, max_j = jj;
  
    if (fabs(A[max_i][max_j]) == 0)
      return;

    /* Swap rows */
    if (max_i != j)
    {
      for (i = j; i < N; i++)
        Swap(&A[max_i][i], &A[j][i]);
      sign = -sign;
    }
    /* Swap columns */
    if (max_j != j)
    {  
      for (i = 0; i < N; i++)
        Swap(&A[i][max_j], &A[i][j]);
      sign = -sign;
    }

    for (k = j + 1; k < N; k++)
    {
      double coef = A[k][j] / A[j][j];

      for (i = j; i < N; i++)
        A[k][i] -= A[j][i] * coef;
    }
  }
  
  for (i = 0; i < N; i++)
    Det *= A[i][i];
  Det *= sign;
}

/* main function */
void main( void )
{
  /*for (i = 0; i < N; i++)
    P[i] = i + 1;       */

  MatrixLoad ("MATRIX.TXT");
  Gauss();
  /*Go(0);*/
  PrintDet();
} /* end of 'main' function */
