/* FILE NAME: T04PERM.C
 * PROGRAMMER: AG4
 * DATE: 05.06.2017
 * PURPOSE: Get all permetations from N elements and their parity.
 */

#include <stdio.h>
#define MAX 10

int N, p[MAX], parity;

void Go( int Pos );
void PrintPerm( void );
void Swap( int *A, int *B );

/* main function */
void main( void )
{
  int i;

  printf("Input a number: ");
  scanf("%i", &N);

  parity = 0;
  for (i = 0; i < N; i++)
    p[i] = i + 1;
  Go(0);
} /* end of main function */

void Go( int Pos ) /* get permetations */
{
  int i;

  if (Pos >= N)
  {
    PrintPerm();
    return;
  }

  for (i = Pos; i < N; i++)
  {
    Swap(&p[Pos], &p[i]);
    if(Pos != i)
      parity = !parity;
    Go(Pos + 1);
    Swap(&p[Pos], &p[i]);
  }
}

void PrintPerm( void ) /* print permetations to file */
{
  int i;
  FILE *F;

  F = fopen("PERM.TXT", "a");

  if(F == NULL)
  {  
    return;
    printf("ERROR!");
  }

  for (i = 0; i < N; i++)
    fprintf(F, "%i ", p[i]);
  
  fprintf(F, "- %s\n", parity ? "odd" : "even");

  fclose(F);
}

void Swap( int *A, int *B ) /* change two elements */
{
  int tmp;

  tmp = *A;
  *A = *B;
  *B = tmp;
}
