#include <math.h>
#include <stdio.h>

double my_sin (double x);
/*{
  double answer = x;
  double old_answer = 0.0;
  double sign = 1.0;
  double part = x;
  double fac = 1.0;
  double xn = x;
  for (int i = 3; answer != old_answer; i += 2)
    {
      sign *= -1.0;
      fac *= i * (i - 1);
      xn *= x * x;
      part = xn / fac;
      part *= sign;
      old_answer = answer;
      answer += part;
    }
  return answer;
}*/

// #define PI 3.14159265

int
main (void)
{
  double x = M_PI / 2;
  double result = sin (x);
  printf ("%lf %lf\n", result, my_sin (x));
  return 0;
}