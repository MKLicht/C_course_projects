#include <stdio.h>
#include <stdlib.h>
#include "optimize.h"
#include "mtfuji.h"

int main(const int argc, const char **argv)
{
  // 引数の個数が1の時だけ、alpha に第1引数を採用し、それ以外は0.01
  const double alpha = (argc == 2) ? atof(argv[1]) : 0.01;

  const int dim = f_dimension();

  double *x = malloc(dim * sizeof(double));
  for (int i = 0; i < dim; i++) {
    x[i] = 0;
  }

  printf("alpha = %f\n", alpha);

  const int iter = optimize(alpha, dim, x, f_gradient);

  printf("number of iterations = %d\n", iter);

  double temp = 0.0;
  temp = x[0]*3.776 + x[1];
  printf("The temperature of Mt.Fuji is %lf\n", temp);

  sortalt();

  free(x);

  return 0;
}

