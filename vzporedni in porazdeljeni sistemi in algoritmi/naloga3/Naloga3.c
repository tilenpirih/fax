#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

//Prevedemo: gcc Naloga3.c -o Naloga3 -lpthread -O2
//Rezerviramo: salloc -n1 --cpus-per-task=<stevilo niti> --reservation=fri 

//N = stevilo elementov, NTHREADS = stevilo niti, MAX = najvecji element, MIN = najmanjsi element, OPTIMIZACIJA: 0 = vkljucena, 1 = izkljucena
#define N 100000
#define NTHREADS 8
#define MAX 100
#define MIN 1
#define OPTIMIZACIJA 1

pthread_t niti[NTHREADS];

pthread_barrier_t barrier;

pthread_mutex_t opt_mutex = PTHREAD_MUTEX_INITIALIZER;

int optimizacija;

struct params{
    int ID;
    int ind1;
    int ind2;
};

struct params arg[NTHREADS];

void * funkcija_niti(void * args);

int tab[N];

int check(){
    int prejsni = tab[0];
    for(int i=1; i<N; i++){
        if(prejsni > tab[i]){
            return 1;
        }
        prejsni = tab[i];
    }
    return 0;
}

void fillTab(){
    srand(time(NULL));
    for(int i=0; i<N; i++){
        tab[i] = rand() % MAX + MIN;
    }
}

void * funkcija_niti(void * args){

    struct params * p = (struct params *) args;

    int ID = p->ID;
    int ind1 = p->ind1;
    int ind2 = p->ind2;

    int numSwap;
    
    for(int j=0; j<N; j++){
        numSwap = 0;
        if(j % 2 == 0){
            for(int i=ind1; i<ind2; i+=2){
                if(i + 1 < N){
                    if(tab[i] > tab[i + 1]){
                        int temp = tab[i];
                        tab[i] = tab[i + 1];
                        tab[i + 1] = temp;
                        numSwap++;
                    }
                }
            }
        }
        else{
            for(int i=ind1 + 1; i<ind2; i+=2){
                if(i + 1 < N){
                    if(tab[i] > tab[i + 1]){
                        int temp = tab[i];
                        tab[i] = tab[i + 1];
                        tab[i + 1] = temp;
                        numSwap++;
                    }
                }
            }
        }
        if(OPTIMIZACIJA == 0){
            if(numSwap == 0){
                pthread_mutex_lock(&opt_mutex);
                optimizacija++;
                pthread_mutex_unlock(&opt_mutex);
            }
            pthread_barrier_wait(&barrier);
            if(optimizacija == NTHREADS){
                return 0;
            }
            else if(optimizacija != 0){
                optimizacija = 0;
            }
        }
        else{
            pthread_barrier_wait(&barrier);
        }
    }
    return 0;
}

int main(){

    pthread_barrier_init(&barrier, NULL, NTHREADS);

    fillTab();
    
    optimizacija = 0;

    struct timeval  time1, time2;
    gettimeofday(&time1, NULL);
    
    int tmp_i1 = 0;
    int tmp_i2 = N / NTHREADS;
    for(int i=0; i<NTHREADS; i++){
        arg[i].ID = i;
        arg[i].ind1 = tmp_i1;
        arg[i].ind2 = tmp_i2;

        pthread_create(&niti[i], NULL, funkcija_niti, (void *)(&arg[i]));

        tmp_i1 = tmp_i2;
        tmp_i2 = tmp_i1 + (i + 2) * N / NTHREADS - (i + 1) * N / NTHREADS;
    }

    for(int i=0; i<NTHREADS; i++){
        pthread_join(niti[i], NULL);
    }

    gettimeofday(&time2, NULL);
    double seconds = (double) (time2.tv_usec - time1.tv_usec) / 1000000 + (double) (time2.tv_sec - time1.tv_sec);

    if(check() == 0){
        if(OPTIMIZACIJA == 0){
            printf("Time=%.2f seconds, N=%d, %d threads, without optimization.\n", seconds,N, NTHREADS);
        }
        else{
            printf("Time=%.2f seconds, N=%d, %d threads, without optimization.\n", seconds,N, NTHREADS);
        }
    } 

    return 0;
}
