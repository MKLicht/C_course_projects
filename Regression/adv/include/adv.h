#ifndef ADV_REPRESSION_H
#define ADV_REPRESSION_H

int comp_alt(const void* x0, const void *x1);
int f_dimension();
double f_value(const double x[]);
void f_gradient(const double x[], double g[]);
void sortalt();

#endif