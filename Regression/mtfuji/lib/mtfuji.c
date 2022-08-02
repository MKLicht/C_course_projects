#include <stdio.h>
#include <stdlib.h>

typedef struct
{
  char *loc;   // location name
  double alt;  // altitude (km)
  double temp; // temperature (centigrade)
} Sample;

const Sample data[] =
{
    { "Nozawa-onsen",   0.576,  22.3 },
    { "Iiyama",         0.313,  23.3 },
    { "Nagano",         0.4182, 23.8 },
    { "Oomachi",        0.784,  21.1 },
    { "Sugadaira",      1.253,  18.5 },
    { "Karuizawa",      0.9991, 19.5 },
    { "Matsumoto",      0.610,  23.6 },
    { "Nagawa",         1.068,  19.7 },
    { "Suwa",           0.760,  22.7 },
    { "Nobeyama",       1.350,  18.4 },
    { "Ina",            0.633,  22.7 },
    { "Nagiso",         0.560,  22.3 },
    { "Iida",           0.5164, 23.9 },
    { "Minami-shinano", 0.407,  23.5 },
};

const int count = 14;

int comp_alt(const void* x0, const void *x1)
{
  const Sample *p0 = (Sample*)x0;
  const Sample *p1 = (Sample*)x1;

  if( p0->alt > p1->alt)
    return 1;
  else
    return -1;
  return 0;
}

int f_dimension()
{
  return 2;
}

double f_value(const double x[])
{
  double sum = 0.0;
  for(int i = 0; i < count; i++)
  {
      sum += (data[i].temp - (x[0] * data[i].alt + x[1])) * (data[i].temp - (x[0] * data[i].alt + x[1]));
  }
  return sum;
}

void f_gradient(const double x[], double g[])
{
  g[0] = 0.0;
  g[1] = 0.0;
  for(int i = 0; i < count; i++)
  {
      g[0] += (-2) * (data[i].temp - x[0] * data[i].alt - x[1]) * data[i].alt;
      g[1] += (-2) * (data[i].temp - x[0] * data[i].alt - x[1]);
  }
}

void sortalt()
{
 Sample data[] =
{
    { "Nozawa-onsen",   0.576,  22.3 },
    { "Iiyama",         0.313,  23.3 },
    { "Nagano",         0.4182, 23.8 },
    { "Oomachi",        0.784,  21.1 },
    { "Sugadaira",      1.253,  18.5 },
    { "Karuizawa",      0.9991, 19.5 },
    { "Matsumoto",      0.610,  23.6 },
    { "Nagawa",         1.068,  19.7 },
    { "Suwa",           0.760,  22.7 },
    { "Nobeyama",       1.350,  18.4 },
    { "Ina",            0.633,  22.7 },
    { "Nagiso",         0.560,  22.3 },
    { "Iida",           0.5164, 23.9 },
    { "Minami-shinano", 0.407,  23.5 },
};
  qsort(data, count,sizeof(data[0]),comp_alt);

  for (int i = 0 ; i < count ; i++){
    printf("%s: %lf %lf\n", data[i].loc, data[i].alt, data[i].temp);
  }
}
