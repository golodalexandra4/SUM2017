/* FILE NAME: T06GAUSS.C
 * PROGRAMMER: AG4
 * DATE: 06.06.2017
 * PURPOSE: Solve SLAY.
 */

#include <stdio.h>
#include <math.h>
#define MAX 4

int N;
double A[MAX][MAX], B[MAX], X[MAX], Det = 1;

/* Get matrix from file */
void MatrixLoad( char *FileName )
{
  int i, j, k;
  FILE *F;

  if ((F = fopen(FileName, "r")) == NULL)
    return;

  fscanf(F, "%d", &N);

  if (N > MAX)
    N = MAX;
  
  for (i = 0; i < N; i++)
    for (j = 0; j < N + 1; j++)
      fscanf(F, "%lf", &A[i][j]);
  for (k = 0; k < N; k++)
  {
    B[k] = A[k][N];
    A[k][N] = 0;
  }

  fclose(F);
}

void Swap( double *A, double *B )
{
  double tmp = *A;

  *A = *B;
  *B = tmp;
}

/* change matrix */
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
      Det = 0;

    /*Swap rows */
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

void GetSolve( void ) /* solving system*/
{
  int i, j;
  double det1, det2;

  MatrixLoad ("SLAY.TXT");
  Gauss();
  det1 = Det;
  
  for (i = 0; i < N; i++)
  { 
    MatrixLoad ("SLAY.TXT");
    for (j = 0; j < N; j++)
      A[i][j] = B[j];
    Gauss();
    det2 = Det;
    if (det2 == 0)
      return;
    X[i] = det2 / det1;
  }
}

void PrintSolve( void ) /* print solve to file */
{
  int i;
  FILE *F;

  if ((F = fopen("SLAY.TXT", "a")) == NULL)
    return;
  for (i = 0; i < N; i++)
    fprintf(F, "%lf\n", X[i]);
  
  fclose(F);
}

/* main function */
void main( void )
{
  GetSolve();
  PrintSolve();
} /* end of 'main' function */