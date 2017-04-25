#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define SIZE 100   // Size by SIZE matrices
int num_thrd;   // number of threads

int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];

// initialize a matrix
void init_matrix(int m[SIZE][SIZE], FILE *fp )
{
  int i, j, val = 0;
  for (i = 0; i < SIZE; i++)
    for (j = 0; j < SIZE; j++)
    fscanf(fp, "%d", &val);
      m[i][j] = val++;
}

void print_matrix(int m[SIZE][SIZE])
{
  int i, j;
  for (i = 0; i < SIZE; i++) {
    printf("\n\t| ");
    for (j = 0; j < SIZE; j++)
      printf("%2d ", m[i][j]);
    printf("|");
  }
}

void* multiply(void* slice)
{
  int s = (int)slice;
  int from = (s * SIZE)/num_thrd;
  int to = ((s+1) * SIZE)/num_thrd;
  int i,j,k;

  printf("multiplicando %d (de %d ate %d)\n", s, from, to-1);
  for (i = from; i < to; i++)
  {
    for (j = 0; j < SIZE; j++)
    {
      C[i][j] = 0;
      for ( k = 0; k < SIZE; k++)
 C[i][j] += A[i][k]*B[k][j];
    }
  }
  printf("finished slice %d\n", s);
  return 0;
}

int main(int argc, char* argv[])
{
  pthread_t* thread;
  int i;
  FILE *fp1, *fp2;

  num_thrd = atoi(argv[1]);
  fp1 = fopen(argv[2], "r");
  fp2 = fopen(argv[3], "r");
  init_matrix(A, fp1);
  init_matrix(B, fp2);
  thread = (pthread_t*) malloc(num_thrd*sizeof(pthread_t));

  for (i = 1; i < num_thrd; i++)
  {
    if (pthread_create (&thread[i], NULL, multiply, (void*)i) != 0 )
    {
      perror("Can't create thread");
      free(thread);
      exit(-1);
    }
  }

  multiply(0);//"segurando" a main thread
  for (i = 1; i < num_thrd; i++)
 pthread_join (thread[i], NULL);

  printf("\n\n");
  print_matrix(A);
  printf("\n\n\t       * \n");
  print_matrix(B);
  printf("\n\n\t       = \n");
  print_matrix(C);
  printf("\n\n");

  free(thread);

  return 0;

}
