#ifndef MTFUJI_MTFUJI_H
#define MTFUJI_MTFUJI_H

int comp_alt(const void* x0, const void *x1);
int f_dimension();
double f_value(const double x[]);
void f_gradient(const double x[], double g[]);
void sortalt();

#endif