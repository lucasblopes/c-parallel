#include <omp.h>
#include <stdio.h>

static long num_steps = 1000000;
double step;

int main() {
    int i;
    double x, pi, sum = 0.0;

    step = 1.0 / (double)num_steps;
#pragma omp parallel for
    for (i = 0; i < num_steps; i++) {
        x = (i + 0.5) * step;
#pragma omp critical
        sum = sum + 4.0 / (1.0 + x * x);
    }

    pi = step * sum;
    printf("%.10f\n", pi);
}
