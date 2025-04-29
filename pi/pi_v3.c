#include <omp.h>
#include <stdio.h>

static long num_steps = 1000000;
double step;
#define NUM_THREADS 2

int main() {
    int i, nthreads;
    double pi = 0.0;
    step = 1.0 / (double)num_steps;
    omp_set_num_threads(NUM_THREADS);

#pragma omp parallel num_threads(NUM_THREADS)
    {
        int i, id, nthrds;
        double x, sum = 0.0;
        id = omp_get_thread_num();
        nthrds = omp_get_num_threads();
        if (id == 0) nthreads = nthrds;  // avoid race condition
        // cyclic distribution
        for (i = id, sum = 0.0; i < num_steps; i = i + nthrds) {
            x = (i + 0.5) * step;
            // The array can cause false-sharing, if when not shared data shares the same line in
            // cache
            sum += 4.0 / (1.0 + x * x);
        }
#pragma omp critical
        // uses critical so only one thread can acess sum at a time (can use atomic)
        pi += sum * step;
    }
    printf("%.10f\n", pi);
}
