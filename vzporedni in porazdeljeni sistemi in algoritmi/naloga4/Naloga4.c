#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <omp.h>
#include <pthread.h>
// rezerviramo: salloc -n1 --cpus-per-task=32 --reservation=fri
// prevedemo: gcc Naloga4.c -o Naloga4 -lm -fopenmp -O2
#define N 100000
#define NTHREADS 8

pthread_mutex_t nClients_mutex;

int A[N];

void findDivisors(int n)
{
    int vs = 1;

    for (int i = 2; i <= sqrt(n); i++)

    {
        if (n % i == 0)
        {
            if (n / i == i)
                vs += i;

            else
                vs += i + n / i;
        }
    }
    A[n] = vs;
}

int main()
{
    struct timeval time1, time2;
    gettimeofday(&time1, NULL);

    omp_set_num_threads(NTHREADS);
    int vs = 0;

//#pragma omp parallel for schedule(dynamic, 1000)
#pragma omp parallel for schedule(auto)

    for (int i = 1; i < N; i++)

    {
        findDivisors(i);
    }

#pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (A[i] == j && A[j] == i && i != j)
            {
                pthread_mutex_lock(&nClients_mutex);
                vs += 1;
                pthread_mutex_unlock(&nClients_mutex);
                
                printf("%d %d sta prijatelski števili \n", i, j);
            }
        }
    }
    gettimeofday(&time2, NULL);
    double seconds = (double)(time2.tv_usec - time1.tv_usec) / 1000000 + (double)(time2.tv_sec - time1.tv_sec);
    printf("\nN: %d\nŠt prijateljskih števil: %d \nThreadov: %d \nTime: %.3f\n", N, vs, NTHREADS, seconds);
    return 0;
}
/*
narejeni testi na različnih metodah
+----------------+--------------+--------------+---------------+------------+-------------+-------+
| threads/method | dynamic (10) | dynamic(100) | dynamic(1000) | guided(10) | guided(100) | auto  |
+----------------+--------------+--------------+---------------+------------+-------------+-------+
| 1              | 7.661        | 7.644        | 7.629         | 7.647      | 7.663       | 6.129 |
+----------------+--------------+--------------+---------------+------------+-------------+-------+
| 2              | 5.820        | 5.711        | 6.026         | 5.726      | 5.746       | 4.626 |
+----------------+--------------+--------------+---------------+------------+-------------+-------+
| 4              | 3.582        | 3.404        | 3.367         | 3.399      | 3.476       | 2.697 |
+----------------+--------------+--------------+---------------+------------+-------------+-------+
| 8              | 2.110        | 1.869        | 2.033         | 1.889      | 2.036       | 1.525 |
+----------------+--------------+--------------+---------------+------------+-------------+-------+
*/