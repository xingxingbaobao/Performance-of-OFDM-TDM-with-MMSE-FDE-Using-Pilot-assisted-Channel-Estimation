#ifndef _print_H_
#define _print_H_

#include "configurations.h"

void print_complex_vector(const char *title, complex *x, int n);
void print_float_vector(const char *title, double *x, int n);
void print_int_vector(const char *title, int *x, int n);

#endif