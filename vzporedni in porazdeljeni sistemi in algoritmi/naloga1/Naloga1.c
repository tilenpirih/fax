#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
double *Random(int n){
    srand ( time ( NULL));
    double *loc;
    loc = (double *)calloc(n,8);
    for(int i=0;i<n;i++){
        loc[i]=(double)rand()/RAND_MAX;
        printf("%f ",loc[i]);
    }
    printf("\n\n");
    return loc;
}
double printNumbers(double *loc,int n){
    for(int i=0; i<n;i++){
        printf("%f ",loc[i]);
    }
}

double ** Matrix(double *A, int n, int r, int c){
    double **M = (double **)calloc(r, sizeof(double));
    for(int i=0; i<r;i++){
        M[i]=(double *)calloc(c, sizeof(double));
    }
    int st=0;
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            if(n >= st){
                M[i][j]=A[st];
                st++;
            }
            else{
                M[i][j]=0.0;
            }
            printf("%f ",M[i][j]);
        }
        printf("\n");
    }
    return M;
}

double * Max(double *A, int n){
    double *max=0;
    double najv=0;
    for(int i=0;i<n;i++){
        if(A[i]>najv){
            najv=A[i];
            max=&A[i];
        }
    }

    printf("\nVrednost: %f naslov: %p",najv,max);
    return max;
}

int main() {
    int n=7;
    int r=4;
    int c=5;
    double *tab=Random(n);
    Matrix(tab,n,r,c);
    double *max=Max(tab,n);

}