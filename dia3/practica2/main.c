// Note : LINUX users, uncomment the next line
// #include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void set_value (float *ptr, float value, int nb);

int main () {
  int i, failed=0;
  int N = 1024; // size of vector
  float *ptr;

  /* Allocate vector */
  ptr = malloc(sizeof(float)*N);

  /* Call our function. This is NOT a direct kernel invocation */
  set_value (ptr, 23.0, N);

  /* Now check that it did what we want */

  for (i = 0; i < 10; i++)//first ten values are written
    printf ("%f\t", ptr[i]);
  printf ("\n");
  for (i = N-10; i < N; i++)//last ten values are written
    printf ("%f\t", ptr[i]);
  printf ("\n");

  for (i = 0; i < N; i++) {//All values are compared
    if (fabs(ptr[i]-23.0) > 1e-8) {
      failed=1;
    }
  }
  if (failed) {
    printf ("FAILED !!\n");
  } else {
    printf ("PASSED !!\n");
  }    
  free (ptr);
}
