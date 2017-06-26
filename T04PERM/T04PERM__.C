/* FILE NAME: T04PERM.C
 * PROGRAMMER: AG4
 * DATE: 05.06.2017
 * PURPOSE: Get all perpermetations and their parity.
 */

#include <stdio.h>

#define MAX 10

int N = 5;
int P[MAX];
int parity = 0;

void Swap( int *A, int *B);
void Go( int Pos );
void PrintPerm( void );

void main( void )
{
  int i;

  for (i = 0; i < N; i++)
    P[i] = i + 1;
}

void Go( int Pos )
{
  int i;

  if (Pos >= N)
  {  
    //PrintPerm();
    return;
  for (i = Pos; i < N; i++)
  {
    Swap(&P[Pos], &P[i]);
    Go(Pos + 1);
    Swap(&P[Pos], &P[i]);
  }
}

void PrintPerm( void )
{
  FILE *F;
  int i;

  F = fopen("PERM.TXT", "a");
  if (F == NULL)
    return;

  for (i = 0; i < N; i++)
    fprintf(F, "%i ", P[i]);
  fprintf(F, "- %s", parity ? "odd" : "even");

  fclose(F);
}

void Swap( int *A, int *B)
{
  int tmp;
  
  tmp = *A;
  *A = *B;
  *B = tmp;
}