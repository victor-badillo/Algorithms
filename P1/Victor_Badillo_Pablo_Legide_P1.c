#include <stdio.h>
#include <math.h>
#include <sys/time.h>
/* obtains the current time in microseconds */
double microseconds() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

int fib1(int n) {
    if(n<2) {
        return n;
    }
    else {
        return fib1(n-1)+ fib1(n-2);
    }
}

int fib2(int n) {
    int i, j;
    i=1; j=0;
    for(int k=1; k <= n; k++) {
        j= i+j;
        i= j-i;
    }
    return j;
}

int fib3(int n) {
    int i, j, k, h, t;
    i=1; j=0; k=0; h=1;
    while (n>0) {
        if ((n%2)!=0) {
            t=j*h;
            j=(i*h)+(j*k)+t;
            i=(i*k)+t;
        }
        t= h*h;
        h= (2*k*h)+t;
        k= (k*k)+t;
        n= n/2;
    }
    return j;
}

void chfib1() { //Function to print fib1 table
    int i=2, p=0;
    double t1, t2, t, x, y, z;

    for(int k=0; k < 5; k++) {

        t1 = microseconds();
        fib1(i);
        t2 = microseconds();
        t = t2-t1;
        if(t<500) { //times less than 500 microseconds
            t1 = microseconds();
            for(p=0;p<1000;p++) {
                fib1(i);
            }
            t2 = microseconds();
            t = (t2-t1)/1000;
            x = t / pow(1.1,i);             //underestimated bound
            y = t / pow(((1+sqrt(5))/2),i); //tight bound
            z = t / pow(2, i);              //overestimated bound
            printf("%12d%15.3f%15.6f%15.6f%15.6f\n", i, t, x, y, z);
        }
        else {
            x = t / pow(1.1,i);             //underestimated bound
            y = t / pow(((1+sqrt(5))/2),i); //tight bound
            z = t / pow(2, i);              //overestimated bound
            printf("%12d%15.3f%15.6f%15.6f%15.6f\n", i, t, x, y, z);
        }
        i*=2;
    }
}

void chfib2() { //Function to print fib2 table
    int j=1000, p=0;
    double t1, t2, t, x, y, z;

    for (int k = 0; k < 5; k++) {

        t1 = microseconds();
        fib2(j);
        t2 = microseconds();
        t = (t2 - t1);
        if(t<500) { //times less than 500 microseconds
            t1 = microseconds();
            for(p=0;p<1000;p++) {
                fib2(j);
            }
            t2 = microseconds();
            t = (t2 - t1)/1000;
            x = t / pow(j, 0.8); //underestimated bound
            y = t / j;           //tight bound
            z = t / j*log(j);    //overestimated bound
            printf("%12d%15.3f%15.6f%15.6f%15.6f\n", j, t, x, y, z);
        }
        else {
            x = t / pow(j, 0.8); //underestimated bound
            y = t / j;           //tight bound
            z = t / j*log(j);    //overestimated bound
            printf("%12d%15.3f%15.6f%15.6f%15.6f\n", j, t, x, y, z);
        }
        j *= 10;
    }
}

void chfib3() { //Function to print fib3 table
    int j = 1000,p=0;
    double t1, t2, t, x, y, z;

    for (int k = 0; k < 5; k++) {

        t1 = microseconds();
        fib3(j);
        t2 = microseconds();
        t = (t2 - t1);
        if(t<500) { //times less than 500 microseconds
            t1 = microseconds();
            for(p=0;p<1000;p++) {
                fib3(j);
            }
            t2 = microseconds();
            t = (t2 - t1)/1000;
            x = t / sqrt(log(j)); //underestimated bound
            y = t / log(j);       //tight bound
            z = t / pow(j, 0.5);  //overestimated bound
            printf("%12d%15.3f%15.6f%15.6f%15.6f\n", j, t, x, y, z);
        }
        else {
            x = t / sqrt(log(j)); //underestimated bound
            y = t / log(j);       //tight bound
            z = t / pow(j, 0.5);  //overestimated bound
            printf("%12d%15.3f%15.6f%15.6f%15.6f\n", j, t, x, y, z);
        }
        j *= 10;
    }
}


void test() { //Function to validate that the algorithms are working
    int i = 2, j = 1000 , r= 1000;

    for (int k = 0; k < 5; k++) {
        printf("%12d%12d",i, fib1(i));
        i *= 2;
    }
    printf("\n");
    for (int k = 0; k < 5; k++) {
        printf("%12d%12d",j, fib2(j));
        j *= 10;
    }
    printf("\n");
    for (int k = 0; k < 5; k++) {
        printf("%12d%12d",r, fib3(r));
        r *= 10;
    }
}


int main() {
    test();
    printf("\nAlgorithmFib1:\n");
    chfib1();
    printf("\nAlgorithmFib2:\n");
    chfib2();
    printf("\nAlgorithmFib3:\n");
    chfib3();
}
