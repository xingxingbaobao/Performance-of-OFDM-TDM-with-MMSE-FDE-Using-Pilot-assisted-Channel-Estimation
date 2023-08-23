#include <stdio.h>
#include <stdlib.h>

#include "print.h"


void print_complex_vector(const char *title, complex *x, int n)
{
  int i;
  printf("%s (count=%d):", title, n);
  for(i=0; i<n; i++ ) printf(" (%5.2f,%5.2f) ", x[i].Re,x[i].Im);
  putchar('\n');
  return;
}


void print_float_vector(const char *title, double *x, int n)
{
  int i;
  printf("%s (count=%d):", title, n);
  for(i=0; i<n; i++ ) printf("( %0.4f )",x[i]);
  putchar('\n');
  return;
}

void print_int_vector(const char *title, int *x, int n)
{
  int i;
  printf("%s (count=%d):", title, n);
  for(i=0; i<n; i++ ) printf(" %d ",x[i]);
  putchar('\n');
  return;
}
