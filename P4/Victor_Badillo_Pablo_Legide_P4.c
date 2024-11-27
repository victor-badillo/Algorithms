#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>


#define LIM 10


#define SZ 512000
typedef struct {
    int vector[SZ];
    int last;
} heap;


void swap(int *a, int *b) {

    int t = *a;
    *a = *b;
    *b = t;
}


void percolateDown(heap * h, int i){

    int largest = i;
    int leftChild = 2*i+1;
    int rightChild = 2*i+2;

    if(leftChild < h->last && h->vector[leftChild] > h->vector[largest])
        largest = leftChild;
    if(rightChild < h->last && h->vector[rightChild] > h->vector[largest])
        largest = rightChild;
    if(largest != i){
        swap(&h->vector[i], &h->vector[largest]);
        percolateDown(h, largest);
    }
}


void create_heap(int v[], int n, heap * h){

    for(int i=0; i<n; i++){
        h->vector[i] = v[i];
    }
    h->last = n;
    int lastNonLeafNode = (n/2)-1;
    for(int i = lastNonLeafNode; i>=0; i--){
        percolateDown(h,i);
    }
}


void printHeap(heap * h){

    for(int i=0; i<h->last; i++){
        printf("%d ", h->vector[i]);
    }
    printf("\n");
}


bool isEmptyHeap(heap * h){

    return (h->last == 0);
}


int remove_max(heap * h){
    if(isEmptyHeap(h)) return -1;
    int x = h->vector[0];
    h->vector[0] = h->vector[h->last-1];
    h->last = (h->last)-1;
    if (h->last > 0) percolateDown(h, 0);
    return x;
}


void heap_sort(int v[], int n){
    heap h;
    create_heap(v, n ,&h);
    for(int i=n-1; i>=0; i--){
        v[i] = remove_max(&h);
    }
}


double microseconds() {// measures in microseconds
  
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}


void init_seed() {
  
    srand(time(NULL));
}


void random_init(int v [], int n) {// generates pseudo-random numbers between -n y +n
  
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
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


void testHeap(void (*init)(int[],int), char *name) { //validates that the algorithms are working
    
    heap h1;
    int numbers[LIM];
    (*init)(numbers,LIM);

    printf("Array with %s initialization:\n", name);
    for(int i=0;i<LIM;i++){
        printf("%d ", numbers[i]);
    }
    printf("\nCreate heap with %s initialization:\n", name);
    create_heap(numbers, 10, &h1);
    printHeap(&h1);
    printf("Remove max from %s initialization heap:\n", name);
    printf("Max = %d\n",remove_max(&h1));
    printHeap(&h1);
    printf("\n");
} 
    

void printTable1(void (*sortInit)(int[],int), double underBound, double tightBound, double overBound) { // prints each algorithm table

    double t1, t2, t, x, y, z, taux1, taux2;
    heap h2;
    
    printf("\t  n\t      t(n)\t  t(n)/n^%1.2f\t  t(n)/n^%1.2f\t t(n)/n^%1.2f\n",underBound,tightBound,overBound);
    printf("___________________________________________________________________________\n");
    for(int i=500; i<=32000; i*=2) {
        int numbers[i];
        (*sortInit)(numbers,i);

        t1 = microseconds();
        create_heap(numbers,i,&h2);
        t2 = microseconds();
        t = t2-t1;
        if(t<500) { //times less than 500 microseconds
            t1 = microseconds();
            for(int p=0;p<1000;p++) {
                (*sortInit)(numbers,i);
                create_heap(numbers,i,&h2);
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


void testHeapSort(void (*init)(int[],int), char *name) { //validates that the algorithms are working
    
    int numbers[LIM];
    (*init)(numbers,LIM);

    printf("Array with %s initialization:\n", name);
    for(int i=0;i<LIM;i++){
        printf("%d ", numbers[i]);
    }
    printf("\nTest heap sort with %s initialization:\n", name);
    heap_sort(numbers, LIM);
    for(int j=0; j<LIM; j++){
        printf("%d ",numbers[j]);
    }
    printf("\nFinished\n\n");
} 

void printTable2(void (*sortInit)(int[],int), char *initialization, double underBound, double tightBound, double overBound) { // prints each algorithm table

    double t1, t2, t, x, y, z, taux1, taux2;
    printf("\n Heap sort with %s initialization:\n\n", initialization);
    printf("\t  n\t      t(n)\t  t(n)/n^%1.2f\t  t(n)/n^%1.2f\t t(n)/n^%1.2f\n",underBound,tightBound,overBound);
    printf("___________________________________________________________________________\n");
    for(int i=500; i<=32000; i*=2) {
        int numbers[i];
        (*sortInit)(numbers,i);

        t1 = microseconds();
        heap_sort(numbers, i);
        t2 = microseconds();
        t = t2-t1;
        if(t<500) { //times less than 500 microseconds
            t1 = microseconds();
            for(int p=0;p<1000;p++) {
                (*sortInit)(numbers,i);
                heap_sort(numbers, i);
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


int main() {
    init_seed();
    testHeap(ascending_init, "ascending");
    testHeap(random_init, "random");
    testHeap(descending_init,"descending");
    printTable1(ascending_init, 0.8, 1, 1.2);
    printf("\n\n"); 
    testHeapSort(ascending_init, "ascending");
    testHeapSort(random_init, "random");
    testHeapSort(descending_init, "descending");
    printTable2(ascending_init, "ascending", 0.8, 1.09, 1.3);; 
    printTable2(random_init, "random", 0.8, 1.10, 1.3);
    printTable2(descending_init, "descending", 0.8, 1.12, 1.3);
    return 0;
}
