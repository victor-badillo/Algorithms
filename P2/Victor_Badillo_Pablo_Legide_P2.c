/* Victor Badillo - victor.badillo@udc.es and Pablo Legide - p.legide@udc.es */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define LIM 20
#define THRESHOLD 1

double microseconds() {// measures in microseconds
  
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}


void ins_sort(int v[], int n){

    for(int i=1; i<n; i++){
        int x=v[i];
        int j=i-1;
        while(j>=0 && v[j]>=x){
            v[j+1]=v[j];
            j--;
        }
        v[j+1]=x;
    }
}


void init_seed() {
  
    srand(time(NULL));
}


void random_init(int v [], int n) {// generates pseudo-random numbers between -n y +n
  
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
}

int printRandom(int left, int right) {// generates a random number that we will use as a pivot for Quicksort
  
    int num = (rand() % (right - left + 1)) + left;
    return num;
}



void ascending_init(int v [], int n) {
   
    int i;
    for (i=0; i < n; i++)
        v[i] = i;
}

void descending_init(int v [], int n) {
   
    int i,j=0;
    for (i=n-1; i >= 0; i--){
        v[j] = i;
        j++;}
}


void swap(int *a, int *b) {
    
    int t = *a;
    *a = *b;
    *b = t;
}


void sort_aux(int v[], int left, int right){

    if (left+THRESHOLD <= right){
        int x = printRandom(left, right);
        int pivot=v[x];
        swap(&v[left],&v[x]);
        int i=left+1;
        int j=right;
        while (i<=j){
            while(i<=right && v[i]<pivot){
                i=i+1;
            }
            while(v[j]>pivot){
                j=j-1;
            }
            if (i<=j){
                swap(&v[i],&v[j]);
                i=i+1;
                j=j-1;
            }
        }
        swap(&v[left],&v[j]);
        sort_aux(v,left, j-1);
        sort_aux(v,j+1,right);
    }
}


void quick_sort(int v[], int n){ //Quicksort gets help of Insertion sort if threshold is higher than 1
    
    sort_aux(v, 0, n-1);
    if(THRESHOLD > 1)
        ins_sort(v,n);
}


void test(void (*insSortInit)(int[],int), char *name, char *alg) { //validates that the algorithms are working
   
    int numbers[LIM];
    (*insSortInit)(numbers,LIM);
    
    if(!strcmp(alg,"ins_sort")) printf("\nInsertion sort with %s initialization:\n", name); 
    else printf("\nQuicksort with %s initialization and threshold 1:\n", name);
    
    for(int i=0;i<=LIM-2;i++){
        printf("%d,", numbers[i]);
    }
    printf("%d\n", numbers[LIM-1]);
    printf("Sorted? 0\n");
    (!strcmp(alg,"ins_sort"))?ins_sort(numbers,LIM):quick_sort(numbers,LIM);
    for(int j=0;j<=LIM-2;j++){
        printf("%d,", numbers[j]);
    }
    printf("%d\n", numbers[LIM-1]);
    printf("Sorted\n\n\n");
}


void printAlg(void (*sortInit)(int[],int), char * name, double underBound, double tightBound, double overBound,void (*sortAlgo)(int[],int)) { // prints each algorithm table

    double t1, t2, t, x, y, z, taux1, taux2;
    
    if(sortAlgo==ins_sort) printf("\nInsertion sort with %s initialization:\n", name); 
    else printf("\nQuicksort with %s initialization and threshold %d:\n", name, THRESHOLD);
    
    printf("\t  n\t      t(n)\t  t(n)/n^%1.2f\t  t(n)/n^%1.2f\t t(n)/n^%1.2f\n",underBound,tightBound,overBound);
    printf("___________________________________________________________________________\n");
    for(int i=500; i<=32000; i*=2) {
        int numbers[i];
        (*sortInit)(numbers,i);

        t1 = microseconds();
        (*sortAlgo)(numbers,i);
        t2 = microseconds();
        t = t2-t1;
        if(t<500) { //times less than 500 microseconds
            t1 = microseconds();
            for(int p=0;p<1000;p++) {
                (*sortInit)(numbers,i);
                (*sortAlgo)(numbers,i);
            }
            t2 = microseconds();
            taux1 = (t2-t1);

            t1 = microseconds();
            for(int p = 0; p < 1000; p++){
                (*sortInit)(numbers,i);
            }
            t2 = microseconds();
            taux2 = (t2 -t1);

            t = (taux1 - taux2)/1000;
            x = t / pow(i,underBound);            //underestimated bound
            y = t / pow(i,tightBound);            //tight bound
            z = t / pow(i,overBound);             //overestimated bound
            printf("(*)%9d%15.3f%16.6f%16.6f%15.7f\n", i, t, x, y, z);
        }
        else {
            x = t / pow(i,underBound);            //underestimated bound
            y = t / pow(i,tightBound);            //tight bound
            z = t / pow(i,overBound);             //overestimated bound
            printf("%12d%15.3f%16.6f%16.6f%15.7f\n", i, t, x, y, z);
        }
    }
}

int main(){
   
    init_seed();
    
    test(random_init, "random","ins_sort");
    test(ascending_init, "ascending","ins_sort");
    test(descending_init, "descending","ins_sort");

    printAlg(random_init, "random", 1.8, 2, 2.2, ins_sort);
    printAlg(ascending_init, "ascending", 0.8, 1, 1.2, ins_sort);
    printAlg(descending_init,"descending", 1.8, 2, 2.2, ins_sort);

    printf("\n\n");

    test(random_init, "random","quicksort" );
    test(ascending_init, "ascending","quicksort");
    test(descending_init, "descending","quicksort");

    printAlg(random_init, "random", 0.8, 1.09, 1.4, quick_sort); 
    printAlg(ascending_init, "ascending", 0.8, 1.07, 1.4,quick_sort);
    printAlg(descending_init, "descending", 0.8, 1.06, 1.4,quick_sort);

    
    return 0;
}
