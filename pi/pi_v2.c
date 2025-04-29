#include <omp.h>
#include <stdio.h>

static long num_steps = 1000000;
double step;
#define NUM_THREADS 2

int main() {
    int i, nthreads;
    double pi, sum[NUM_THREADS];
    step = 1.0 / (double)num_steps;

#pragma omp parallel num_threads(NUM_THREADS)
    {
        int i, id, nthrds;
        double x;
        id = omp_get_thread_num();
        nthrds = omp_get_num_threads();
        if (id == 0) nthreads = nthrds;  // avoid race condition
        // cyclic distribution
        for (i = id, sum[id] = 0.0; i < num_steps; i = i + nthrds) {
            x = (i + 0.5) * step;
            // The array can cause false-sharing, if when not shared data shares the same line in
            // cache
            sum[id] += 4.0 / (1.0 + x * x);
        }
    }
    for (i = 0, pi = 0.0; i < nthreads; i++) pi += sum[i] * step;
    printf("%.10f\n", pi);
}
